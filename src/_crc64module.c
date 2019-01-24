/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crc64module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.0.2
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-19 13:17PM
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*
*********************************************************************************************************
*/

#include <Python.h>

#define                 TRUE                    1
#define                 FALSE                   0

#define		            HZ64_POLYNOMIAL_ISO     0xD800000000000000L
#define		            HZ64_POLYNOMIAL_ECMA182 0x42F0E1EBA9EA3693L

static int				crc_tab64_iso_init		= FALSE;
static int				crc_tab64_ecma182_init	= FALSE;

unsigned long long	    crc_tab64_iso[256]      = {0x00000000};
unsigned long long      crc_tab64_ecma182[256]  = {0x00000000};


/*
*********************************************************************************************************
                                    POLY=0xD800000000000000L [ISO]
*********************************************************************************************************
*/

void init_crc64_iso_table( void ) 
{
    unsigned int i = 0, j = 0;
	unsigned long long crc;

    for ( i=0; i<256; i++ ) {
        crc = (unsigned long long) i;
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x0000000000000001L ) {
                crc = ( crc >> 1 ) ^ HZ64_POLYNOMIAL_ISO;
            } else {
                crc =   crc >> 1;
            }
        }
        crc_tab64_iso[i] = crc;
    }
    crc_tab64_iso_init = TRUE;
}

unsigned long long hz_update_crc64_iso( unsigned long long crc64, unsigned char c ) 
{
    unsigned long long crc = crc64;
    unsigned long long tmp, long_c;
    long_c = 0x00000000000000FFL & (unsigned long long) c;
    if ( ! crc_tab64_iso_init ) init_crc64_iso_table();

    tmp = crc ^ long_c;
    crc = (crc >> 8) ^ crc_tab64_iso[ tmp & 0xFF ];

    return crc;
}

/*
 * Width            = 64
 * Poly             = 0xD800000000000000L
 * InitValue(crc64) = 0x0000000000000000L
 */

unsigned long long hz_calc_crc64_iso( const unsigned char *pSrc, unsigned int len, unsigned long long crc64 ) 
{
    unsigned int i = 0;
    unsigned long long crc = crc64;

	for ( i=0; i<len; i++ ) {
		crc = hz_update_crc64_iso(crc, pSrc[i]);
    }

	return crc;
}

static PyObject * _crc64_iso(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned long long crc64    = 0x0000000000000000L;
    unsigned long long result   = 0x00000000L;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|K", &data, &data_len, &crc64))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|K", &data, &data_len, &crc64))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc64_iso(data, data_len, crc64);

    return Py_BuildValue("K", result);
}


/*
*********************************************************************************************************
                                    POLY=0x42F0E1EBA9EA3693L [ECMA182]
*********************************************************************************************************
*/
void init_crc64_ecma182_table( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned long long crc,c;

    for ( i=0; i<256; i++ ) {
		crc = 0;
		c	= ((unsigned long long) i) << 56;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x8000000000000000L ) {
                crc = ( crc << 1 ) ^ HZ64_POLYNOMIAL_ECMA182;
            } else {
                crc =   crc << 1;
            }
			c = c << 1;
        }
		crc_tab64_ecma182[i] = crc;
    }
	crc_tab64_ecma182_init = TRUE;
}

unsigned long long hz_update_crc64_ecma182( unsigned long long crc64, unsigned char c ) 
{
    unsigned long long crc = crc64;
    unsigned long long tmp, long_c;
    
    long_c = 0x00000000000000FFL & (unsigned long long) c;

    if ( ! crc_tab64_ecma182_init ) init_crc64_ecma182_table();

    tmp = (crc >> 56) ^ long_c;
    crc = (crc << 8)  ^ crc_tab64_ecma182[ tmp & 0xFF ];

    return crc;
}

/*
 * Width            = 64
 * Poly             = 0x42F0E1EBA9EA3693L
 * InitValue(crc64) = 0xFFFFFFFFFFFFFFFFL
 */

unsigned long long hz_calc_crc64_ecma182( const unsigned char *pSrc, unsigned int len, unsigned long long crc64 ) 
{
    unsigned int i = 0;
    unsigned long long crc = crc64;

	for ( i=0; i<len; i++ ) {
		crc = hz_update_crc64_ecma182(crc, pSrc[i]);
	}
	crc ^= 0xFFFFFFFFFFFFFFFF;
	return crc;
}

static PyObject * _crc64_ecma182(PyObject *self, PyObject *args)
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned long long crc64    = 0xFFFFFFFFFFFFFFFFL;
    unsigned long long result   = 0x00000000L;

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#|K", &data, &data_len, &crc64))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "s#|K", &data, &data_len, &crc64))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hz_calc_crc64_ecma182(data, data_len, crc64);

    return Py_BuildValue("K", result);
}


/* method table */
static PyMethodDef _crc64Methods[] = {
    {"iso",     _crc64_iso,    METH_VARARGS, "Calculate CRC (IOS) of CRC64 [Poly=0xD800000000000000L, Init=0x0000000000000000L]"},
    {"ecma182", _crc64_ecma182,METH_VARARGS, "Calculate CRC (ECMA182) of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL]"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR(_crc64_doc,
"Calculation of CRC64 \n"
"libscrc.iso -> Calculate CRC (IOS) of CRC64 [Poly=0xD800000000000000L, Init=0x0000000000000000L]\n"
"libscrc.ecma182 -> Calculate CRC (ECMA182) of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL]\n"
"\n");


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc64module = {
   PyModuleDef_HEAD_INIT,
   "_crc64",                    /* name of module */
   _crc64_doc,                  /* module documentation, may be NULL */
   -1,                          /* size of per-interpreter state of the module */
   _crc64Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc64(void)
{
    PyObject *m;

    m = PyModule_Create(&_crc64module);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddStringConstant(m, "__version__", "0.0.2");
    PyModule_AddStringConstant(m, "__author__", "Heyn");

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc64(void)
{
    (void) Py_InitModule3("_crc64", _crc64Methods, _crc64_doc);
}

#endif /* PY_MAJOR_VERSION */
