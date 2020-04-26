/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _canxmodule.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-21 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#include <Python.h>
#include "_canxtables.h"

static unsigned char hexin_PyArg_ParseTuple( PyObject *self, PyObject *args,
                                             unsigned int init,
                                             unsigned int (*function)( const unsigned char *,
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

    *result = (* function)( (unsigned char *)data.buf, (unsigned int)data.len, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _canx_can15( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0x00000000L;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_can15, ( unsigned int * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", (unsigned short)(result & 0x00007FFF) );
}

static PyObject * _canx_can17( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0x00000000L;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_can17, ( unsigned int * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", (result & 0x0001FFFF) );
}

static PyObject * _canx_can21( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0x00000000L;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_can21, ( unsigned int * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", (result & 0x001FFFFF) );
}

/* method table */
static PyMethodDef _canxMethods[] = {
    { "can15",      (PyCFunction)_canx_can15, METH_VARARGS, "Calculate CAN15 [Poly=0x4599, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "can17",      (PyCFunction)_canx_can17, METH_VARARGS, "Calculate CAN17 [Poly=0x1685B, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "can21",      (PyCFunction)_canx_can21, METH_VARARGS, "Calculate CAN21 [Poly=0x102899, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _canx_doc,
"Calculation of CANx \n"
"libscrc.can15   -> Calculate CAN15 [Poly = 0x4599 Initial = 0 Xorout=0 Refin=True Refout=True]\n"
"libscrc.can17   -> Calculate CAN17 [Poly = 0x1685B Initial = 0 Xorout=0 Refin=True Refout=True]\n"
"libscrc.can21   -> Calculate CAN21 [Poly = 0x102899 Initial = 0 Xorout=0 Refin=True Refout=True]\n"
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

    PyModule_AddStringConstant( m, "__version__", "1.1"   );
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
