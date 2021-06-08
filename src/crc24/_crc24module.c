/*
 ********************************************************************************************************
*                              		(c) Copyright 2020-2021, Hexin
*                                           All Rights Reserved
* File    : _crc24module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-17 [Heyn] Initialize.
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2020-11-17 [Heyn] Fixed Issues #6 (Python2 vc9 error C2059 )
*                       2021-06-07 [Heyn] Add hacker24() reinit parameter. reinit=True -> Reinitialize the table
*
********************************************************************************************************
*/

#include <Python.h>
#include "_crc24tables.h"

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_crc24 *param )
{
    Py_buffer data = { NULL, NULL };
    unsigned int init = param->init;

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

    /* Fixed Issues #4  */
    param->is_gradual = ( unsigned int )PyTuple_Size( args );   // Fixed warning C4244
    if ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) {
        init = ( init ^ param->xorout );
    }

    param->result = hexin_crc24_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _crc24_ble( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_ble;

    crc24_param_ble.is_initial = FALSE;
    crc24_param_ble.width      = HEXIN_CRC24_WIDTH;
    crc24_param_ble.poly       = CRC24_POLYNOMIAL_00065B;
    crc24_param_ble.init       = 0x00555555;
    crc24_param_ble.refin      = TRUE;
    crc24_param_ble.refout     = TRUE;
    crc24_param_ble.xorout     = 0x00000000;
    crc24_param_ble.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_ble ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_ble.result );
}

static PyObject * _crc24_flexraya( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_flexraya;

    crc24_param_flexraya.is_initial = FALSE;
    crc24_param_flexraya.width      = HEXIN_CRC24_WIDTH;
    crc24_param_flexraya.poly       = CRC24_POLYNOMIAL_5D6DCB;
    crc24_param_flexraya.init       = 0x00FEDCBA;
    crc24_param_flexraya.refin      = FALSE;
    crc24_param_flexraya.refout     = FALSE;
    crc24_param_flexraya.xorout     = 0x00000000;
    crc24_param_flexraya.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_flexraya ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_flexraya.result );
}

static PyObject * _crc24_flexrayb( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_flexrayb;

    crc24_param_flexrayb.is_initial = FALSE;
    crc24_param_flexrayb.width      = HEXIN_CRC24_WIDTH;
    crc24_param_flexrayb.poly       = CRC24_POLYNOMIAL_5D6DCB;
    crc24_param_flexrayb.init       = 0x00ABCDEF;
    crc24_param_flexrayb.refin      = FALSE;
    crc24_param_flexrayb.refout     = FALSE;
    crc24_param_flexrayb.xorout     = 0x00000000;
    crc24_param_flexrayb.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_flexrayb ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_flexrayb.result );
}

static PyObject * _crc24_openpgp( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_openpgp;

    crc24_param_openpgp.is_initial = FALSE;
    crc24_param_openpgp.width      = HEXIN_CRC24_WIDTH;
    crc24_param_openpgp.poly       = CRC24_POLYNOMIAL_864CFB;
    crc24_param_openpgp.init       = 0x00B704CE;
    crc24_param_openpgp.refin      = FALSE;
    crc24_param_openpgp.refout     = FALSE;
    crc24_param_openpgp.xorout     = 0x00000000;
    crc24_param_openpgp.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_openpgp ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_openpgp.result );
}

static PyObject * _crc24_lte_a( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_lte_a;

    crc24_param_lte_a.is_initial = FALSE;
    crc24_param_lte_a.width      = HEXIN_CRC24_WIDTH;
    crc24_param_lte_a.poly       = CRC24_POLYNOMIAL_864CFB;
    crc24_param_lte_a.init       = 0x00000000;
    crc24_param_lte_a.refin      = FALSE;
    crc24_param_lte_a.refout     = FALSE;
    crc24_param_lte_a.xorout     = 0x00000000;
    crc24_param_lte_a.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_lte_a ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_lte_a.result );
}

