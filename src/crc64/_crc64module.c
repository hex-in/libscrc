/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc64module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-19 13:17PM
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2020-03-16 [Heyn] New add hacker64 code.
*                       2020-04-17 [Heyn] Issues #1
*                       2020-04-23 [Heyn] New add we() and xz() functions.
*                       2020-05-12 [Heyn] (Python2.7) FIX : Windows compilation error.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2020-11-18 [Heyn] Fixed (Python2) Parsing arguments has no 'p' type
*                       2021-06-07 [Heyn] Add hacker64() reinit parameter. reinit=True -> Reinitialize the table
*                       2022-08-18 [Heyn] Fix Python deprication (PY_SSIZE_T_CLEAN)
*
*********************************************************************************************************
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "_crc64tables.h"

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_crc64 *param )
{
    Py_buffer data = { NULL, NULL };
    unsigned long long init = param->init;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*|K", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*|K", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#endif /* PY_MAJOR_VERSION */

    /* Fixed Issues #4  */
    param->is_gradual = ( unsigned int )PyTuple_Size( args );   // Fixed warning C4244
    if ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) {
        init = ( init ^ param->xorout );
    }

    param->result = hexin_crc64_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _crc64_iso( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_crc64 crc64_param_goiso = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC64_WIDTH,
                                                     .poly   = 0x000000000000001BL,
                                                     .init   = 0xFFFFFFFFFFFFFFFFL,
                                                     .refin  = TRUE,
                                                     .refout = TRUE,
                                                     .xorout = 0xFFFFFFFFFFFFFFFFL,
                                                     .result = 0 };
#else
    static struct _hexin_crc64 crc64_param_goiso = { FALSE,
                                                     HEXIN_CRC64_WIDTH,
                                                     0x000000000000001BL,
                                                     0xFFFFFFFFFFFFFFFFL,
                                                     TRUE,
                                                     TRUE,
                                                     0xFFFFFFFFFFFFFFFFL,
                                                     0 };
#endif /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc64_param_goiso ) ) {
        return NULL;
    }

    return Py_BuildValue( "K", crc64_param_goiso.result );
}

static PyObject * _crc64_ecma182( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_crc64 crc64_param_ecma = { .is_initial=FALSE,
                                                    .width  = HEXIN_CRC64_WIDTH,
                                                    .poly   = CRC64_POLYNOMIAL_ECMA182,
                                                    .init   = 0x0000000000000000L,
                                                    .refin  = FALSE,
                                                    .refout = FALSE,
                                                    .xorout = 0x0000000000000000L,
                                                    .result = 0 };
#else
    static struct _hexin_crc64 crc64_param_ecma = { FALSE,
                                                    HEXIN_CRC64_WIDTH,
                                                    CRC64_POLYNOMIAL_ECMA182,
                                                    0,
                                                    FALSE,
                                                    FALSE,
                                                    0,
                                                    0 };
#endif /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc64_param_ecma ) ) {
        return NULL;
    }

    return Py_BuildValue( "K", crc64_param_ecma.result );
}

static PyObject * _crc64_we( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_crc64 crc64_param_we = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC64_WIDTH,
                                                  .poly   = CRC64_POLYNOMIAL_ECMA182,
                                                  .init   = 0xFFFFFFFFFFFFFFFFL,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0xFFFFFFFFFFFFFFFFL,
                                                  .result = 0 };
#else
    static struct _hexin_crc64 crc64_param_we = { FALSE,
                                                  HEXIN_CRC64_WIDTH,
                                                  CRC64_POLYNOMIAL_ECMA182,
                                                  0xFFFFFFFFFFFFFFFFL,
                                                  FALSE,
                                                  FALSE,
                                                  0xFFFFFFFFFFFFFFFFL,
                                                  0 };
#endif /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc64_param_we ) ) {
        return NULL;
    }

    return Py_BuildValue( "K", crc64_param_we.result );
}

static PyObject * _crc64_xz( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_crc64 crc64_param_xz = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC64_WIDTH,
                                                  .poly   = CRC64_POLYNOMIAL_ECMA182,
                                                  .init   = 0xFFFFFFFFFFFFFFFFL,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0xFFFFFFFFFFFFFFFFL,
                                                  .result = 0 };
#else
    static struct _hexin_crc64 crc64_param_xz = { FALSE,
                                                  HEXIN_CRC64_WIDTH,
                                                  CRC64_POLYNOMIAL_ECMA182,
                                                  0xFFFFFFFFFFFFFFFFL,
                                                  TRUE,
                                                  TRUE,
                                                  0xFFFFFFFFFFFFFFFFL,
                                                  0 };
#endif /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc64_param_xz ) ) {
        return NULL;
    }

    return Py_BuildValue( "K", crc64_param_xz.result );
}

static PyObject * _crc64_gsm40( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_crc64 crc64_param_gsm = { .is_initial=FALSE,
                                                   .width  = 40,
                                                   .poly   = 0x0004820009,
                                                   .init   = 0x0000000000,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0xFFFFFFFFFFL,
                                                   .result = 0 };
#else
    static struct _hexin_crc64 crc64_param_gsm = { FALSE,
                                                   40,
                                                   0x0004820009,
                                                   0x0000000000,
                                                   FALSE,
                                                   FALSE,
                                                   0xFFFFFFFFFFL,
                                                   0 };
#endif /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc64_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "K", crc64_param_gsm.result );
}

