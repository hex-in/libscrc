/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crcxmodule.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized Code.
*
*********************************************************************************************************
*/

#include <Python.h>
#include "_crcxtables.h"

static unsigned char hexin_PyArg_ParseTuple( PyObject *self, PyObject *args,
                                             unsigned char init,
                                             unsigned char (*function)( unsigned char *,
                                                                        unsigned int,
                                                                        unsigned char ),
                                             unsigned char *result )
{
    Py_buffer data = { NULL, NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*|B", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*|B", &data, &init ) ) {
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

static PyObject * _crc3_gsm( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc3_gsm, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( ( result ^ 0x07 ) & 0x07 ) );
}

static PyObject * _crc3_rohc( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x07;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc3_rohc, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x07 ) );
}

/*
*********************************************************************************************************
*                                   POLY=0x03 [CRC4-ITU]
* Poly:    0x03
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0x0C = hexin_reverse8( 0x03 ) >> (8-4)
*********************************************************************************************************
*/

static PyObject * _crc4_itu( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc4_0c, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x0F ) );
}

static PyObject * _crc4_interlaken4( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x0F;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc4_03, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( (result ^ 0x0F) & 0x0F ) );
}

/*
*********************************************************************************************************
*                                   POLY=0x15 [CRC5-ITU]
* Poly:    0x15
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0x15 = hexin_reverse8( 0x15 ) >> ( 8-5 )
*********************************************************************************************************
*/

static PyObject * _crc5_itu( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc5_15, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x1F ) );
}

/*
*********************************************************************************************************
*                                   POLY=0x09 [CRC5-EPC]
* Poly:    0x09
* Init:    0x09
* Refin:   False
* Refout:  False
* Xorout:  0x00
*
* 0x48 = ( 0x09 << ( 8-5 ) )
*********************************************************************************************************
*/
static PyObject * _crc5_epc( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x48;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc5_09, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( ( result >> 3 ) & 0x1F ) );
}

/*
*********************************************************************************************************
*                                   POLY=0x05 [CRC5-USB]
* Poly:    0x05
* Init:    0x1F
* Refin:   True
* Refout:  True
* Xorout:  0x1F
*
* 0x14 = hexin_reverse8( 0x05 ) >> ( 8-5 )
*********************************************************************************************************
*/
static PyObject * _crc5_usb( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x1F;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc5_14, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( (result ^ 0x1F) & 0x1F ) );
}

/*
*********************************************************************************************************
*                                   POLY=0x03 [CRC6-ITU]
* Poly:    0x03
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*
* 0x30 = hexin_reverse8( 0x03 ) >> ( 8-6 )
*********************************************************************************************************
*/
static PyObject * _crc6_itu( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc6_30, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x3F ) );
}

/*
*********************************************************************************************************
* Poly:    0x2F
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x3F
*
* ( 0x2F << ( 8-6 ) ) = 0xBC
*********************************************************************************************************
*/
static PyObject * _crc6_gsm( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc6_2f, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B",  ( ( result ^ 0x3F ) & 0x3F ) );
}

static PyObject * _crc6_darc6( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc6_19, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B",  ( result & 0x3F ) );
}

static PyObject * _crc7_mmc( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc7_09, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x7F ) );
}

static PyObject * _crc7_umts7( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc7_45, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x7F ) );
}

static PyObject * _crc7_rohc7( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x7F;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc7_4f, &result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", ( result & 0x7F ) );
}

