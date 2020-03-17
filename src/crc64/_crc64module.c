/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2020, Hexin
*                                           All Rights Reserved
* File    : _crc64module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-19 13:17PM
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2020-03-16 [Heyn] New add hacker64 code.
*
*********************************************************************************************************
*/

#include <Python.h>
#include "_crc64tables.h"

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

    result = hexin_calc_crc64_iso(data, data_len, crc64);

    return Py_BuildValue("K", result);
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

    result = hexin_calc_crc64_ecma182( data, data_len, crc64 );

    return Py_BuildValue( "K", result );
}

static PyObject * _crc64_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned long long init   = 0xFFFFFFFFFFFFFFFFL;
    unsigned long long xorout = 0x0000000000000000L;
    unsigned int   ref        = 0x00000000L;
    unsigned long long result = 0x0000000000000000L;
    unsigned long long polynomial = CRC64_POLYNOMIAL_ECMA182;

    static char* kwlist[]={ "data", "poly", "init", "xorout", "ref", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y#|KKKp", kwlist, &data, &data_len, &polynomial, &init, &xorout, &ref ) )
        return NULL;
#else
    return NULL;
#endif /* PY_MAJOR_VERSION */
    if ( ref == 0x00000001L ) {
        polynomial = hexin_reverse64( polynomial );
    }
    result = hexin_calc_crc64_hacker( data, data_len, init, polynomial );
    result = result ^ xorout;
    return Py_BuildValue( "K", result );
}

/* method table */
static PyMethodDef _crc64Methods[] = {
    {"iso",         _crc64_iso,     METH_VARARGS, "Calculate CRC (IOS) of CRC64 [Poly=0xD800000000000000L, Init=0x0000000000000000L]"},
    {"ecma182",     _crc64_ecma182, METH_VARARGS, "Calculate CRC (ECMA182) of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL]"},
    {"hacker64",    _crc64_hacker,  METH_KEYWORDS|METH_VARARGS, "libscrc.hacker64(data=b'123456789', poly=0x42F0E1EBA9EA3693L, init=0xFFFFFFFFFFFFFFFF) ### Xorout=0 Refin=True Refout=True "},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR(_crc64_doc,
"Calculation of CRC64 \n"
"libscrc.iso -> Calculate CRC (IOS) of CRC64 [Poly=0xD800000000000000L, Init=0x0000000000000000L]\n"
"libscrc.ecma182  -> Calculate CRC (ECMA182) of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL]\n"
"libscrc.hacker64 -> Free calculation CRC64 (not support python2 series) Xorout=0 Refin=True Refout=True\n"
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

    PyModule_AddStringConstant(m, "__version__", "0.1.6");
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
