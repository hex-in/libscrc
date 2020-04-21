/*
 ********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
 *                                           All Rights Reserved
 * File    : _crc32module.c
 * Author  : Heyn (heyunhuan@gmail.com)
 * Version : V1.1
 *
 * LICENSING TERMS:
 * ---------------
 *		New Create at 	2017-08-09 16:39PM
 *                      2017-08-17 [Heyn] Optimized Code.
 *                      2017-08-21 [Heyn] Optimization code for the C99 standard.
 *                                        for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
 *                      2017-09-22 [Heyn] Optimized Code. New add table32() function.
 *                      2020-03-20 [Heyn] New add adler32 and fletcher32 functions.
 *                      2020-04-17 [Heyn] Issues #1
 * 
 * Web : https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
 *
 ********************************************************************************************************
 */

#include <Python.h>
#include "_crc32tables.h"

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


static unsigned char hexin_PyArg_ParseTuple_Parametes( PyObject *self, PyObject *args,
                                                       unsigned int  init,
                                                       unsigned int  polynomial,
                                                       unsigned char mask,
                                                       unsigned int (*function)( unsigned char *,
                                                                                 unsigned int,
                                                                                 unsigned int,
                                                                                 unsigned int,
                                                                                 unsigned char ),
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

    *result = function( (unsigned char *)data.buf, (unsigned int)data.len, init, polynomial, mask  );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

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
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc32_04c11db7, &result ) ) {
        return NULL;
    }

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
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc32_edb88320, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result ^ 0xFFFFFFFFL );
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
    unsigned int table[MAX_TABLE_ARRAY] = { 0x00000000L };
    PyObject* plist = PyList_New( MAX_TABLE_ARRAY );

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "I", &poly ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "I", &poly ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( HEXIN_POLYNOMIAL_IS_HIGH( poly ) ) {
        hexin_crc32_init_table_poly_is_high( poly, table );
    } else {
        hexin_crc32_init_table_poly_is_low ( poly, table );
    }

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        PyList_SetItem( plist, i, Py_BuildValue( "I", table[i] ) );
    }

    return plist;
}

/*
*********************************************************************************************************
*                                   For hacker
*********************************************************************************************************
*/

static PyObject * _crc32_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    Py_buffer data = { NULL, NULL };
    unsigned int init     = 0xFFFFFFFFL;
    unsigned int xorout   = 0x00000000L;
    unsigned int ref      = 0x00000000L;
    unsigned int result   = 0x00000000L;
    unsigned int polynomial = CRC32_POLYNOMIAL_EDB88320;
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
        polynomial = hexin_reverse32( polynomial );
    }

    result = hexin_calc_crc32_hacker( (unsigned char *)data.buf, (unsigned int)data.len, init, polynomial );
    result = result ^ xorout;

    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_BuildValue( "I", result );
}


static PyObject * _crc32_adler32( PyObject *self, PyObject *args )
{
    unsigned int result   = 0x00000000L;
    unsigned int reserved = 0x00000000L;
 
    if ( !hexin_PyArg_ParseTuple( self, args, reserved, hexin_calc_crc32_adler, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result );
}

/*
*********************************************************************************************************
                                    For fletcher32 checksum
*********************************************************************************************************
*/

static PyObject * _crc32_fletcher32( PyObject *self, PyObject *args )
{
    unsigned int result   = 0x00000000L;
    unsigned int reserved = 0x00000000L;
 
    if ( !hexin_PyArg_ParseTuple( self, args, reserved, hexin_calc_crc32_fletcher, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result );
}

static PyObject * _crc32_posix( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0x00000000L;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc32_04c11db7, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result ^ 0xFFFFFFFFL );
}

static PyObject * _crc32_bzip2( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc32_04c11db7, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result ^ 0xFFFFFFFFL );
}

