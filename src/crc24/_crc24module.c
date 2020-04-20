/*
 ********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
 *                                           All Rights Reserved
 * File    : _crc24module.c
 * Author  : Heyn (heyunhuan@gmail.com)
 * Version : V1.1
 *
 * LICENSING TERMS:
 * ---------------
 *		New Create at 	2020-04-17 [Heyn] Initialize.
 *
 ********************************************************************************************************
 */

#include <Python.h>
#include "_crc24tables.h"

static unsigned char hexin_PyArg_ParseTuple( PyObject *self, PyObject *args,
                                             unsigned int init,
                                             unsigned int (*function)( unsigned char *,
                                                                       unsigned int,
                                                                       unsigned int ),
                                             unsigned int *result )
{
    Py_buffer data = { NULL, NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*|I", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*|I", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#endif /* PY_MAJOR_VERSION */

    *result = function( (unsigned char *)data.buf, (unsigned int)data.len, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _crc24_ble( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00AAAAAA;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_00065b, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result );
}

static PyObject * _crc24_flexraya( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00FEDCBA;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_5d6dcb, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result );
}

static PyObject * _crc24_flexrayb( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00ABCDEF;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_5d6dcb, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result );
}

static PyObject * _crc24_openpgp( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00B704CE;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_864cfb, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result );
}

static PyObject * _crc24_lte_a( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00000000;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_864cfb, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result );
}

static PyObject * _crc24_lte_b( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00000000;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_800063, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result );
}

static PyObject * _crc24_os9( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00FFFFFF;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_800063, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result ^ 0xFFFFFF );
}

static PyObject * _crc24_interlaken( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000;
    unsigned int init   = 0x00FFFFFF;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc24_328b63, &result ) ) {
        return NULL;
    }
    return Py_BuildValue( "I", result ^ 0xFFFFFF );
}

/*
*********************************************************************************************************
*                                   For hacker
*********************************************************************************************************
*/

static PyObject * _crc24_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    Py_buffer data = { NULL, NULL };
    unsigned int init     = 0x00555555L;
    unsigned int xorout   = 0x00000000L;
    unsigned int ref      = 0x00000001L;
    unsigned int result   = 0x00000000L;
    unsigned int polynomial = CRC24_POLYNOMIAL_00065B;
    static char* kwlist[]={ "data", "poly", "init", "xorout", "ref", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y*|IIIp", kwlist, &data, &polynomial, &init, &xorout, &ref ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;        
    }
#else
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "s*|IIIp", kwlist, &data, &polynomial, &init, &xorout, &ref ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;  
    }
#endif /* PY_MAJOR_VERSION */

    if ( HEXIN_REFIN_OR_REFOUT_IS_TRUE( ref ) ) {
        polynomial = hexin_reverse24( polynomial );
    }

    result = hexin_calc_crc24_hacker( (unsigned char *)data.buf, (unsigned int)data.len, init, polynomial );
    result = result ^ xorout;

    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_BuildValue( "I", result );
}

/* method table */
static PyMethodDef _crc24Methods[] = {
    { "ble",         (PyCFunction)_crc24_ble,        METH_VARARGS,   "Calculate BLE of CRC24 [Poly=0x00065B, Init=0x555555, Xorout=0x00000000 Refin=True Refout=True]"},
    { "flexraya",    (PyCFunction)_crc24_flexraya,   METH_VARARGS,   "Calculate FLEXRAY-A of CRC24 [Poly=0x5D6DCB, Init=0xFEDCBA, Xorout=0x00000000 Refin=True Refout=True]"},
    { "flexrayb",    (PyCFunction)_crc24_flexrayb,   METH_VARARGS,   "Calculate FLEXRAY-B of CRC24 [Poly=0x5D6DCB, Init=0xABCDEF, Xorout=0x00000000 Refin=True Refout=True]"},
    { "openpgp",     (PyCFunction)_crc24_openpgp,    METH_VARARGS,   "Calculate OPENPGP of CRC24 [Poly=0x864CFB, Init=0xB704CE, Xorout=0x00000000 Refin=True Refout=True]"},
    { "lte_a",       (PyCFunction)_crc24_lte_a,      METH_VARARGS,   "Calculate LTE-A of CRC24 [Poly=0x864CFB, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]"},
    { "lte_b",       (PyCFunction)_crc24_lte_b,      METH_VARARGS,   "Calculate LTE-B of CRC24 [Poly=0x800063, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]"},
    { "os9",         (PyCFunction)_crc24_os9,        METH_VARARGS,   "Calculate OS-9 of CRC24 [Poly=0x800063, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]"},
    { "interlaken",  (PyCFunction)_crc24_interlaken, METH_VARARGS,   "Calculate INTERLAKEN of CRC24 [Poly=0x328B63, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]"},
    { "hacker24",    (PyCFunction)_crc24_hacker,     METH_KEYWORDS|METH_VARARGS, "User calculation CRC24\n"
                                                                                 "@data   : bytes\n"
                                                                                 "@poly   : default=0xEDB88320\n"
                                                                                 "@init   : default=0xFFFFFFFF\n"
                                                                                 "@xorout : default=0x00000000\n"
                                                                                 "@ref    : default=False" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};

/* module documentation */
PyDoc_STRVAR( _crc24_doc,
"Calculation of CRC24 \n"
"libscrc.ble        -> Calculate BLE of CRC24 [Poly=0x00065B, Init=0x555555, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.flexraya   -> Calculate FLEXRAY-A of CRC24 [Poly=0x5D6DCB, Init=0xFEDCBA, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.flexrayb   -> Calculate FLEXRAY-B of CRC24 [Poly=0x5D6DCB, Init=0xABCDEF, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.openpgp    -> Calculate OPENPGP of CRC24 [Poly=0x864CFB, Init=0xB704CE, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.lte_a      -> Calculate LTE-A of CRC24 [Poly=0x864CFB, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.lte_b      -> Calculate LTE-B of CRC24 [Poly=0x800063, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.os9        -> Calculate OS-9 of CRC24 [Poly=0x800063, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]\n"
"libscrc.interlaken -> Calculate INTERLAKEN of CRC24 [Poly=0x328B63, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc24module = {
   PyModuleDef_HEAD_INIT,
   "_crc24",                    /* name of module */
   _crc24_doc,                  /* module documentation, may be NULL */
   -1,                          /* size of per-interpreter state of the module */
   _crc24Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc24( void )
{
    PyObject *m;

    m = PyModule_Create( &_crc24module );
    if ( m == NULL ) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "1.1"  );
    PyModule_AddStringConstant( m, "__author__",  "Heyn" );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc24( void )
{
    ( void ) Py_InitModule3( "_crc24", _crc24Methods, _crc24_doc );
}

#endif /* PY_MAJOR_VERSION */
