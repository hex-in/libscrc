/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crc32module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.0
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-09 16:39PM
*                       2017-08-17 [Heyn] Optimized Code.
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2017-09-22 [Heyn] Optimized Code.
*                                         New get crc32 table.
*
*Web : https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
*
*********************************************************************************************************
*/

#include <Python.h>

#define                 TRUE                                            1
#define                 FALSE                                           0

#define                 MAX_TABLE_ARRAY                                 256

#define		            CRC32_POLYNOMIAL_04C11DB7		                0x04C11DB7L
#define		            CRC32_POLYNOMIAL_EDB88320		                0xEDB88320L

static unsigned int     crc32_tab_shift_04c11db7[MAX_TABLE_ARRAY]       = {0x00000000};
static unsigned int     crc32_tab_shift_edb88320[MAX_TABLE_ARRAY]       = {0x00000000};

static int              crc32_tab_shift_04c11db7_init                   = FALSE;
static int              crc32_tab_shift_edb88320_init                   = FALSE;

/*
*********************************************************************************************************
                                    POLY=0x04C11DB7L [FSC]
*********************************************************************************************************
*/

static void _init_crc32_table_04c11db7( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned int crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
		crc = 0;
		c	= (( unsigned int ) i) << 24;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x80000000L )  crc = ( crc << 1 ) ^ CRC32_POLYNOMIAL_04C11DB7;
            else                            crc =   crc << 1;
			c = c << 1;
        }
        crc32_tab_shift_04c11db7[i] = crc;
    }
    crc32_tab_shift_04c11db7_init = TRUE;
}

static unsigned int _hz_update_crc32_04c11db7( unsigned int crc32, unsigned char c ) 
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FF & (unsigned int) c;

    if ( ! crc32_tab_shift_04c11db7_init ) _init_crc32_table_04c11db7();

	tmp = (crc >> 24) ^ int_c;
    crc = (crc << 8)  ^ crc32_tab_shift_04c11db7[ tmp & 0xFF ];

    return crc;
}


unsigned int hz_calc_crc32_04c11db7( const unsigned char *pSrc, unsigned int len, unsigned int crc32 ) 
{
    unsigned int i = 0;
    unsigned int crc = crc32;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc32_04c11db7(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xEDB88320L [CRC32 for file]
*********************************************************************************************************
*/
static void _init_crc32_table_edb88320( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = i;
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x00000001L ) crc = ( crc >> 1 ) ^ CRC32_POLYNOMIAL_EDB88320;
            else                     crc =   crc >> 1;
        }
        crc32_tab_shift_edb88320[i] = crc;
    }
    crc32_tab_shift_edb88320_init = TRUE;
}

static unsigned int _hz_update_crc32_edb88320( unsigned int crc32, unsigned char c ) 
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FFL & (unsigned int) c;
    if ( ! crc32_tab_shift_edb88320_init ) _init_crc32_table_edb88320();

    tmp = crc ^ int_c;
    crc = (crc >> 8) ^ crc32_tab_shift_edb88320[ tmp & 0xFF ];

    return crc;
}

unsigned int hz_calc_crc32_edb88320( const unsigned char *pSrc, unsigned int len, unsigned int crc32 ) 
{
    unsigned int i = 0;
    unsigned int crc = crc32;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc32_edb88320(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
*                                   POLY=0x4C11DB7 [MPEG2 ]
* Poly:    0x4C11DB7
* Init:    0xFFFFFFF
* Refin:   False
* Refout:  False
* Xorout:  0x00000000
*
*********************************************************************************************************
*/

static PyObject * _crc32_mpeg_2(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned int crc32    = 0xFFFFFFFFL;
    unsigned int result   = 0x00000000L;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|I", &data, &data_len, &crc32))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|I", &data, &data_len, &crc32))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc32_04c11db7(data, data_len, crc32);

    return Py_BuildValue("I", result);
}

/*
*********************************************************************************************************
*                                   POLY=0x4C11DB7 [CRC_32 ADCCP ]
* Poly:    0x4C11DB7
* Init:    0xFFFFFFF
* Refin:   True
* Refout:  True
* Xorout:  0xFFFFFFFF
* Use:     WinRAR,ect
*
*********************************************************************************************************
*/

static PyObject * _crc32_crc32(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned int crc32    = 0xFFFFFFFFL;
    unsigned int result   = 0x00000000L;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|I", &data, &data_len, &crc32))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|I", &data, &data_len, &crc32))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc32_edb88320(data, data_len, crc32);
    result = result ^ 0xFFFFFFFFL;

    return Py_BuildValue("I", result);
}


static PyObject * _crc32_table(PyObject *self, PyObject *args)
{
    unsigned int i = 0x00;
    unsigned int poly = CRC32_POLYNOMIAL_04C11DB7;
    PyObject* plist = PyList_New(MAX_TABLE_ARRAY);

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "I", &poly))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "I", &poly))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    switch ( poly ) {
        case CRC32_POLYNOMIAL_04C11DB7:
            if ( ! crc32_tab_shift_04c11db7_init ) _init_crc32_table_04c11db7();
            for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
                PyList_SetItem(plist, i, Py_BuildValue("I", crc32_tab_shift_04c11db7[i]));
            }
            break;

        case CRC32_POLYNOMIAL_EDB88320:
            if ( ! crc32_tab_shift_edb88320_init ) _init_crc32_table_edb88320();
            for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
                PyList_SetItem(plist, i, Py_BuildValue("I", crc32_tab_shift_edb88320[i]));
            }
            break;
        
        default:
            break;
    }

    return plist;
}

/* method table */
static PyMethodDef _crc32Methods[] = {
    {"mpeg2",   _crc32_mpeg_2, METH_VARARGS, "Calculate CRC (MPEG2) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]"},
    {"fsc",     _crc32_mpeg_2, METH_VARARGS, "Calculate CRC (Ethernt's FSC) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]"},
    {"crc32",   _crc32_crc32,  METH_VARARGS, "Calculate CRC (WinRAR, File) of CRC32  [Poly=0xEDB88320, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},

    {"table32", _crc32_table,  METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR(_crc32_doc,
"Calculation of CRC32 \n"
"libscrc.fsc   -> Calculate CRC for Ethernet frame sequence (FSC) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.mpeg2 -> Calculate CRC for Media file (MPEG2) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.crc32 -> Calculate CRC for file [Poly=0xEDB88320L, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]\n"
"\n");


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc32module = {
   PyModuleDef_HEAD_INIT,
   "_crc32",                    /* name of module */
   _crc32_doc,                  /* module documentation, may be NULL */
   -1,                          /* size of per-interpreter state of the module */
   _crc32Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc32(void)
{
    PyObject *m;

    m = PyModule_Create(&_crc32module);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddStringConstant(m, "__version__", "0.1.0");
    PyModule_AddStringConstant(m, "__author__", "Heyn");

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc32(void)
{
    (void) Py_InitModule3("_crc32", _crc32Methods, _crc32_doc);
}

#endif /* PY_MAJOR_VERSION */
