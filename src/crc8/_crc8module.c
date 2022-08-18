/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc8module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.8
*
* LICENSING TERMS:
* ---------------
*       WebTool : http://reveng.sourceforge.net/crc-catalogue/1-15.htm#crc.cat-bits.8
*		New Create at 	2017-08-10 08:42AM
*                       2017-08-17 [Heyn] Optimized code.
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2017-08-22 [Heyn] Bugfixes Parsing arguments
*                                         Change PyArg_ParseTuple(* , "y#|I")
*                                         To     PyArg_ParseTuple(* , "y#|B")
*                                         "B" : Convert a Python integer to a tiny int without overflow
*                                               checking, stored in a C unsigned char.
*                       2017-09-21 [Heyn] Optimized code for _crc8_maxim
*                                         New CRC8-MAXIM8 CRC8-ROHC CRC8-ITU8 CRC8
*                       2020-02-17 [Heyn] New CRC8-SUM.
*                       2020-03-20 [Heyn] New CRC8-FLETCHER8.
*                       2020-04-17 [Heyn] Issues #1
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2020-09-18 [Heyn] New add lin and lin2x checksum.
*                       2020-11-18 [Heyn] Fixed (Python2) Parsing arguments has no 'p' type
*                       2021-06-07 [Heyn] Add hacker8() reinit parameter. reinit=True -> Reinitialize the table
*                       2022-03-04 [Heyn] New add modbus(ASCII) checksum(LRC).
*                       2022-08-18 [Heyn] Fix Python deprication (PY_SSIZE_T_CLEAN)
*
*********************************************************************************************************
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "_crc8tables.h"

static unsigned char hexin_PyArg_ParseTuple( PyObject *self, PyObject *args,
                                             unsigned char init,
                                             unsigned char (*function)( const unsigned char *,
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

    *result = (* function)( (const unsigned char *)data.buf, (unsigned int)data.len, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_crc8 *param )
{
    Py_buffer data = { NULL, NULL };
    unsigned char init = param->init;

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

    /* Fixed Issues #4  */
    param->is_gradual = ( unsigned int )PyTuple_Size( args );   // Fixed warning C4244
    if ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) {
        init = ( init ^ param->xorout );
    }
    
    param->result = hexin_crc8_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _crc8_intel( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_lrc, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_bcc( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_bcc, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_lrc( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_lrc, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_maxim( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_maxim = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_31,
                                                   .init   = 0x00,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0x00,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_maxim ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_maxim.result );
}

static PyObject * _crc8_rohc( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_rohc = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC8_WIDTH,
                                                  .poly   = CRC8_POLYNOMIAL_07,
                                                  .init   = 0xFF,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_rohc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_rohc.result );
}

static PyObject * _crc8_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_itu8 = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC8_WIDTH,
                                                  .poly   = CRC8_POLYNOMIAL_07,
                                                  .init   = 0x00,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x55,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_itu8 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_itu8.result );
}

static PyObject * _crc8_crc8( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_crc8 = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC8_WIDTH,
                                                  .poly   = CRC8_POLYNOMIAL_07,
                                                  .init   = 0x00,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_crc8 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_crc8.result );
}

static PyObject * _crc8_sum( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_sum, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_table( PyObject *self, PyObject *args )
{
    unsigned int i = 0x00000000L;
    unsigned int ref = FALSE;
    unsigned char poly = CRC8_POLYNOMIAL_31;
    unsigned char table[MAX_TABLE_ARRAY] = { 0x00 };
    PyObject* plist = PyList_New( MAX_TABLE_ARRAY );

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "B|p", &poly, &ref ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "B|p", &poly, &ref ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( FALSE == ref ) {
        hexin_crc8_init_table_poly_is_low ( poly, table );
    } else {
        hexin_crc8_init_table_poly_is_high( hexin_reverse8( poly ), table );
    }

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        PyList_SetItem( plist, i, Py_BuildValue( "B", table[i] ) );
    }

    return plist;
}

static PyObject * _crc8_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    unsigned int reinit = FALSE;
    Py_buffer data = { NULL, NULL };
    static struct _hexin_crc8 crc8_param_hacker = { .is_initial = FALSE,
                                                    .is_gradual = FALSE,
                                                    .width  = HEXIN_CRC8_WIDTH,
                                                    .poly   = CRC8_POLYNOMIAL_31,
                                                    .init   = 0xFF,
                                                    .refin  = FALSE,
                                                    .refout = FALSE,
                                                    .xorout = 0x00,
                                                    .result = 0 };

    static char* kwlist[]={ "data", "poly", "init", "xorout", "refin", "refout", "reinit", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y*|BBBppp", kwlist, &data,
                                                                       &crc8_param_hacker.poly,
                                                                       &crc8_param_hacker.init,
                                                                       &crc8_param_hacker.xorout,
                                                                       &crc8_param_hacker.refin,
                                                                       &crc8_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;
    }
