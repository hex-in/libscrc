/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc16module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-09 09:52AM
*                       2017-08-17 [Heyn] Optimized Code.
*                                         Wheel 0.0.4 New CRC16-SICK / CRC16-DNP
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2017-08-22 [Heyn] Bugfixes Parsing arguments
*                                           Change PyArg_ParseTuple(* , "y#|I")
*                                           To     PyArg_ParseTuple(* , "y#|H")
*                                           "H" : Convert a Python integer to a C unsigned short int,
*                                               without overflow checking.
*                       2017-09-19 [Heyn] New CRC16-X25 CRC16-USB CRC16-MAXIM16 CRC16-DECT. (V0.1.3)
*                                         Optimized Code.
*                       2020-03-13 [Heyn] New add hacker16 code.
*                       2020-03-20 [Heyn] New add UDP and TCP checksum.
*                       2020-03-23 [Blazin64] Adds support for the CRC method used by MCRF4XX RFID hardware.
*                       2020-04-08 [Heyn] New add libscrc.epc16() for RFID tag EPC
*                       2020-04-17 [Heyn] Fixed Issues #1
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2020-11-18 [Heyn] Fixed (Python2) Parsing arguments has no 'p' type
*                       2021-06-07 [Heyn] Add hacker16() reinit parameter. reinit=True -> Reinitialize the table
*                       2022-08-18 [Heyn] Fix Python deprication (PY_SSIZE_T_CLEAN)
*
*********************************************************************************************************
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "_crc16tables.h"

static unsigned char hexin_PyArg_ParseTuple( PyObject *self, PyObject *args,
                                             unsigned short init,
                                             unsigned short (*function)( const unsigned char *,
                                                                         unsigned int,
                                                                         unsigned short ),
                                             unsigned short *result )
{
    Py_buffer data = { NULL, NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*|H", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*|H", &data, &init ) ) {
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

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_crc16 *param )
{
    Py_buffer data = { NULL, NULL };
    unsigned short init = param->init;          /* Fixed Issues #4  */

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y*|H", &data, &init ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return FALSE;
    }
#else
    if ( !PyArg_ParseTuple( args, "s*|H", &data, &init ) ) {
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

    param->result = hexin_crc16_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _crc16_modbus( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_modbus = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_8005,
                                                      .init   = 0xFFFF,
                                                      .refin  = TRUE,
                                                      .refout = TRUE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_modbus ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_modbus.result );
}

static PyObject * _crc16_usb( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_usb = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC16_WIDTH,
                                                   .poly   = CRC16_POLYNOMIAL_8005,
                                                   .init   = 0xFFFF,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0xFFFF,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_usb ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_usb.result );
}

static PyObject * _crc16_ibm( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_ibm = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC16_WIDTH,
                                                   .poly   = CRC16_POLYNOMIAL_8005,
                                                   .init   = 0x0000,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0x0000,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_ibm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_ibm.result );
}

static PyObject * _crc16_xmodem( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_xmodem = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_1021,
                                                      .init   = 0x0000,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_xmodem ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_xmodem.result );
}


static PyObject * _crc16_ccitt( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_ccitt_false = { .is_initial=FALSE,
                                                           .width  = HEXIN_CRC16_WIDTH,
                                                           .poly   = CRC16_POLYNOMIAL_1021,
                                                           .init   = 0xFFFF,
                                                           .refin  = FALSE,
                                                           .refout = FALSE,
                                                           .xorout = 0x0000,
                                                           .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_ccitt_false ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_ccitt_false.result );
}

static PyObject * _crc16_ccitt_aug( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_ccitt_aug = { .is_initial=FALSE,
                                                         .width  = HEXIN_CRC16_WIDTH,
                                                         .poly   = CRC16_POLYNOMIAL_1021,
                                                         .init   = 0x1D0F,
                                                         .refin  = FALSE,
                                                         .refout = FALSE,
                                                         .xorout = 0x0000,
                                                         .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_ccitt_aug ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_ccitt_aug.result );
}

static PyObject * _crc16_kermit( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_kermit = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_1021,
                                                      .init   = 0x0000,
                                                      .refin  = TRUE,
                                                      .refout = TRUE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_kermit ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_kermit.result );
}