static PyObject * _crc24_lte_b( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_lte_b;

    crc24_param_lte_b.is_initial = FALSE;
    crc24_param_lte_b.width      = HEXIN_CRC24_WIDTH;
    crc24_param_lte_b.poly       = CRC24_POLYNOMIAL_800063;
    crc24_param_lte_b.init       = 0x00000000;
    crc24_param_lte_b.refin      = FALSE;
    crc24_param_lte_b.refout     = FALSE;
    crc24_param_lte_b.xorout     = 0x00000000;
    crc24_param_lte_b.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_lte_b ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_lte_b.result );
}

static PyObject * _crc24_os9( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_os9;

    crc24_param_os9.is_initial = FALSE;
    crc24_param_os9.width      = HEXIN_CRC24_WIDTH;
    crc24_param_os9.poly       = CRC24_POLYNOMIAL_800063;
    crc24_param_os9.init       = 0x00FFFFFF;
    crc24_param_os9.refin      = FALSE;
    crc24_param_os9.refout     = FALSE;
    crc24_param_os9.xorout     = 0x00FFFFFF;
    crc24_param_os9.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_os9 ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_os9.result );
}

static PyObject * _crc24_interlaken( PyObject *self, PyObject *args )
{
    static struct _hexin_crc24 crc24_param_interlaken;

    crc24_param_interlaken.is_initial = FALSE;
    crc24_param_interlaken.width      = HEXIN_CRC24_WIDTH;
    crc24_param_interlaken.poly       = CRC24_POLYNOMIAL_328B63;
    crc24_param_interlaken.init       = 0x00FFFFFF;
    crc24_param_interlaken.refin      = FALSE;
    crc24_param_interlaken.refout     = FALSE;
    crc24_param_interlaken.xorout     = 0x00FFFFFF;
    crc24_param_interlaken.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc24_param_interlaken ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", crc24_param_interlaken.result );
}

static PyObject * _crc24_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    unsigned int reinit = FALSE;
    Py_buffer data = { NULL, NULL };
    struct _hexin_crc24 crc24_param_hacker;
    static char* kwlist[]={ "data", "poly", "init", "xorout", "refin", "refout", "reinit", NULL };

    crc24_param_hacker.is_initial = FALSE;
    crc24_param_hacker.is_gradual = FALSE;
    crc24_param_hacker.width      = HEXIN_CRC24_WIDTH;
    crc24_param_hacker.poly       = CRC24_POLYNOMIAL_800063;
    crc24_param_hacker.init       = 0x00FFFFFF;
    crc24_param_hacker.refin      = FALSE;
    crc24_param_hacker.refout     = FALSE;
    crc24_param_hacker.xorout     = 0x00FFFFFF;
    crc24_param_hacker.result     = 0;


#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y*|IIIppp", kwlist, &data,
                                                                       &crc24_param_hacker.poly,
                                                                       &crc24_param_hacker.init,
                                                                       &crc24_param_hacker.xorout,
                                                                       &crc24_param_hacker.refin,
                                                                       &crc24_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;        
    }
#else
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "s*|IIIIII", kwlist, &data,
                                                                       &crc24_param_hacker.poly,
                                                                       &crc24_param_hacker.init,
                                                                       &crc24_param_hacker.xorout,
                                                                       &crc24_param_hacker.refin,
                                                                       &crc24_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;  
    }
#endif /* PY_MAJOR_VERSION */

    crc24_param_hacker.is_initial = ( reinit == FALSE ) ? crc24_param_hacker.is_initial : FALSE;
    crc24_param_hacker.result = hexin_crc24_compute( (const unsigned char *)data.buf, (unsigned int)data.len, &crc24_param_hacker, crc24_param_hacker.init );
    
    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_BuildValue( "I", crc24_param_hacker.result );
}

