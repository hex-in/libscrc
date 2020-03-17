/*
 ********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
 *                                           All Rights Reserved
 * File    : _crc32module.c
 * Author  : Heyn (heyunhuan@gmail.com)
 * Version : V0.1.6
 *
 * LICENSING TERMS:
 * ---------------
 *		New Create at 	2017-08-09 16:39PM
 *                      2017-08-17 [Heyn] Optimized Code.
 *                      2017-08-21 [Heyn] Optimization code for the C99 standard.
 *                                        for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
 *                      2017-09-22 [Heyn] Optimized Code.
 *                                        New get crc32 table.
 *                      2019-04-18 [Heyn] New creaker.
 *                      2019-04-28 [Heyn] New add lclear32 \ rclear32 \ pltable32 \ prtable32 functions.
 *                      2020-03-16 [Heyn] New add hacker32 code.
 *                                        Removed lclear32 \ rclear32 \ pltable32 \ prtable32 functions.
 * 
 * Web : https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
 *
 ********************************************************************************************************
 */

#include <Python.h>

#include "_crc32tables.h"

/*
 ********************************************************************************************************
 *                                   POLY=0x4C11DB7 [MPEG2 ]
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00000000
 *
 ********************************************************************************************************
 */

static PyObject * _crc32_mpeg_2( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned int crc32    = 0xFFFFFFFFL;
    unsigned int result   = 0x00000000L;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|I", &data, &data_len, &crc32 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|I", &data, &data_len, &crc32 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc32_04c11db7( data, data_len, crc32 );
    return Py_BuildValue( "I", result );
}

/*
 ********************************************************************************************************
 *                                   POLY=0x4C11DB7 [CRC_32 ADCCP ]
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFFFFFF
 * Use:     WinRAR,ect
 *
 ********************************************************************************************************
 */

static PyObject * _crc32_crc32( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned int crc32    = 0xFFFFFFFFL;
    unsigned int result   = 0x00000000L;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|I", &data, &data_len, &crc32 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|I", &data, &data_len, &crc32 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc32_edb88320( data, data_len, crc32 );
    result = result ^ 0xFFFFFFFFL;

    return Py_BuildValue( "I", result );
}

/*
*********************************************************************************************************
                                    Print CRC32 table.
*********************************************************************************************************
*/
static PyObject * _crc32_table( PyObject *self, PyObject *args )
{
    unsigned int i = 0x00000000L;
    unsigned int poly = CRC32_POLYNOMIAL_04C11DB7;
    PyObject* plist = PyList_New( MAX_TABLE_ARRAY );

#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "I", &poly))
        return NULL;
#else
    if (!PyArg_ParseTuple(args, "I", &poly))
        return NULL;
#endif /* PY_MAJOR_VERSION */

    // switch ( poly ) {
    //     case CRC32_POLYNOMIAL_04C11DB7:
    //         if ( ! crc32_tab_shift_04c11db7_init ) _init_crc32_table_04c11db7();
    //         for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
    //             PyList_SetItem(plist, i, Py_BuildValue("I", crc32_tab_shift_04c11db7[i]));
    //         }
    //         break;

    //     case CRC32_POLYNOMIAL_EDB88320:
    //         if ( ! crc32_tab_shift_edb88320_init ) _init_crc32_table_edb88320();
    //         for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
    //             PyList_SetItem(plist, i, Py_BuildValue("I", crc32_tab_shift_edb88320[i]));
    //         }
    //         break;
        
    //     default:
    //         _init_crc32_table_hacker( poly );
    //         for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
    //             PyList_SetItem( plist, i, Py_BuildValue( "I", crc32_tab_shift_xxxxxxxx[i] ) );
    //         }
    //         break;
    // }

    return plist;
}

/*
*********************************************************************************************************
*                                   For hacker
*********************************************************************************************************
*/

static PyObject * _crc32_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned int init     = 0xFFFFFFFFL;
    unsigned int xorout   = 0x00000000L;
    unsigned int ref      = 0x00000000L;
    unsigned int result   = 0x00000000L;
    unsigned int polynomial = CRC32_POLYNOMIAL_EDB88320;
    static char* kwlist[]={ "data", "poly", "init", "xorout", "ref", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y#|IIIp", kwlist, &data, &data_len, &polynomial, &init, &xorout, &ref ) )
        return NULL;
#else
    return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( HEXIN_REFIN_OR_REFOUT_IS_TRUE( ref ) ) {
        polynomial = hexin_reverse32( polynomial );
    }

    result = hexin_calc_crc32_hacker( data, data_len, init, polynomial );
    result = result ^ xorout;
    return Py_BuildValue( "I", result );
}

/* method table */
static PyMethodDef _crc32Methods[] = {
    { "mpeg2",       _crc32_mpeg_2,     METH_VARARGS,   "Calculate CRC (MPEG2) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]"},
    { "fsc",         _crc32_mpeg_2,     METH_VARARGS,   "Calculate CRC (Ethernt's FSC) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]"},
    { "crc32",       _crc32_crc32,      METH_VARARGS,   "Calculate CRC (WinRAR, File) of CRC32  [Poly=0xEDB88320, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "table32",     _crc32_table,      METH_VARARGS,   "Print CRC32 table to list. libscrc.table32( polynomial )" },
    { "hacker32",    _crc32_hacker,     METH_KEYWORDS|METH_VARARGS, "libscrc.hacker32(data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF) ### Xorout=0x00000000 Refin=True Refout=True "},
    { NULL, NULL, 0, NULL }        /* Sentinel */
};

/* module documentation */
PyDoc_STRVAR( _crc32_doc,
"Calculation of CRC32 \n"
"libscrc.fsc      -> Calculate CRC for Ethernet frame sequence (FSC) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.mpeg2    -> Calculate CRC for Media file (MPEG2) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.crc32    -> Calculate CRC for file [Poly=0xEDB88320L, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]\n"
"libscrc.table32  -> Print CRC32 table to list\n"
"libscrc.hacker32 -> Free calculation CRC32 (not support python2 series) Xorout=0x00000000 Refin=True Refout=True\n"
"\n" );


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
PyInit__crc32( void )
{
    PyObject *m;

    m = PyModule_Create( &_crc32module );
    if ( m == NULL ) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "0.1.6" );
    PyModule_AddStringConstant( m, "__author__",  "Heyn"  );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc32( void )
{
    ( void ) Py_InitModule3( "_crc32", _crc32Methods, _crc32_doc );
}

#endif /* PY_MAJOR_VERSION */
