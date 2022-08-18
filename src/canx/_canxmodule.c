/*
*********************************************************************************************************
*                              		(c) Copyright 2020-2022, Hexin
*                                           All Rights Reserved
* File    : _canxmodule.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-21 [Heyn] Initialize.
*                       2020-04-27 [Heyn] Optimized Code.
*                       2020-05-12 [Heyn] (Python2.7) FIX : Windows compilation error.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2022-08-18 [Heyn] Fix Python deprication (PY_SSIZE_T_CLEAN)
*
*********************************************************************************************************
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "_canxtables.h"

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_canx *param )
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

    param->result = hexin_canx_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _canx_can15( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_canx canx_param_can15 = { .is_initial=FALSE,
                                                   .width  = 15,
                                                   .poly   = CAN15_POLYNOMIAL_00004599,
                                                   .init   = 0x00000000L,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00000000L,
                                                   .result = 0 };
#else
    static struct _hexin_canx canx_param_can15 = { FALSE,
                                                   15,
                                                   CAN15_POLYNOMIAL_00004599,
                                                   0x00000000L,
                                                   FALSE,
                                                   FALSE,
                                                   0x00000000L,
                                                   0 };
#endif  /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &canx_param_can15 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", (unsigned short)( canx_param_can15.result & 0x00007FFF ) );
}

static PyObject * _canx_can17( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_canx canx_param_can17 = { .is_initial=FALSE,
                                                   .width  = 17,
                                                   .poly   = CAN17_POLYNOMIAL_0001685B,
                                                   .init   = 0x00000000L,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00000000L,
                                                   .result = 0 };
#else
    static struct _hexin_canx canx_param_can17 = { FALSE,
                                                   17,
                                                   CAN17_POLYNOMIAL_0001685B,
                                                   0x00000000L,
                                                   FALSE,
                                                   FALSE,
                                                   0x00000000L,
                                                   0 };
#endif /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &canx_param_can17 ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", (canx_param_can17.result & 0x1FFFF) );
}

static PyObject * _canx_can21( PyObject *self, PyObject *args )
{
#if PY_MAJOR_VERSION >= 3
    static struct _hexin_canx canx_param_can21= { .is_initial=FALSE,
                                                   .width  = 21,
                                                   .poly   = CAN21_POLYNOMIAL_00102899,
                                                   .init   = 0x00000000L,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00000000L,
                                                   .result = 0 };
#else
    static struct _hexin_canx canx_param_can21 = { FALSE,
                                                   21,
                                                   CAN21_POLYNOMIAL_00102899,
                                                   0x00000000L,
                                                   FALSE,
                                                   FALSE,
                                                   0x00000000L,
                                                   0 };
#endif  /* PY_MAJOR_VERSION */

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &canx_param_can21 ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", (canx_param_can21.result & 0x001FFFFF) );
}

/* method table */
static PyMethodDef _canxMethods[] = {
    { "can15",      (PyCFunction)_canx_can15, METH_VARARGS, "Calculate CAN15 [Poly=0x004599, Init=0 Xorout=0 Refin=FALSE Refout=FALSE]"   },
    { "can17",      (PyCFunction)_canx_can17, METH_VARARGS, "Calculate CAN17 [Poly=0x01685B, Init=0 Xorout=0 Refin=FALSE Refout=FALSE]"  },
    { "can21",      (PyCFunction)_canx_can21, METH_VARARGS, "Calculate CAN21 [Poly=0x102899, Init=0 Xorout=0 Refin=FALSE Refout=FALSE]" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _canx_doc,
"Calculation of CANx \n"
"Author : Heyn \n"
"libscrc.can15 -> Calculate CAN15 [Poly = 0x004599 Initial = 0 Xorout=0 Refin=FALSE Refout=FALSE]\n"
"libscrc.can17 -> Calculate CAN17 [Poly = 0x01685B Initial = 0 Xorout=0 Refin=FALSE Refout=FALSE]\n"
"libscrc.can21 -> Calculate CAN21 [Poly = 0x102899 Initial = 0 Xorout=0 Refin=FALSE Refout=FALSE]\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _canxmodule = {
   PyModuleDef_HEAD_INIT,
   "_canx",                    /* name of module */
   _canx_doc,                  /* module documentation, may be NULL */
   -1,                         /* size of per-interpreter state of the module */
   _canxMethods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__canx( void )
{
    PyObject *m = NULL;

    m = PyModule_Create( &_canxmodule );
    if ( m == NULL ) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "1.7"   );
    PyModule_AddStringConstant( m, "__author__",  "Heyn"  );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_canx( void )
{
    (void) Py_InitModule3( "_canx", _canxMethods, _canx_doc );
}

#endif /* PY_MAJOR_VERSION */