/* method table */
static PyMethodDef _crc24Methods[] = {
    { "ble",         (PyCFunction)_crc24_ble,        METH_VARARGS,   "Calculate BLE of CRC24 [Poly=0x00065B, Init=0x555555, Xorout=0x00000000 Refin=True Refout=True]"},
    { "flexraya",    (PyCFunction)_crc24_flexraya,   METH_VARARGS,   "Calculate FLEXRAY-A of CRC24 [Poly=0x5D6DCB, Init=0xFEDCBA, Xorout=0x00000000 Refin=True Refout=True]"},
    { "flexrayb",    (PyCFunction)_crc24_flexrayb,   METH_VARARGS,   "Calculate FLEXRAY-B of CRC24 [Poly=0x5D6DCB, Init=0xABCDEF, Xorout=0x00000000 Refin=True Refout=True]"},
    { "openpgp",     (PyCFunction)_crc24_openpgp,    METH_VARARGS,   "Calculate OPENPGP of CRC24 [Poly=0x864CFB, Init=0xB704CE, Xorout=0x00000000 Refin=True Refout=True]"},
    { "crc24",       (PyCFunction)_crc24_openpgp,    METH_VARARGS,   "Calculate CRC of CRC24 [Poly=0x864CFB, Init=0xB704CE, Xorout=0x00000000 Refin=True Refout=True]"},
    { "lte_a",       (PyCFunction)_crc24_lte_a,      METH_VARARGS,   "Calculate LTE-A of CRC24 [Poly=0x864CFB, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]"},
    { "lte_b",       (PyCFunction)_crc24_lte_b,      METH_VARARGS,   "Calculate LTE-B of CRC24 [Poly=0x800063, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]"},
    { "os9",         (PyCFunction)_crc24_os9,        METH_VARARGS,   "Calculate OS-9 of CRC24 [Poly=0x800063, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]"},
    { "interlaken",  (PyCFunction)_crc24_interlaken, METH_VARARGS,   "Calculate INTERLAKEN of CRC24 [Poly=0x328B63, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]"},
    { "hacker24",    (PyCFunction)_crc24_hacker,     METH_KEYWORDS|METH_VARARGS, "User calculation CRC24\n"
                                                                                 "@poly   : default=0x00800063\n"
                                                                                 "@init   : default=0x00FFFFFF\n"
                                                                                 "@xorout : default=0x00FFFFFF\n"
                                                                                 "@refin  : default=False\n"
                                                                                 "@refout : default=False\n"
                                                                                 "@reinit : default=False" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};

/* module documentation */
PyDoc_STRVAR( _crc24_doc,
"Calculation of CRC24 \n"
"libscrc.ble        -> Calculate BLE of CRC24 [Poly=0x00065B, Init=0x555555, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.flexraya   -> Calculate FLEXRAY-A of CRC24 [Poly=0x5D6DCB, Init=0xFEDCBA, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.flexrayb   -> Calculate FLEXRAY-B of CRC24 [Poly=0x5D6DCB, Init=0xABCDEF, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.openpgp    -> Calculate OPENPGP of CRC24 [Poly=0x864CFB, Init=0xB704CE, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.crc24      -> Calculate CRC of CRC24 [Poly=0x864CFB, Init=0xB704CE, Xorout=0x00000000 Refin=True Refout=True]\n"
"libscrc.lte_a      -> Calculate LTE-A of CRC24 [Poly=0x864CFB, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.lte_b      -> Calculate LTE-B of CRC24 [Poly=0x800063, Init=0x000000, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.os9        -> Calculate OS-9 of CRC24 [Poly=0x800063, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]\n"
"libscrc.interlaken -> Calculate INTERLAKEN of CRC24 [Poly=0x328B63, Init=0xFFFFFF, Xorout=0xFFFFFF Refin=False Refout=False]\n"
"libscrc.hacker24   -> Free calculation CRC24 @reinit reinitialize the crc24 tables\n"
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

    PyModule_AddStringConstant( m, "__version__", "1.7"  );
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