static PyObject * _crc16_mcrf4xx( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_mcrf4xx = { .is_initial=FALSE,
                                                       .width  = HEXIN_CRC16_WIDTH,
                                                       .poly   = CRC16_POLYNOMIAL_1021,
                                                       .init   = 0xFFFF,
                                                       .refin  = TRUE,
                                                       .refout = TRUE,
                                                       .xorout = 0x0000,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_mcrf4xx ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_mcrf4xx.result );
}

static PyObject * _crc16_x25( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_x25 = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC16_WIDTH,
                                                   .poly   = CRC16_POLYNOMIAL_1021,
                                                   .init   = 0xFFFF,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0xFFFF,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_x25 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_x25.result );
}

static PyObject * _crc16_sick( PyObject *self, PyObject *args )
{
    unsigned short result = 0x0000;
    unsigned short init   = 0x0000;
 
    if ( !hexin_PyArg_ParseTuple( self, args, init, hexin_calc_crc16_sick, ( unsigned short * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", result );
}

static PyObject * _crc16_dnp( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_dnp = { .is_initial=FALSE,
                                                   .width  = HEXIN_CRC16_WIDTH,
                                                   .poly   = CRC16_POLYNOMIAL_3D65,
                                                   .init   = 0x0000,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0xFFFF,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_dnp ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_dnp.result );
}

static PyObject * _crc16_maxim( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_maxim = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = CRC16_POLYNOMIAL_8005,
                                                     .init   = 0x0000,
                                                     .refin  = TRUE,
                                                     .refout = TRUE,
                                                     .xorout = 0xFFFF,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_maxim ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_maxim.result );
}

static PyObject * _crc16_dect_r( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_dectr = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = CRC16_POLYNOMIAL_0589,
                                                     .init   = 0x0000,
                                                     .refin  = FALSE,
                                                     .refout = FALSE,
                                                     .xorout = 0x0001,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_dectr ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_dectr.result );
}

static PyObject * _crc16_dect_x( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_dectx = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = CRC16_POLYNOMIAL_0589,
                                                     .init   = 0x0000,
                                                     .refin  = FALSE,
                                                     .refout = FALSE,
                                                     .xorout = 0x0000,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_dectx ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_dectx.result );
}

static PyObject * _crc16_table( PyObject *self, PyObject *args )
{
    unsigned int i   = 0x00000000L;
    unsigned int ref = FALSE;
    unsigned short poly = CRC16_POLYNOMIAL_8005;
    unsigned short table[MAX_TABLE_ARRAY] = { 0x0000 };
    PyObject* plist = PyList_New( MAX_TABLE_ARRAY );

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "H|p", &poly, &ref ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "H|p", &poly, &ref  ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( FALSE == ref ) {
        hexin_crc16_init_table_poly_is_low ( poly, table );
    } else {
        hexin_crc16_init_table_poly_is_high( hexin_reverse16( poly ), table );
    }

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        PyList_SetItem( plist, i, Py_BuildValue( "H", table[i] ) );
    }

    return plist;
}

static PyObject * _crc16_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    unsigned int reinit = FALSE;
    Py_buffer data = { NULL, NULL };
    static struct _hexin_crc16 crc16_param_hacker = { .is_initial = FALSE,
                                                      .is_gradual = FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_8005,
                                                      .init   = 0xFFFF,
                                                      .refin  = TRUE,
                                                      .refout = TRUE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    static char* kwlist[]={ "data", "poly", "init", "xorout", "refin", "refout", "reinit", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y*|HHHppp", kwlist, &data,
                                                                       &crc16_param_hacker.poly,
                                                                       &crc16_param_hacker.init,
                                                                       &crc16_param_hacker.xorout,
                                                                       &crc16_param_hacker.refin,
                                                                       &crc16_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;
    }
#else
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "s*|HHHIII", kwlist, &data,
                                                                       &crc16_param_hacker.poly,
                                                                       &crc16_param_hacker.init,
                                                                       &crc16_param_hacker.xorout,
                                                                       &crc16_param_hacker.refin,
                                                                       &crc16_param_hacker.refout,
                                                                       &reinit ) ) {
        if ( data.obj ) {
            PyBuffer_Release( &data );
        }
        return NULL;
    }