static PyObject * _crc32_jamcrc( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
    unsigned int poly   = hexin_reverse32( 0x04C11DB7L );
    unsigned char mask  = TRUE;
 
    if ( !hexin_PyArg_ParseTuple_Parametes( self, args, init, poly, mask, hexin_calc_crc32_shared, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result );
}

static PyObject * _crc32_autosar( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
    unsigned int poly   = hexin_reverse32( 0xF4ACFB13L );
    unsigned char mask  = TRUE;
 
    if ( !hexin_PyArg_ParseTuple_Parametes( self, args, init, poly, mask, hexin_calc_crc32_shared, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result ^ 0xFFFFFFFFL );
}

static PyObject * _crc32_crc32_c( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
    unsigned int poly   = hexin_reverse32( 0x1EDC6F41L );
    unsigned char mask  = TRUE;
 
    if ( !hexin_PyArg_ParseTuple_Parametes( self, args, init, poly, mask, hexin_calc_crc32_shared, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result ^ 0xFFFFFFFFL );
}

static PyObject * _crc32_crc32_d( PyObject *self, PyObject *args )   // TODO
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0xFFFFFFFFL;
    unsigned int poly   = hexin_reverse32( 0xA833982BL );
    unsigned char mask  = TRUE;
 
    if ( !hexin_PyArg_ParseTuple_Parametes( self, args, init, poly, mask, hexin_calc_crc32_shared, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result ^ 0xFFFFFFFFL );
}

static PyObject * _crc32_crc32_q( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0x00000000L;
    unsigned int poly   = 0x814141ABL;
    unsigned char mask  = FALSE;
 
    if ( !hexin_PyArg_ParseTuple_Parametes( self, args, init, poly, mask, hexin_calc_crc32_shared, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result );
}

static PyObject * _crc32_xfer( PyObject *self, PyObject *args )
{
    unsigned int result = 0x00000000L;
    unsigned int init   = 0x00000000L;
    unsigned int poly   = 0x000000AFL;
    unsigned char mask  = FALSE;
 
    if ( !hexin_PyArg_ParseTuple_Parametes( self, args, init, poly, mask, hexin_calc_crc32_shared, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "I", result );
}

/* method table */
static PyMethodDef _crc32Methods[] = {
    { "mpeg2",       (PyCFunction)_crc32_mpeg_2,     METH_VARARGS,   "Calculate CRC (MPEG2) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]"},
    { "fsc",         (PyCFunction)_crc32_mpeg_2,     METH_VARARGS,   "Calculate CRC (Ethernt's FSC) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]"},
    { "crc32",       (PyCFunction)_crc32_crc32,      METH_VARARGS,   "Calculate CRC (WinRAR, File) of CRC32  [Poly=0xEDB88320, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "table32",     (PyCFunction)_crc32_table,      METH_VARARGS,   "Print CRC32 table to list. libscrc.table32( polynomial )" },
    { "hacker32",    (PyCFunction)_crc32_hacker,     METH_KEYWORDS|METH_VARARGS, "User calculation CRC32\n"
                                                                                 "@data   : bytes\n"
                                                                                 "@poly   : default=0xEDB88320\n"
                                                                                 "@init   : default=0xFFFFFFFF\n"
                                                                                 "@xorout : default=0x00000000\n"
                                                                                 "@ref    : default=False" },
    { "adler32",    (PyCFunction)_crc32_adler32,     METH_VARARGS,   "Calculate adler32 (MOD=65521)" },
    { "fletcher32", (PyCFunction)_crc32_fletcher32,  METH_VARARGS,   "Calculate fletcher32" },
    { "posix",      (PyCFunction)_crc32_posix,       METH_VARARGS,   "Calculate CRC (POSIX) of CRC32 [Poly=0x04C11DB7, Init=0x00000000, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "bzip2",      (PyCFunction)_crc32_bzip2,       METH_VARARGS,   "Calculate CRC (BZIP2) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "jamcrc",     (PyCFunction)_crc32_jamcrc,      METH_VARARGS,   "Calculate CRC (JAMCRC) of CRC32 [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=True Refout=True]"},
    { "autosar",    (PyCFunction)_crc32_autosar,     METH_VARARGS,   "Calculate CRC (AUTOSAR) of CRC32 [Poly=0xF4ACFB13, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "crc32_c",    (PyCFunction)_crc32_crc32_c,     METH_VARARGS,   "Calculate CRC (CRC32_C) of CRC32 [Poly=0x1EDC6F41, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "crc32_d",    (PyCFunction)_crc32_crc32_d,     METH_VARARGS,   "Calculate CRC (CRC32_D) of CRC32 [Poly=0xA833982B, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]"},
    { "crc32_q",    (PyCFunction)_crc32_crc32_q,     METH_VARARGS,   "Calculate CRC (CRC32_Q) of CRC32 [Poly=0x814141AB, Init=0x00000000, Xorout=0x00000000 Refin=True Refout=True]"},
    { "xfer",       (PyCFunction)_crc32_xfer,        METH_VARARGS,   "Calculate CRC (XFER) of CRC32 [Poly=0x000000AF, Init=0x00000000, Xorout=0x00000000 Refin=True Refout=True]"},
    { NULL, NULL, 0, NULL }        /* Sentinel */
};

/* module documentation */
PyDoc_STRVAR( _crc32_doc,
"Calculation of CRC32 \n"
"libscrc.fsc        -> Calculate CRC for Ethernet frame sequence (FSC) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.mpeg2      -> Calculate CRC for Media file (MPEG2) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=False Refout=False]\n"
"libscrc.crc32      -> Calculate CRC for file [Poly=0xEDB88320L, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True Refout=True]\n"
"libscrc.table32    -> Print CRC32 table to list\n"
"libscrc.hacker32   -> Free calculation CRC32 (not support python2 series) Xorout=0x00000000 Refin=False Refout=False\n"
"libscrc.adler32    -> Calculate adler32 (MOD=65521)\n"
"libscrc.fletcher32 -> Calculate fletcher32\n"
"libscrc.posix      -> Calculate CRC (POSIX) [Poly=0x04C11DB7, Init=0x00000000, Xorout=0xFFFFFFFF Refin=False Refout=False]\n"
"libscrc.bzip2      -> Calculate CRC (BZIP2) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True  Refout=True]\n"
"libscrc.jamcrc     -> Calculate CRC (JAMCRC) [Poly=0x04C11DB7, Init=0xFFFFFFFF, Xorout=0x00000000 Refin=True  Refout=True]\n"
"libscrc.autosar    -> Calculate CRC (AUTOSAR) [Poly=0xF4ACFB13, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True  Refout=True]\n"
"libscrc.crc32_c    -> Calculate CRC (CRC32_C) [Poly=0x1EDC6F41, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True  Refout=True]\n"
"libscrc.crc32_d    -> Calculate CRC (CRC32_D) [Poly=0xA833982B, Init=0xFFFFFFFF, Xorout=0xFFFFFFFF Refin=True  Refout=True]\n"
"libscrc.crc32_q    -> Calculate CRC (CRC32_Q) [Poly=0x814141AB, Init=0x00000000, Xorout=0x00000000 Refin=True  Refout=True]\n"
"libscrc.xfer       -> Calculate CRC (XFER) [Poly=0x000000AF, Init=0x00000000, Xorout=0x00000000 Refin=True  Refout=True]\n"
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

    PyModule_AddStringConstant( m, "__version__", "1.1"  );
    PyModule_AddStringConstant( m, "__author__",  "Heyn" );

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