/* method table */
static PyMethodDef _crcxMethods[] = {
    { "gsm3",    _crc3_gsm,          METH_VARARGS, "Calculate GSM  of CRC3 [Poly=0x03 Initial=0x00 Xorout=0x07 Refin=False Refout=False]" },
    { "rohc3",   _crc3_rohc,         METH_VARARGS, "Calculate ROHC of CRC3 [Poly=0x03 Initial=0x07 Xorout=0x00 Refin=True Refout=True]" },
    { "itu4",    _crc4_itu,          METH_VARARGS, "Calculate ITU  of CRC4 [Poly=0x03 Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "g_704",   _crc4_itu,          METH_VARARGS, "Calculate G-704 of CRC4 [Poly=0x03 Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "itu5",    _crc5_itu,          METH_VARARGS, "Calculate ITU  of CRC5 [Poly=0x15 Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "epc",     _crc5_epc,          METH_VARARGS, "Calculate EPC  of CRC5 [Poly=0x09 Initial=0x09 Xorout=0x00 Refin=False Refout=False]" },
    { "usb5",    _crc5_usb,          METH_VARARGS, "Calculate USB  of CRC5 [Poly=0x05 Initial=0x1F Xorout=0x1F Refin=True Refout=True]" },
    { "itu6",    _crc6_itu,          METH_VARARGS, "Calculate ITU  of CRC6 [Poly=0x03 Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "gsm6",    _crc6_gsm,          METH_VARARGS, "Calculate GSM  of CRC6 [Poly=0x2F Initial=0x00 Xorout=0x3F Refin=False Refout=False]" },
    { "darc6",   _crc6_darc6,        METH_VARARGS, "Calculate ROHC of CRC6 [Poly=0x19 Initial=0x00 Xorout=0x00 Refin=True Refout=True]"   },
    { "mmc",     _crc7_mmc,          METH_VARARGS, "Calculate MMC  of CRC7 [Poly=0x09 Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "crc7",    _crc7_mmc,          METH_VARARGS, "Calculate CRC  of CRC7 [Poly=0x09 Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "umts7",   _crc7_umts7,        METH_VARARGS, "Calculate UMTS of CRC7 [Poly=0x45 Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "rohc7",   _crc7_rohc7,        METH_VARARGS, "Calculate ROHC of CRC7 [Poly=0x4F Initial=0x7F Xorout=0x00 Refin=True Refout=True]"   },
    
    { "interlaken4",    _crc4_interlaken4,  METH_VARARGS, "Calculate INTERLAKEN of CRC4 [Poly=0x03 Initial=0x0F Xorout=0x0F Refin=False Refout=False]" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _crcx_doc,
"Calculation of CRCx \n"
"libscrc.gsm3   -> Calculate GSM  of CRC3 [Poly=0x03 Initial=0x00 Xorout=0x07 Refin=False Refout=False]\n"
"libscrc.rohc3  -> Calculate ROHC of CRC3 [Poly=0x03 Initial=0x07 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.itu4   -> Calculate ITU  of CRC4 [Poly=0x03 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.g_704  -> Calculate G-704 of CRC4 [Poly=0x03 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.itu5   -> Calculate ITU  of CRC5 [Poly=0x15 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.epc    -> Calculate EPC  of CRC5 [Poly=0x09 Initial=0x09 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.usb5   -> Calculate USB  of CRC5 [Poly=0x05 Initial=0x1F Xorout=0x1F Refin=True Refout=True]\n"
"libscrc.itu6   -> Calculate ITU  of CRC6 [Poly=0x03 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.gsm6   -> Calculate GSM  of CRC6 [Poly=0x2F Initial=0x00 Xorout=0x3F Refin=False Refout=False]\n"
"libscrc.darc6  -> Calculate DARC of CRC6 [Poly=0x19 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.mmc    -> Calculate MMC  of CRC7 [Poly=0x09 Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.crc7   -> Calculate CRC  of CRC7 [Poly=0x09 Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.umts7  -> Calculate UMTS of CRC7 [Poly=0x45 Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.rohc7  -> Calculate ROHC of CRC7 [Poly=0x4F Initial=0x7F Xorout=0x00 Refin=True Refout=True]\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crcxmodule = {
   PyModuleDef_HEAD_INIT,
   "_crcx",                    /* name of module */
   _crcx_doc,                  /* module documentation, may be NULL */
   -1,                         /* size of per-interpreter state of the module */
   _crcxMethods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crcx( void )
{
    PyObject *m = NULL;

    m = PyModule_Create( &_crcxmodule );
    if ( m == NULL ) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "1.1" );
    PyModule_AddStringConstant( m, "__author__",  "Heyn"  );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crcx( void )
{
    (void) Py_InitModule3( "_crcx", _crcxMethods, _crcx_doc );
}

#endif /* PY_MAJOR_VERSION */