#endif /* PY_MAJOR_VERSION */

    crc16_param_hacker.is_initial = ( reinit == FALSE ) ? crc16_param_hacker.is_initial : FALSE;
    crc16_param_hacker.result = hexin_crc16_compute( (const unsigned char *)data.buf, (unsigned int)data.len, &crc16_param_hacker, crc16_param_hacker.init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return Py_BuildValue( "H", crc16_param_hacker.result );
}

static PyObject * _crc16_network( PyObject *self, PyObject *args )
{
    unsigned short result   = 0x0000;
    unsigned short reserved = 0x0000;
 
    if ( !hexin_PyArg_ParseTuple( self, args, reserved, hexin_calc_crc16_network, ( unsigned short * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", result );
}

static PyObject * _crc16_fletcher( PyObject *self, PyObject *args )
{
    unsigned short result   = 0x0000;
    unsigned short reserved = 0x0000;
 
    if ( !hexin_PyArg_ParseTuple( self, args, reserved, hexin_calc_crc16_fletcher, ( unsigned short * )&result ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", result );
}

static PyObject * _crc16_rfid_epc( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_epc16 = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = CRC16_POLYNOMIAL_1021,
                                                     .init   = 0xFFFF,
                                                     .refin  = FALSE,
                                                     .refout = FALSE,
                                                     .xorout = 0xFFFF,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_epc16 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_epc16.result );
}

static PyObject * _crc16_profibus( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_profibus = { .is_initial=FALSE,
                                                        .width  = HEXIN_CRC16_WIDTH,
                                                        .poly   = CRC16_POLYNOMIAL_1DCF,
                                                        .init   = 0xFFFF,
                                                        .refin  = FALSE,
                                                        .refout = FALSE,
                                                        .xorout = 0xFFFF,
                                                        .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_profibus ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_profibus.result );
}

static PyObject * _crc16_buypass( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_buypass = { .is_initial=FALSE,
                                                       .width  = HEXIN_CRC16_WIDTH,
                                                       .poly   = CRC16_POLYNOMIAL_8005,
                                                       .init   = 0x0000,
                                                       .refin  = FALSE,
                                                       .refout = FALSE,
                                                       .xorout = 0x0000,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_buypass ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_buypass.result );
}

static PyObject * _crc16_gsm16( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_gsm16 = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = CRC16_POLYNOMIAL_1021,
                                                     .init   = 0x0000,
                                                     .refin  = FALSE,
                                                     .refout = FALSE,
                                                     .xorout = 0xFFFF,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_gsm16 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_gsm16.result );
}

static PyObject * _crc16_riello( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_riello = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_1021,
                                                      .init   = 0xB2AA,
                                                      .refin  = TRUE,
                                                      .refout = TRUE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_riello ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_riello.result );
}

static PyObject * _crc16_crc16_a( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_crc16a = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_1021,
                                                      .init   = 0xC6C6,
                                                      .refin  = TRUE,
                                                      .refout = TRUE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_crc16a ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_crc16a.result );
}

static PyObject * _crc16_cdma2000( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_cdma2000 = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_C867,
                                                      .init   = 0xFFFF,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_cdma2000 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_cdma2000.result );
}

static PyObject * _crc16_teledisk( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_teledisk = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = 0xA097,
                                                      .init   = 0x0000,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_teledisk ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_teledisk.result );
}

static PyObject * _crc16_tms37157( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_tms37157 = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = CRC16_POLYNOMIAL_1021,
                                                      .init   = 0x89EC,
                                                      .refin  = TRUE,
                                                      .refout = TRUE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_tms37157 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_tms37157.result );
}

static PyObject * _crc16_en13757( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_en13757 = { .is_initial=FALSE,
                                                       .width  = HEXIN_CRC16_WIDTH,
                                                       .poly   = CRC16_POLYNOMIAL_3D65,
                                                       .init   = 0x0000,
                                                       .refin  = FALSE,
                                                       .refout = FALSE,
                                                       .xorout = 0xFFFF,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_en13757 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_en13757.result );
}