#else
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "s*|BBBIII", kwlist, &data,
                                                                       &crc8_param_hacker.poly,
                                                                       &crc8_param_hacker.init,
                                                                       &crc8_param_hacker.xorout,
                                                                       &crc8_param_hacker.refin,
                                                                       &crc8_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;
    }
#endif /* PY_MAJOR_VERSION */

    crc8_param_hacker.is_initial = ( reinit == FALSE ) ? crc8_param_hacker.is_initial : FALSE;
    crc8_param_hacker.result = hexin_crc8_compute( (const unsigned char *)data.buf, (unsigned int)data.len, &crc8_param_hacker, crc8_param_hacker.init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_BuildValue( "B", crc8_param_hacker.result );
}

static PyObject * _crc8_fletcher( PyObject *self, PyObject *args )
{
    unsigned char result   = 0x00;
    unsigned char reserved = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, reserved, hexin_calc_crc8_fletcher, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_autosar8( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_autosar8 = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC8_WIDTH,
                                                      .poly   = CRC8_POLYNOMIAL_2F,
                                                      .init   = 0xFF,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0xFF,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_autosar8 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_autosar8.result );
}

static PyObject * _crc8_lte8( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_lte8 = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC8_WIDTH,
                                                  .poly   = CRC8_POLYNOMIAL_9B,
                                                  .init   = 0x00,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_lte8 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_lte8.result );
}

static PyObject * _crc8_wcdma( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_wcdma = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_9B,
                                                   .init   = 0x00,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0x00,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_wcdma ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_wcdma.result );
}

static PyObject * _crc8_sae_j1850( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_j1850 = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_1D,
                                                   .init   = 0xFF,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0xFF,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_j1850 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_j1850.result );
}

static PyObject * _crc8_icode( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_icode = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_1D,
                                                   .init   = 0xFD,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_icode ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_icode.result );
}

static PyObject * _crc8_gsm8_a( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_gsm8a = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_1D,
                                                   .init   = 0x00,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_gsm8a ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_gsm8a.result );
}

static PyObject * _crc8_gsm8_b( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_gsm8b = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_49,
                                                   .init   = 0x00,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0xFF,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_gsm8b ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_gsm8b.result );
}

static PyObject * _crc8_nrsc_5( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_nrsc5 = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_31,
                                                   .init   = 0xFF,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_nrsc5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_nrsc5.result );
}

static PyObject * _crc8_bluetooth( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_bluetooth = { .is_initial=FALSE,
                                                       .width  = HEXIN_CRC8_WIDTH,
                                                       .poly   = CRC8_POLYNOMIAL_A7,
                                                       .init   = 0x00,
                                                       .refin  = TRUE,
                                                       .refout = TRUE,
                                                       .xorout = 0x00,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_bluetooth ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_bluetooth.result );
}

static PyObject * _crc8_dvb_s2( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_dvbs2 = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC8_WIDTH,
                                                   .poly   = CRC8_POLYNOMIAL_D5,
                                                   .init   = 0x00,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x00,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_dvbs2 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_dvbs2.result );
}

static PyObject * _crc8_ebu8( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_ebu8 = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC8_WIDTH,
                                                  .poly   = CRC8_POLYNOMIAL_1D,
                                                  .init   = 0xFF,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_ebu8 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_ebu8.result );
}

static PyObject * _crc8_darc( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_darc = { .is_initial=FALSE,
                                                  .width  = HEXIN_CRC8_WIDTH,
                                                  .poly   = CRC8_POLYNOMIAL_39,
                                                  .init   = 0x00,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_darc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_darc.result );
}

static PyObject * _crc8_opensafety8( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_opensafety8 = { .is_initial=FALSE,
                                                         .width  = HEXIN_CRC8_WIDTH,
                                                         .poly   = CRC8_POLYNOMIAL_2F,
                                                         .init   = 0x00,
                                                         .refin  = FALSE,
                                                         .refout = FALSE,
                                                         .xorout = 0x00,
                                                         .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_opensafety8 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_opensafety8.result );
}