static PyObject * _crc64_table( PyObject *self, PyObject *args )
{
    unsigned int i = 0;
    unsigned int ref = FALSE;
    unsigned long long poly = CRC64_POLYNOMIAL_ECMA182;
    unsigned long long table[MAX_TABLE_ARRAY] = { 0x0000000000000000L };
    PyObject* plist = PyList_New( MAX_TABLE_ARRAY );

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "K|p", &poly, &ref ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "K|p", &poly, &ref ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( FALSE == ref ) {
        hexin_crc64_init_table_poly_is_low ( poly, table );
    } else {
        hexin_crc64_init_table_poly_is_high( hexin_reverse64( poly ), table );
    }

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        PyList_SetItem( plist, i, Py_BuildValue( "K", table[i] ) );
    }

    return plist;
}

static PyObject * _crc64_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    unsigned int reinit = FALSE;
    Py_buffer data = { NULL, NULL };

    static struct _hexin_crc64 crc64_param_hacker = { .is_initial = FALSE,
                                                      .is_gradual = FALSE,
                                                      .width  = HEXIN_CRC64_WIDTH,
                                                      .poly   = CRC64_POLYNOMIAL_ECMA182,
                                                      .init   = 0x0000000000000000L,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x0000000000000000L,
                                                      .result = 0 };

    static char* kwlist[]={ "data", "poly", "init", "xorout", "refin", "refout", "reinit", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y*|KKKppp", kwlist, &data,
                                                                       &crc64_param_hacker.poly,
                                                                       &crc64_param_hacker.init,
                                                                       &crc64_param_hacker.xorout,
                                                                       &crc64_param_hacker.refin,
                                                                       &crc64_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL; 
    }
#else
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "s*|KKKIII", kwlist, &data,
                                                                       &crc64_param_hacker.poly,
                                                                       &crc64_param_hacker.init,
                                                                       &crc64_param_hacker.xorout,
                                                                       &crc64_param_hacker.refin,
                                                                       &crc64_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL; 
    }
#endif /* PY_MAJOR_VERSION */

    crc64_param_hacker.is_initial = ( reinit == FALSE ) ? crc64_param_hacker.is_initial : FALSE;
    crc64_param_hacker.result = hexin_crc64_compute( (const unsigned char *)data.buf, (unsigned int)data.len, &crc64_param_hacker, crc64_param_hacker.init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_BuildValue( "K", crc64_param_hacker.result );
}

/* method table */
static PyMethodDef _crc64Methods[] = {
    { "iso",         (PyCFunction)_crc64_iso,     METH_VARARGS, "Calculate GO-IOS of CRC64 [Poly=0x000000000000001BL, Init=0xFFFFFFFFFFFFFFFFL, refin=True,  refout=True,  xorout=0xFFFFFFFFFFFFFFFFL]" },
    { "ecma182",     (PyCFunction)_crc64_ecma182, METH_VARARGS, "Calculate ECMA182 of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0x0000000000000000, refin=False, refout=False, xorout=0x0000000000000000]" },
    { "we",          (PyCFunction)_crc64_we,      METH_VARARGS, "Calculate WE of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL, refin=False, refout=False, xorout=0xFFFFFFFFFFFFFFFFL]" },
    { "xz64",        (PyCFunction)_crc64_xz,      METH_VARARGS, "Calculate XZ of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL, refin=True, refout=True, xorout=0xFFFFFFFFFFFFFFFFL]" },
    { "gsm40",       (PyCFunction)_crc64_gsm40,   METH_VARARGS, "Calculate GSM of CRC40 [Poly=0x0004820009L, Init=0x0000000000, refin=False, refout=False, xorout=0xFFFFFFFFFF ]" },
    { "table64",     (PyCFunction)_crc64_table,   METH_VARARGS, "Print CRC64 table to list. libscrc.table64( polynomial )" },
    { "hacker64",    (PyCFunction)_crc64_hacker,  METH_KEYWORDS|METH_VARARGS, "User calculation CRC64\n"
                                                                              "@data   : bytes\n"
                                                                              "@poly   : default=0x42F0E1EBA9EA3693\n"
                                                                              "@init   : default=0x0000000000000000\n"
                                                                              "@xorout : default=0x0000000000000000\n"
                                                                              "@ref    : default=False" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _crc64_doc,
"Calculation of CRC64 \n"
"Author : Heyn \n"
"libscrc.iso      -> Calculate GO-IOS of CRC64 [Poly=0x000000000000001BL, Init=0xFFFFFFFFFFFFFFFFL, refin=True,  refout=True,  xorout=0xFFFFFFFFFFFFFFFFL]\n"
"libscrc.ecma182  -> Calculate ECMA182 of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0x0000000000000000, refin=False, refout=False, xorout=0x0000000000000000]\n"
"libscrc.we       -> Calculate WE of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL, refin=False, refout=False, xorout=0xFFFFFFFFFFFFFFFFL]\n"
"libscrc.xz64     -> Calculate XZ of CRC64 [Poly=0x42F0E1EBA9EA3693L, Init=0xFFFFFFFFFFFFFFFFL, refin=True, refout=True, xorout=0xFFFFFFFFFFFFFFFFL]\n"
"libscrc.gsm40    -> Calculate GSM of CRC40 [Poly=0x0004820009L, Init=0x0000000000, refin=False, refout=False, xorout=0xFFFFFFFFFF]\n"
"libscrc.hacker64 -> Free calculation CRC64 Xorout=0 Refin=False Refout=False\n"
"\n" );


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
PyInit__crc64( void )
{
    PyObject *m;

    m = PyModule_Create( &_crc64module );
    if ( m == NULL ) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "1.7" );
    PyModule_AddStringConstant( m, "__author__",  "Heyn");

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc64( void )
{
    (void) Py_InitModule3( "_crc64", _crc64Methods, _crc64_doc );
}

#endif /* PY_MAJOR_VERSION */