static PyObject * _crc16_t10_dif( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_t10_dif = { .is_initial=FALSE,
                                                       .width  = HEXIN_CRC16_WIDTH,
                                                       .poly   = 0x8BB7,
                                                       .init   = 0x0000,
                                                       .refin  = FALSE,
                                                       .refout = FALSE,
                                                       .xorout = 0x0000,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_t10_dif ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_t10_dif.result );
}

static PyObject * _crc16_dds_110( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_dds_110 = { .is_initial=FALSE,
                                                       .width  = HEXIN_CRC16_WIDTH,
                                                       .poly   = CRC16_POLYNOMIAL_8005,
                                                       .init   = 0x800D,
                                                       .refin  = FALSE,
                                                       .refout = FALSE,
                                                       .xorout = 0x0000,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_dds_110 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_dds_110.result );
}

static PyObject * _crc16_cms( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_cms16 = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = CRC16_POLYNOMIAL_8005,
                                                     .init   = 0xFFFF,
                                                     .refin  = FALSE,
                                                     .refout = FALSE,
                                                     .xorout = 0x0000,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_cms16 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_cms16.result );
}

static PyObject * _crc16_lj1200( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_lj1200 = { .is_initial=FALSE,
                                                      .width  = HEXIN_CRC16_WIDTH,
                                                      .poly   = 0x6F63,
                                                      .init   = 0x0000,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x0000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_lj1200 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_lj1200.result );
}

static PyObject * _crc16_nrsc5( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_nrsc5 = { .is_initial=FALSE,
                                                     .width  = HEXIN_CRC16_WIDTH,
                                                     .poly   = 0x080B,
                                                     .init   = 0xFFFF,
                                                     .refin  = TRUE,
                                                     .refout = TRUE,
                                                     .xorout = 0x0000,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_nrsc5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_nrsc5.result );
}

static PyObject * _crc16_opensafety_a( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_opensafety_a = { .is_initial=FALSE,
                                                            .width  = HEXIN_CRC16_WIDTH,
                                                            .poly   = 0x5935,
                                                            .init   = 0x0000,
                                                            .refin  = FALSE,
                                                            .refout = FALSE,
                                                            .xorout = 0x0000,
                                                            .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_opensafety_a ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_opensafety_a.result );
}

static PyObject * _crc16_opensafety_b( PyObject *self, PyObject *args )
{
    static struct _hexin_crc16 crc16_param_opensafety_b = { .is_initial=FALSE,
                                                            .width  = HEXIN_CRC16_WIDTH,
                                                            .poly   = 0x755B,
                                                            .init   = 0x0000,
                                                            .refin  = FALSE,
                                                            .refout = FALSE,
                                                            .xorout = 0x0000,
                                                            .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc16_param_opensafety_b ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc16_param_opensafety_b.result );
}