static PyObject * _crc8_mifare_mad( PyObject *self, PyObject *args )
{
    static struct _hexin_crc8 crc8_param_mifare_mad = { .is_initial=FALSE,
                                                        .width  = HEXIN_CRC8_WIDTH,
                                                        .poly   = CRC8_POLYNOMIAL_1D,
                                                        .init   = 0xC7,
                                                        .refin  = FALSE,
                                                        .refout = FALSE,
                                                        .xorout = 0x00,
                                                        .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc8_param_mifare_mad ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc8_param_mifare_mad.result );
}

static PyObject * _crc8_lin( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_lin, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_lin2x( PyObject *self, PyObject *args )
{
    unsigned char crc = 0x00;
    unsigned char pid = 0x00;
    Py_buffer data  = { NULL, NULL };
    PyObject *pDict = Py_None;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*", &data ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*", &data ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#endif /* PY_MAJOR_VERSION */

    crc = hexin_calc_crc8_lin2x( ( const unsigned char * )data.buf, (unsigned int)data.len, 0 );
    pid = hexin_crc8_get_lin2x_pid( (( const unsigned char * )data.buf)[0] );

    pDict = PyDict_New();
    PyDict_SetItem( pDict, Py_BuildValue( "s", "crc" ),  Py_BuildValue( "B", crc ) );
    PyDict_SetItem( pDict, Py_BuildValue( "s", "pid" ),  Py_BuildValue( "B", pid ) );
    
    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_INCREF( pDict ), pDict;
}

static PyObject * _crc8_id8( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_id8, ( unsigned char * )&result ) ) {
        return NULL;
    }

    if ( result == 'N' ) {
        Py_RETURN_NONE;
    }

    return Py_BuildValue( "z#", &result, 1 );
}

static PyObject * _crc8_nmea( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00;
    unsigned char init   = 0x00;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc8_nmea, ( unsigned char * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", result );
}

static PyObject * _crc8_modbus_asc( PyObject *self, PyObject *args )
{
    unsigned char result = 0x00, tmp = 0x00;
    unsigned char init   = 0x00;
    unsigned char crc[2] = { 0x00, 0x00 };

    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_modbus_ascii, ( unsigned char * )&result ) ) {
        return NULL;
    }

    tmp   = ( result >> 4 ) & 0x0F;
    crc[0] = ( tmp > 9 ? ( tmp - 10 + 'A' ) : ( tmp + 0x30 ) );
    tmp   = ( result >> 0 ) & 0x0F;
    crc[1] = ( tmp > 9 ? ( tmp - 10 + 'A' ) : ( tmp + 0x30 ) );
    
    return Py_BuildValue( "y#", crc, 2 );
}

