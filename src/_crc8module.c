/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crc8module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.1
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-10 08:42AM
*                       2017-08-17 [Heyn] Optimized code.
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2017-08-22 [Heyn] Bugfixes Parsing arguments
*                                         Change PyArg_ParseTuple(* , "y#|I")
*                                         To     PyArg_ParseTuple(* , "y#|B")
*                                         "B" : Convert a Python integer to a tiny int without overflow
*                                               checking, stored in a C unsigned char.
*                       2017-09-21 [Heyn] Optimized code for _crc8_maxim
*                                         New CRC8-MAXIM8 CRC8-ROHC CRC8-ITU8 CRC8
*
*********************************************************************************************************
*/

#include <Python.h>

#define                 TRUE                                    1
#define                 FALSE                                   0

#define                 MAX_TABLE_ARRAY                         256

static unsigned char	crc8_tab_shift_8c[MAX_TABLE_ARRAY]      = {0x00};
static unsigned char	crc8_tab_shift_e0[MAX_TABLE_ARRAY]      = {0x00};
static unsigned char	crc8_tab_shift_07[MAX_TABLE_ARRAY]      = {0x00};

static int              crc8_tab_shift_8c_init                  = FALSE;
static int              crc8_tab_shift_e0_init                  = FALSE;
static int              crc8_tab_shift_07_init                  = FALSE;

/*
 * Width            = 8
 * InitValue(crc8)  = 0x00
 */
unsigned char hz_calc_crc8_bcc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc ^= pSrc[i];
    }

	return crc;
}

/*
 * Width            = 8
 * InitValue(crc8)  = 0x00
 */
unsigned char hz_calc_crc8_lrc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc += pSrc[i];
	}
    crc = (~crc) + 0x01;

	return crc;
}

/*
*********************************************************************************************************
*                                   POLY=0x31 [MAXIM8]
* Poly:    0x31
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Alias:   DOW-CRC,CRC-8/IBUTTON
* Use:     Maxim(Dallas)'s some devices,e.g. DS18B20
* 0x8C = reverse 0x31
*********************************************************************************************************
*/
static void _init_crc8_table_8c( void )
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x01 ) crc = ( crc >> 1 ) ^ 0x8C;
			else                    crc =   crc >> 1;
			c = c >> 1;
        }
        crc8_tab_shift_8c[i] = crc;
    }
	crc8_tab_shift_8c_init = TRUE;
}

static unsigned char _hz_update_crc8_8c( unsigned char crc8, unsigned char c ) 
{
    unsigned char crc = crc8;
    if ( ! crc8_tab_shift_8c_init ) _init_crc8_table_8c();
    crc = crc8_tab_shift_8c[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc8_8c( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc8_8c(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
*                                   POLY=0x07 [ROHC]
* Poly:    0x07
* Init:    0xFF
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0xE0 = reverse 0x07
*********************************************************************************************************
*/
static void _init_crc8_table_e0( void )
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x01 ) crc = ( crc >> 1 ) ^ 0xE0;
			else                    crc =   crc >> 1;
			c = c >> 1;
        }
        crc8_tab_shift_e0[i] = crc;
    }
	crc8_tab_shift_e0_init = TRUE;
}

static unsigned char _hz_update_crc8_e0( unsigned char crc8, unsigned char c ) 
{
    unsigned char crc = crc8;
    if ( ! crc8_tab_shift_e0_init ) _init_crc8_table_e0();
    crc = crc8_tab_shift_e0[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc8_e0( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc8_e0(crc, pSrc[i]);
	}
	return crc;
}


static void _init_crc8_table_07( void )
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ 0x07;
			else                    crc =   crc << 1;
			c = c << 1;
        }
        crc8_tab_shift_07[i] = crc;
    }
	crc8_tab_shift_07_init = TRUE;
}