/* method table */
static PyMethodDef _crc16Methods[] = {
    { "modbus",      (PyCFunction)_crc16_modbus,    METH_VARARGS, "Calculate MODBUS of CRC16 [Poly=0x8005, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]" },
    { "usb16",       (PyCFunction)_crc16_usb,       METH_VARARGS, "Calculate USB of CRC16 [Poly=0x8005, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]" },
    { "ibm",         (PyCFunction)_crc16_ibm,       METH_VARARGS, "Calculate IBM (Alias:ARC/LHA) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "arc",         (PyCFunction)_crc16_ibm,       METH_VARARGS, "Calculate ARC (Alias:IBM/LHA) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "lha",         (PyCFunction)_crc16_ibm,       METH_VARARGS, "Calculate LHA (Alias:ARC/IBM) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "xmodem",      (PyCFunction)_crc16_xmodem,    METH_VARARGS, "Calculate XMODEM of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "zmodem",      (PyCFunction)_crc16_xmodem,    METH_VARARGS, "Calculate ZMODEM of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "acorn",       (PyCFunction)_crc16_xmodem,    METH_VARARGS, "Calculate ACORN of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "v41_msb",     (PyCFunction)_crc16_xmodem,    METH_VARARGS, "Calculate V-41-MSB of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "lte16",       (PyCFunction)_crc16_xmodem,    METH_VARARGS, "Calculate LTE of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "ccitt_aug",   (PyCFunction)_crc16_ccitt_aug, METH_VARARGS, "Calculate CCITT-AUG of CRC16 [Poly=0x1021, Init=0x1D0F Xorout=0x0000 Refin=False Refout=False]" },
    { "spi_fujitsu", (PyCFunction)_crc16_ccitt_aug, METH_VARARGS, "Calculate SPI-FUJITSU of CRC16 [Poly=0x1021, Init=0x1D0F Xorout=0x0000 Refin=False Refout=False]" },
    { "ccitt_false", (PyCFunction)_crc16_ccitt,     METH_VARARGS, "Calculate CCITT-FALSE of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]" },
    { "ibm_3740",    (PyCFunction)_crc16_ccitt,     METH_VARARGS, "Calculate IBM-3740 of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]" },
    { "autosar16",   (PyCFunction)_crc16_ccitt,     METH_VARARGS, "Calculate AUTOSAR of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]" },
    { "kermit",      (PyCFunction)_crc16_kermit,    METH_VARARGS, "Calculate KERMIT of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "ccitt_true",  (PyCFunction)_crc16_kermit,    METH_VARARGS, "Calculate CCITT-TRUE of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "ccitt",       (PyCFunction)_crc16_kermit,    METH_VARARGS, "Calculate CCITT of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "v41_lsb",     (PyCFunction)_crc16_kermit,    METH_VARARGS, "Calculate V-41-LSB of CRC16 [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "mcrf4xx",     (PyCFunction)_crc16_mcrf4xx,   METH_VARARGS, "Calculate MCRF4XX of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]" },
    { "sick",        (PyCFunction)_crc16_sick,      METH_VARARGS, "Calculate SICK of CRC16 [Poly=0x8005, Init=0x0000]" },
    { "dnp",         (PyCFunction)_crc16_dnp,       METH_VARARGS, "Calculate DNP (Ues:M-Bus, ICE870) of CRC16 [Poly=0x3D65, Init=0x0000 Xorout=0xFFFF Refin=False Refout=False]" },
    { "x25",         (PyCFunction)_crc16_x25,       METH_VARARGS, "Calculate X25 of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]" },
    { "ibm_sdlc",    (PyCFunction)_crc16_x25,       METH_VARARGS, "Calculate IBM-SDLC of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]" },
    { "iso_hdlc16",  (PyCFunction)_crc16_x25,       METH_VARARGS, "Calculate ISO-HDLC of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]" },
    { "iec14443_3_b",(PyCFunction)_crc16_x25,       METH_VARARGS, "Calculate ISO-IEC-14443-3-B of CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]" },
    { "maxim16",     (PyCFunction)_crc16_maxim,     METH_VARARGS, "Calculate MAXIM(MAXIM-DOW) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]" },
    { "dect_r",      (PyCFunction)_crc16_dect_r,    METH_VARARGS, "Calculate DECT-R of CRC16 [Poly=0x0589, Init=0x0000 Xorout=0x0001 Refin=False Refout=False]" },
    { "dect_x",      (PyCFunction)_crc16_dect_x,    METH_VARARGS, "Calculate DECT-X of CRC16 [Poly=0x0589, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "table16",     (PyCFunction)_crc16_table,     METH_VARARGS, "Print CRC16 table to list. libscrc.table16( polynomial )" },
    { "hacker16",    (PyCFunction)_crc16_hacker,    METH_KEYWORDS|METH_VARARGS, "User calculation CRC16\n"
                                                                                "@data   : bytes\n"
                                                                                "@poly   : default=0x8050\n"
                                                                                "@init   : default=0xFFFF\n"
                                                                                "@xorout : default=0x0000\n"
                                                                                "@refin  : default=True\n"
                                                                                "@refout : default=True\n"
                                                                                "@reinit : default=False" },
    { "udp",         (PyCFunction)_crc16_network,    METH_VARARGS, "Calculate UDP checksum." },
    { "tcp",         (PyCFunction)_crc16_network,    METH_VARARGS, "Calculate TCP checksum." },
    { "fletcher16",  (PyCFunction)_crc16_fletcher,   METH_VARARGS, "Calculate FLETCHER16" },
    { "epc16",       (PyCFunction)_crc16_rfid_epc,   METH_VARARGS, "Calculate RFID EPC CRC16 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "profibus",    (PyCFunction)_crc16_profibus,   METH_VARARGS, "Calculate PROFIBUS of CRC16 [Poly=0x1DCF, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "buypass",     (PyCFunction)_crc16_buypass,    METH_VARARGS, "Calculate BUYPASS [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "umts",        (PyCFunction)_crc16_buypass,    METH_VARARGS, "Calculate UMTS [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "verifone",    (PyCFunction)_crc16_buypass,    METH_VARARGS, "Calculate VERIFONE [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "genibus",     (PyCFunction)_crc16_rfid_epc,   METH_VARARGS, "Calculate GENIBUS [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "darc",        (PyCFunction)_crc16_rfid_epc,   METH_VARARGS, "Calculate DARC [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "epc_c1g2",    (PyCFunction)_crc16_rfid_epc,   METH_VARARGS, "Calculate EPC-C1G2 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "icode16",     (PyCFunction)_crc16_rfid_epc,   METH_VARARGS, "Calculate I-CODE16 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "gsm16",       (PyCFunction)_crc16_gsm16,      METH_VARARGS, "Calculate GSM16 [Poly=0x1021, Init=0x0000 Xorout=0xFFFF Refin=False Refout=False]" },
    { "riello",      (PyCFunction)_crc16_riello,     METH_VARARGS, "Calculate RIELLO [Poly=0x1021, Init=0xB2AA Xorout=0x0000 Refin=True Refout=True]" },
    { "crc16_a",     (PyCFunction)_crc16_crc16_a,    METH_VARARGS, "Calculate CRC16-A [Poly=0x1021, Init=0xC6C6 Xorout=0x0000 Refin=True Refout=True]" },
    { "iec14443_3_a",(PyCFunction)_crc16_crc16_a,    METH_VARARGS, "Calculate ISO-IEC-14443-3-A [Poly=0x1021, Init=0xC6C6 Xorout=0x0000 Refin=True Refout=True]" },
    { "cdma2000",    (PyCFunction)_crc16_cdma2000,   METH_VARARGS, "Calculate CDMA2000 [Poly=0xC867, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]" },
    { "teledisk",    (PyCFunction)_crc16_teledisk,   METH_VARARGS, "Calculate TELEDISK [Poly=0xA097, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "tms37157",    (PyCFunction)_crc16_tms37157,   METH_VARARGS, "Calculate TMS37157 [Poly=0x1021, Init=0x89EC Xorout=0x0000 Refin=True Refout=True]" },
    { "en13757",     (PyCFunction)_crc16_en13757,    METH_VARARGS, "Calculate EN13757(Used in the Wireless M-Bus protocol for remote meter reading) [Poly=0x3D65, Init=0x0000 Xorout=0xFFFF Refin=False Refout=False]" },
    { "t10_dif",     (PyCFunction)_crc16_t10_dif,    METH_VARARGS, "Calculate T10-DIF [Poly=0x8BB7, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "dds_110",     (PyCFunction)_crc16_dds_110,    METH_VARARGS, "Calculate DDS-110 [Poly=0x8005, Init=0x800D Xorout=0x0000 Refin=False Refout=False]" },
    { "cms",         (PyCFunction)_crc16_cms,        METH_VARARGS, "Calculate CMS [Poly=0x8005, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]" },
    { "lj1200",      (PyCFunction)_crc16_lj1200,     METH_VARARGS, "Calculate LJ1200 [Poly=0x6F63, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "nrsc5",       (PyCFunction)_crc16_nrsc5,      METH_VARARGS, "Calculate NRSC-5 [Poly=0x080B, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]" },
    
    { "opensafety_a",   (PyCFunction)_crc16_opensafety_a,   METH_VARARGS, "Calculate OPENSAFETY-A [Poly=0x5935, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "opensafety_b",   (PyCFunction)_crc16_opensafety_b,   METH_VARARGS, "Calculate OPENSAFETY-B [Poly=0x755B, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _crc16_doc,
"Calculation of CRC16 \n"
"libscrc.modbus     -> Calculate Modbus of CRC16              [Poly=0x8005, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.usb16      -> Calculate USB    of CRC16              [Poly=0xA001, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.ibm        -> Calculate IBM (Alias:ARC/LHA) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.xmodem     -> Calculate XMODEM of CRC16              [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.ccitt      -> Calculate CCITT of CRC16               [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.ccitt_true -> Calculate CCITT-TRUE of CRC16          [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.ccitt_aug  -> Calculate CCITT-AUG of CRC16           [Poly=0x1021, Init=0x1D0F Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.ccitt_false-> Calculate CCITT-FALSE of CRC16         [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.ibm_3740   -> Calculate IBM-3740 of CRC16            [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.autosar16  -> Calculate AUTOSAR of CRC16             [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.kermit     -> Calculate KERMIT (CCITT-TRUE)          [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.mcrf4xx    -> Calculate MCRF4XX of CRC16             [Poly=0x1021, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.sick       -> Calculate SICK of CRC16                [Poly=0x8005, Init=0x0000]\n"
"libscrc.dnp        -> Calculate DNP (Ues:M-Bus, ICE870)      [Poly=0x3D65, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.x25        -> Calculate X25 of CRC16                 [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.ibm_sdlc   -> Calculate IBM-SDLC of CRC16            [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.iso_hdlc16 -> Calculate ISO-HDLC of CRC16            [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.iec14443_3_a -> Calculate ISO-IEC-14443-3-A          [Poly=0x1021, Init=0xC6C6 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.iec14443_3_b -> Calculate ISO-IEC-14443-3-B          [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.maxim16    -> Calculate MAXIM of CRC16               [Poly=0x8005, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.dect_r     -> Calculate DECT-R of CRC16              [Poly=0x0589, Init=0x0000 Xorout=0x0001 Refin=False Refout=False]\n"
"libscrc.dect_x     -> Calculate DECT-X of CRC16              [Poly=0x0589, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.hacker16   -> Free calculation CRC16 @reinit reinitialize the crc16 tables\n"
"libscrc.fletcher16 -> Calculate FLETCHER16\n"
"libscrc.epc16      -> Calculate RFID EPC of CRC16           [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.profibus   -> Calculate PROFIBUS [Poly=0x1DCF, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.buypass    -> Calculate BUYPASS  [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.genibus    -> Calculate GENIBUS  [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.darc       -> Calculate DARC     [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.epc_c1g2   -> Calculate EPC-C1-G2  [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.icode16    -> Calculate I-CODE   [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.gsm16      -> Calculate GSM-16   [Poly=0x1021, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.riello     -> Calculate RIELLO   [Poly=0x1021, Init=0xB2AA Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.crc16_a    -> Calculate CRC16-A  [Poly=0x1021, Init=0xC6C6 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.cdma2000   -> Calculate CDMA2000 [Poly=0xC867, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.teledisk   -> Calculate TELEDISK [Poly=0xA097, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.tms37157   -> Calculate TMS37157 [Poly=0x1021, Init=0x89EC Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.en13757    -> Calculate EN13757(Used in the Wireless M-Bus protocol for remote meter reading) [Poly=0x3D65, Init=0x0000 Xorout=0xFFFF Refin=False Refout=False]\n"
"libscrc.t10_dif    -> Calculate T10-DIF [Poly=0x8BB7, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.dds_110    -> Calculate DDS-110 [Poly=0x8005, Init=0x800D Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.cms        -> Calculate CMS     [Poly=0x8005, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.lj1200     -> Calculate LJ1200  [Poly=0x6F63, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.nrsc5      -> Calculate NRSC-5  [Poly=0x080B, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.umts       -> Calculate UMTS    [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.v41_lsb    -> Calculate V-41-LSB         [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.v41_msb    -> Calculate V-41-MSB         [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.opensafety_a   -> Calculate OPENSAFETY-A [Poly=0x5935, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.opensafety_b   -> Calculate OPENSAFETY-B [Poly=0x5935, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc16module = {
   PyModuleDef_HEAD_INIT,
   "_crc16",                    /* name of module */
   _crc16_doc,                  /* module documentation, may be NULL */
   -1,                          /* size of per-interpreter state of the module */
   _crc16Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc16( void )
{
    PyObject *m;

    m = PyModule_Create( &_crc16module );
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "1.7"   );
    PyModule_AddStringConstant( m, "__author__",  "Heyn"  );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc16( void )
{
    (void) Py_InitModule3( "_crc16", _crc16Methods, _crc16_doc );
}

#endif /* PY_MAJOR_VERSION */