/* method table */
static PyMethodDef _crc8Methods[] = {
    { "intel",      (PyCFunction)_crc8_intel,        METH_VARARGS, "Calculate Intel hexadecimal of CRC8 [Initial=0x00]" },
    { "bcc",        (PyCFunction)_crc8_bcc,          METH_VARARGS, "Calculate BCC(XOR) of CRC8 [Initial=0x00]" },
    { "lrc",        (PyCFunction)_crc8_lrc,          METH_VARARGS, "Calculate LRC of CRC8 [Initial=0x00]" },
    { "maxim8",     (PyCFunction)_crc8_maxim,        METH_VARARGS, "Calculate MAXIM(MAXIM-DOM) of CRC8 [Poly=0x31 Initial=0x00 Xorout=0x00 Refin=True Refout=True] e.g. DS18B20" },
    { "rohc",       (PyCFunction)_crc8_rohc,         METH_VARARGS, "Calculate ROHC of CRC8 [Poly=0x07 Initial=0xFF Xorout=0x00 Refin=True Refout=True]" },
    { "itu8",       (PyCFunction)_crc8_itu,          METH_VARARGS, "Calculate ITU of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x55 Refin=False Refout=False]" },
    { "i432_1",     (PyCFunction)_crc8_itu,          METH_VARARGS, "Calculate I-432-1 of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x55 Refin=False Refout=False]" },
    { "crc8",       (PyCFunction)_crc8_crc8,         METH_VARARGS, "Calculate CRC of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "sum8",       (PyCFunction)_crc8_sum,          METH_VARARGS, "Calculate SUM of CRC8 [Initial=0x00]" },
    { "table8",     (PyCFunction)_crc8_table,        METH_VARARGS, "Print CRC8 table to list. libscrc.table8( polynomial )" },
    { "hacker8",    (PyCFunction)_crc8_hacker,       METH_KEYWORDS|METH_VARARGS, "User calculation CRC8\n"
                                                                                 "@data   : bytes\n"
                                                                                 "@poly   : default=0x31\n"
                                                                                 "@init   : default=0xFF\n"
                                                                                 "@xorout : default=0x00\n"
                                                                                 "@refin  : default=False\n"
                                                                                 "@refout : default=False\n"
                                                                                 "@reinit : default=False" },
    { "fletcher8",  (PyCFunction)_crc8_fletcher,     METH_VARARGS, "Calculate fletcher8" },
    { "smbus",      (PyCFunction)_crc8_crc8,         METH_VARARGS, "Calculate SMBUS of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "autosar8",   (PyCFunction)_crc8_autosar8,     METH_VARARGS, "Calculate AUTOSAR of CRC8 [Poly=0x2F Initial=0xFF Xorout=0xFF Refin=False Refout=False]" },
    { "lte8",       (PyCFunction)_crc8_lte8,         METH_VARARGS, "Calculate LTE of CRC8 [Poly=0x9B Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "sae_j1850",  (PyCFunction)_crc8_sae_j1850,    METH_VARARGS, "Calculate SAE-J1850 of CRC8 [Poly=0x1D Initial=0xFF Xorout=0xFF Refin=False Refout=False]" },
    { "icode8",     (PyCFunction)_crc8_icode,        METH_VARARGS, "Calculate I-CODE of CRC8 [Poly=0x1D Initial=0xFD Xorout=0x00 Refin=False Refout=False]" },
    { "gsm8_a",     (PyCFunction)_crc8_gsm8_a,       METH_VARARGS, "Calculate GSM8-A of CRC8 [Poly=0x1D Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "gsm8_b",     (PyCFunction)_crc8_gsm8_b,       METH_VARARGS, "Calculate GSM8-B of CRC8 [Poly=0x49 Initial=0x00 Xorout=0xFF Refin=False Refout=False]" },
    { "nrsc_5",     (PyCFunction)_crc8_nrsc_5,       METH_VARARGS, "Calculate NRSC-5 of CRC8 [Poly=0x31 Initial=0xFF Xorout=0x00 Refin=False Refout=False]" },
    { "wcdma8",     (PyCFunction)_crc8_wcdma,        METH_VARARGS, "Calculate WCDMA of CRC8 [Poly=0x9B Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "bluetooth",  (PyCFunction)_crc8_bluetooth,    METH_VARARGS, "Calculate BLUETOOTH of CRC8 [Poly=0xA7 Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "dvb_s2",     (PyCFunction)_crc8_dvb_s2,       METH_VARARGS, "Calculate DVB-S2 of CRC8 [Poly=0xD5 Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "ebu8",       (PyCFunction)_crc8_ebu8,         METH_VARARGS, "Calculate EBU of CRC8 [Poly=0x1D Initial=0xFF Xorout=0x00 Refin=True Refout=True]" },
    { "tech_3250",  (PyCFunction)_crc8_ebu8,         METH_VARARGS, "Calculate TECH-3250 of CRC8 [Poly=0x1D Initial=0xFF Xorout=0x00 Refin=True Refout=True]" },
    { "aes8",       (PyCFunction)_crc8_ebu8,         METH_VARARGS, "Calculate AES of CRC8 [Poly=0x1D Initial=0xFF Xorout=0x00 Refin=True Refout=True]" },
    { "darc8",      (PyCFunction)_crc8_darc,         METH_VARARGS, "Calculate DARC of CRC8 [Poly=0x39 Initial=0x00 Xorout=0x00 Refin=True Refout=True]" },
    { "opensafety8",(PyCFunction)_crc8_opensafety8,  METH_VARARGS, "Calculate OPENSAFETY of CRC8 [Poly=0x2F Initial=0x00 Xorout=0x00 Refin=False Refout=False]" },
    { "mifare_mad", (PyCFunction)_crc8_mifare_mad,   METH_VARARGS, "Calculate MIFARE-MAD of CRC8 [Poly=0x1D Initial=0xC7 Xorout=0x00 Refin=False Refout=False]" },
    { "lin",        (PyCFunction)_crc8_lin,          METH_VARARGS, "Calculate LIN Protocol 1.3 (CLASSIC)"   },
    { "lin2x",      (PyCFunction)_crc8_lin2x,        METH_VARARGS, "Calculate LIN Protocol 2.x (ENHANCED)"  },
    { "id8",        (PyCFunction)_crc8_id8,          METH_VARARGS, "Calculate identity card of CHINA."      },
    { "nmea",       (PyCFunction)_crc8_nmea,         METH_VARARGS, "Calculate NMEA Checksum. XOR of all the bytes between the $ and the * (not including the delimiters themselves)" },
    { "modbus_asc", (PyCFunction)_crc8_modbus_asc,   METH_VARARGS, "Calculate Modbus(ASCII) Checksum."      },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};

/* module documentation */
PyDoc_STRVAR( _crc8_doc,
"Calculation of CRC8 \n"
"libscrc.intel      -> Calculate Intel hexadecimal of CRC8 [Initial=0x00]\n"
"libscrc.bcc        -> Calculate BCC(XOR) of CRC8 [Initial=0x00]\n"
"libscrc.lrc        -> Calculate LRC of CRC8 [Initial=0x00]\n"
"libscrc.maxim8     -> Calculate MAXIM(MAXIM-DOM) of CRC8 [Poly=0x31 Initial=0x00 Xorout=0x00 Refin=True Refout=True] e.g. DS18B20\n"
"libscrc.rohc       -> Calculate ROHC of CRC8 [Poly=0x07 Initial=0xFF Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.itu8       -> Calculate ITU of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x55 Refin=False Refout=False]\n"
"libscrc.i432_1     -> Calculate I-432-1 of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x55 Refin=False Refout=False]\n"
"libscrc.crc8       -> Calculate CRC of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.sum8       -> Calculate SUM of CRC8 [Initial=0x00]\n"
"libscrc.hacker8    -> Free calculation CRC8 @reinit reinitialize the crc8 tables\n"
"libscrc.fletcher8  -> Calculate fletcher8 \n"
"libscrc.smbus      -> Calculate SMBUS of CRC8 [Poly=0x07 Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.autosar8   -> Calculate AUTOSAR of CRC8 [Poly=0x2F Initial=0xFF Xorout=0xFF Refin=False Refout=False]\n"
"libscrc.lte8       -> Calculate LTE of CRC8 [Poly=0x9B Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.sae_j1850  -> Calculate SAE-J1850 of CRC8 [Poly=0x1D Initial=0xFF Xorout=0xFF Refin=False Refout=False]\n"
"libscrc.icode8     -> Calculate I-CODE of CRC8 [Poly=0x1D Initial=0xFD Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.gsm8_a     -> Calculate GSM8-A of CRC8 [Poly=0x1D Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.gsm8_b     -> Calculate GSM8-B of CRC8 [Poly=0x49 Initial=0x00 Xorout=0xFF Refin=False Refout=False]\n"
"libscrc.nrsc_5     -> Calculate NRSC-5 of CRC8 [Poly=0x31 Initial=0xFF Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.wcdma8     -> Calculate WCDMA of CRC8 [Poly=0x9B Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.bluetooth  -> Calculate BLUETOOTH of CRC8 [Poly=0xA7 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.dvb_s2     -> Calculate DVB-S2 of CRC8 [Poly=0xD5 Initial=0x00 Xorout=0x00 Refin=False Refout=False]]\n"
"libscrc.ebu8       -> Calculate EBU of CRC8 [Poly=0x1D Initial=0xFF Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.tech_3250  -> Calculate TECH-3250 of CRC8 [Poly=0x1D Initial=0xFF Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.aes8       -> Calculate AES of CRC8 [Poly=0x1D Initial=0xFF Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.darc8      -> Calculate DARC of CRC8 [Poly=0x39 Initial=0x00 Xorout=0x00 Refin=True Refout=True]\n"
"libscrc.opensafety8-> Calculate OPENSAFETY of CRC8 [Poly=0x2F Initial=0x00 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.mifare_mad -> Calculate MIFARE-MAD of CRC8 [Poly=0x1D Initial=0xC7 Xorout=0x00 Refin=False Refout=False]\n"
"libscrc.lin        -> Calculate LIN Protocol 1.3 (CLASSIC)\n"
"libscrc.lin2x      -> Calculate LIN Protocol 2.x (ENHANCED)\n"
"libscrc.id8        -> Calculate identity card of CHINA.\n"
"libscrc.nmea       -> Calculate NMEA Checksum. XOR of all the bytes between the $ and the * (not including the delimiters themselves).\n"
"libscrc.modbus_asc -> Calculate Modbus(ASCII) Checksum.\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc8module = {
   PyModuleDef_HEAD_INIT,
   "_crc8",                    /* name of module */
   _crc8_doc,                  /* module documentation, may be NULL */
   -1,                         /* size of per-interpreter state of the module */
   _crc8Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc8( void )
{
    PyObject *m = NULL;

    m = PyModule_Create( &_crc8module );
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
init_crc8( void )
{
    (void) Py_InitModule3( "_crc8", _crc8Methods, _crc8_doc );
}

#endif /* PY_MAJOR_VERSION */