static unsigned char _hz_update_crc8_07( unsigned char crc8, unsigned char c ) 
{
    unsigned char crc = crc8;
    if ( ! crc8_tab_shift_07_init ) _init_crc8_table_07();
    crc = crc8_tab_shift_07[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc8_07( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc8_07(crc, pSrc[i]);
	}
	return crc;
}


static PyObject * _crc8_intel(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_lrc(data, data_len, crc8);

    return Py_BuildValue("B", result);
}

static PyObject * _crc8_bcc(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_bcc(data, data_len, crc8);

    return Py_BuildValue("B", result);
}

static PyObject * _crc8_lrc(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_lrc(data, data_len, crc8);

    return Py_BuildValue("B", result);
}

static PyObject * _crc8_maxim(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_8c(data, data_len, crc8);

    return Py_BuildValue("B", result);
}

static PyObject * _crc8_rohc(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0xFF;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_e0(data, data_len, crc8);

    return Py_BuildValue("B", result);
}


static PyObject * _crc8_itu(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_07(data, data_len, crc8);
    result = result ^ 0x55;
    return Py_BuildValue("B", result);
}

static PyObject * _crc8(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00;
    unsigned char crc8    = 0x00;
    unsigned char result  = 0x00;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|B", &data, &data_len, &crc8))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|B", &data, &data_len, &crc8))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc8_07(data, data_len, crc8);

    return Py_BuildValue("B", result);
}

/* method table */
static PyMethodDef _crc8Methods[] = {
    {"intel",   _crc8_intel,    METH_VARARGS, "Calculate Intel hexadecimal of CRC8 [Initial = 0x00]"},
    {"bcc",     _crc8_bcc,      METH_VARARGS, "Calculate BCC of CRC8 [Initial = 0x00]"},
    {"lrc",     _crc8_lrc,      METH_VARARGS, "Calculate LRC of CRC8 [Initial = 0x00]"},
    {"maxim8",  _crc8_maxim,    METH_VARARGS, "Calculate MAXIM of CRC8 [Poly = 0x31 Initial = 0x00 Xorout=0x00 Refin=True Refout=True] e.g. DS18B20"},
    {"rohc",    _crc8_rohc,     METH_VARARGS, "Calculate ROHC of CRC8 [Poly = 0x07 Initial = 0xFF Xorout=0x00 Refin=True Refout=True]"},
    {"itu8",    _crc8_itu,      METH_VARARGS, "Calculate ITU  of CRC8 [Poly = 0x07 Initial = 0x00 Xorout=0x55 Refin=False Refout=False]"},
    {"crc8",    _crc8,          METH_VARARGS, "Calculate CRC  of CRC8 [Poly = 0x07 Initial = 0x00 Xorout=0x00 Refin=False Refout=False]"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR(_crc8_doc,
"Calculation of CRC8 \n"
"libscrc.intel  -> Calculate Intel hexadecimal of CRC8 [Initial = 0x00]\n"
"libscrc.bcc    -> Calculate BCC(XOR) of CRC8 [Initial = 0x00]\n"
"libscrc.lrc    -> Calculate LRC of CRC8 [Initial = 0x00]\n"
"libscrc.maxim8 -> Calculate MAXIM of CRC8 [Poly = 0x31 Initial = 0x00 Xorout=0x00 Refin=True Refout=True] e.g. DS18B20\n"
"libscrc.rohc   -> Calculate ROHC of CRC8 [Poly = 0x07 Initial = 0xFF Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.itu8   -> Calculate ITU  of CRC8 [Poly = 0x07 Initial = 0x00 Xorout=0x55 Refin=False Refout=False]\n"
"libscrc.crc8   -> Calculate CRC  of CRC8 [Poly = 0x07 Initial = 0x00 Xorout=0x00 Refin=False Refout=False]\n"
"\n");


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc8module = {
   PyModuleDef_HEAD_INIT,
   "_crc8",                    /* name of module */
   _crc8_doc,                  /* module documentation, may be NULL */
   -1,                         /* size of per-interpreter state of the module */
   _crc8Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc8(void)
{
    PyObject *m = NULL;

    m = PyModule_Create(&_crc8module);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddStringConstant(m, "__version__", "0.1.1");
    PyModule_AddStringConstant(m, "__author__",  "Heyn");

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc8(void)
{
    (void) Py_InitModule3("_crc8", _crc8Methods, _crc8_doc);
}

#endif /* PY_MAJOR_VERSION */
