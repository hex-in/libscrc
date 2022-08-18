/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crcxmodule.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized code.
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Update gradually calculate functions.
*                                         ( CRCx -> libscrc.umts12()  libscrc.crc12_3gpp() ).
*                       2022-08-18 [Heyn] Fix Python deprication (PY_SSIZE_T_CLEAN)
*
*********************************************************************************************************
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "_crcxtables.h"

static unsigned char hexin_PyArg_ParseTuple_Paramete( PyObject *self, PyObject *args, struct _hexin_crcx *param )
{
    Py_buffer data = { NULL, NULL };
    unsigned short init = param->init;

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

    param->result = hexin_crcx_compute( (const unsigned char *)data.buf, (unsigned int)data.len, param, init );

    if ( data.obj )
       PyBuffer_Release( &data );

    return TRUE;
}

static PyObject * _crc3_gsm( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc3_param_gsm = { .is_initial=FALSE,
                                                 .width  = 3,
                                                 .poly   = 0x03,
                                                 .init   = 0x00,
                                                 .refin  = FALSE,
                                                 .refout = FALSE,
                                                 .xorout = 0x07,
                                                 .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc3_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc3_param_gsm.result );
}

static PyObject * _crc3_rohc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc3_param_rohc = { .is_initial=FALSE,
                                                  .width  = 3,
                                                  .poly   = 0x03,
                                                  .init   = 0x07,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc3_param_rohc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc3_param_rohc.result );
}

static PyObject * _crc4_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc4_param_itu4 = { .is_initial=FALSE,
                                                  .width  = 4,
                                                  .poly   = 0x03,
                                                  .init   = 0x00,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc4_param_itu4 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc4_param_itu4.result );
}

static PyObject * _crc4_interlaken4( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc4_param_interlaken = { .is_initial=FALSE,
                                                        .width  = 4,
                                                        .poly   = 0x03,
                                                        .init   = 0x0F,
                                                        .refin  = FALSE,
                                                        .refout = FALSE,
                                                        .xorout = 0x0F,
                                                        .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc4_param_interlaken ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc4_param_interlaken.result );
}

static PyObject * _crc5_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc5_param_itu5 = { .is_initial=FALSE,
                                                  .width  = 5,
                                                  .poly   = 0x15,
                                                  .init   = 0x00,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc5_param_itu5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc5_param_itu5.result );
}

static PyObject * _crc5_epc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc5_param_epc5 = { .is_initial=FALSE,
                                                  .width  = 5,
                                                  .poly   = 0x09,
                                                  .init   = 0x09,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc5_param_epc5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc5_param_epc5.result );
}

static PyObject * _crc5_usb( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc5_param_usb5 = { .is_initial=FALSE,
                                                  .width  = 5,
                                                  .poly   = 0x05,
                                                  .init   = 0x1F,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x1F,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc5_param_usb5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc5_param_usb5.result );
}

static PyObject * _crc6_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc6_param_itu6 = { .is_initial=FALSE,
                                                  .width  = 6,
                                                  .poly   = 0x03,
                                                  .init   = 0x00,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc6_param_itu6 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc6_param_itu6.result );
}

static PyObject * _crc6_gsm( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc6_param_gsm6 = { .is_initial=FALSE,
                                                  .width  = 6,
                                                  .poly   = 0x2F,
                                                  .init   = 0x00,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x3F,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc6_param_gsm6 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc6_param_gsm6.result );
}

static PyObject * _crc6_darc6( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc6_param_darc = { .is_initial=FALSE,
                                                  .width  = 6,
                                                  .poly   = 0x19,
                                                  .init   = 0x00,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc6_param_darc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc6_param_darc.result );
}

static PyObject * _crc7_mmc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc7_param_mmc7 = { .is_initial=FALSE,
                                                  .width  = 7,
                                                  .poly   = 0x09,
                                                  .init   = 0x00,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc7_param_mmc7 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc7_param_mmc7.result );
}

static PyObject * _crc7_umts7( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc7_param_umts = { .is_initial=FALSE,
                                                  .width  = 7,
                                                  .poly   = 0x45,
                                                  .init   = 0x00,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc7_param_umts ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc7_param_umts.result );
}

static PyObject * _crc7_rohc7( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc7_param_rohc = { .is_initial=FALSE,
                                                  .width  = 7,
                                                  .poly   = 0x4F,
                                                  .init   = 0x7F,
                                                  .refin  = TRUE,
                                                  .refout = TRUE,
                                                  .xorout = 0x00,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc7_param_rohc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc7_param_rohc.result );
}

static PyObject * _crc10_atm10( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc10_param_atm = { .is_initial=FALSE,
                                                  .width  = 10,
                                                  .poly   = 0x233,
                                                  .init   = 0x000,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x000,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc10_param_atm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc10_param_atm.result );
}

static PyObject * _crc10_cdma2000( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc10_param_cdma2000 = { .is_initial=FALSE,
                                                       .width  = 10,
                                                       .poly   = 0x3D9,
                                                       .init   = 0x3FF,
                                                       .refin  = FALSE,
                                                       .refout = FALSE,
                                                       .xorout = 0x000,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc10_param_cdma2000 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc10_param_cdma2000.result );
}

static PyObject * _crc10_gsm10( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc10_param_gsm10 = { .is_initial=FALSE,
                                                    .width  = 10,
                                                    .poly   = 0x175,
                                                    .init   = 0x000,
                                                    .refin  = FALSE,
                                                    .refout = FALSE,
                                                    .xorout = 0x3FF,
                                                    .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc10_param_gsm10 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc10_param_gsm10.result );
}

static PyObject * _crc11_flexray11( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc11_param_flexray = { .is_initial=FALSE,
                                                      .width  = 11,
                                                      .poly   = 0x385,
                                                      .init   = 0x01A,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x000,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc11_param_flexray ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc11_param_flexray.result );
}

static PyObject * _crc11_umts11( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc11_param_umts11 = { .is_initial=FALSE,
                                                     .width  = 11,
                                                     .poly   = 0x307,
                                                     .init   = 0x000,
                                                     .refin  = FALSE,
                                                     .refout = FALSE,
                                                     .xorout = 0x000,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc11_param_umts11 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc11_param_umts11.result );
}

static PyObject * _crc12_cdma2000( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_cdma2000 = { .is_initial=FALSE,
                                                       .width  = 12,
                                                       .poly   = 0xF13,
                                                       .init   = 0xFFF,
                                                       .refin  = FALSE,
                                                       .refout = FALSE,
                                                       .xorout = 0x000,
                                                       .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_cdma2000 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc12_param_cdma2000.result );
}

static PyObject * _crc12_dect12( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_dect = { .is_initial=FALSE,
                                                   .width  = 12,
                                                   .poly   = 0x80F,
                                                   .init   = 0x000,
                                                   .refin  = FALSE,
                                                   .refout = FALSE,
                                                   .xorout = 0x000,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_dect ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc12_param_dect.result );
}

static PyObject * _crc12_gsm12( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_gsm = { .is_initial=FALSE,
                                                  .width  = 12,
                                                  .poly   = 0xD31,
                                                  .init   = 0x000,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0xFFF,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc12_param_gsm.result );
}

static PyObject * _crc12_umts12( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_umts12 = { .is_initial=FALSE,
                                                     .width  = 12,
                                                     .poly   = 0x80F,
                                                     .init   = 0x000,
                                                     .refin  = FALSE,
                                                     .refout = TRUE,
                                                     .xorout = 0x000,
                                                     .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_umts12 ) ) {
        return NULL;
    }

    /* TODO : refin=False, refout=True
       v1.7 fixed ( removed below code )
    */
    // crc12_param_umts12.result = hexin_crcx_reverse12( crc12_param_umts12.result );

    return Py_BuildValue( "H", crc12_param_umts12.result );
}

static PyObject * _crc13_bbc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc13_param_bbc = { .is_initial=FALSE,
                                                  .width  = 13,
                                                  .poly   = 0x1CF5,
                                                  .init   = 0x0000,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x0000,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc13_param_bbc ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc13_param_bbc.result );
}

static PyObject * _crc14_darc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc14_param_darc = { .is_initial=FALSE,
                                                   .width  = 14,
                                                   .poly   = 0x0805,
                                                   .init   = 0x0000,
                                                   .refin  = TRUE,
                                                   .refout = TRUE,
                                                   .xorout = 0x0000,
                                                   .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc14_param_darc ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc14_param_darc.result );
}

static PyObject * _crc14_gsm( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc14_param_gsm = { .is_initial=FALSE,
                                                  .width  = 14,
                                                  .poly   = 0x202D,
                                                  .init   = 0x0000,
                                                  .refin  = FALSE,
                                                  .refout = FALSE,
                                                  .xorout = 0x3FFF,
                                                  .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc14_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc14_param_gsm.result );
}

static PyObject * _crc15_mpt1327( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc15_param_mpt1327 = { .is_initial=FALSE,
                                                      .width  = 15,
                                                      .poly   = 0x6815,
                                                      .init   = 0x0000,
                                                      .refin  = FALSE,
                                                      .refout = FALSE,
                                                      .xorout = 0x0001,
                                                      .result = 0 };

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc15_param_mpt1327 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc15_param_mpt1327.result );
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
    { "atm10",   _crc10_atm10,       METH_VARARGS, "Calculate ATM of CRC10 [Poly=0x233 Initial=0x000 Refin=False Refout=False Xorout=0x000]"   },
    { "gsm10",   _crc10_gsm10,       METH_VARARGS, "Calculate GSM of CRC10 [Poly=0x175 Initial=0x000 Refin=False Refout=False Xorout=0x3FF]"   },
    { "umts11",  _crc11_umts11,      METH_VARARGS, "Calculate UMTS of CRC11 [Poly=0x307 Initial=0x000 Refin=False Refout=False Xorout=0x000]"   },
    { "dect12",  _crc12_dect12,      METH_VARARGS, "Calculate DECT of CRC12 [Poly=0x80F Initial=0x000 Refin=False Refout=False Xorout=0x000]"   },
    { "gsm12",   _crc12_gsm12,       METH_VARARGS, "Calculate GSM of CRC12 [Poly=0xD31 Initial=0x000 Refin=False Refout=False Xorout=0xFFF]"   },
    { "umts12",  _crc12_umts12,      METH_VARARGS, "Calculate UMTS of CRC12 [Poly=0x80F Initial=0x000 Refin=False Refout=true Xorout=0x000]"   },
    { "crc12",   _crc12_dect12,      METH_VARARGS, "Calculate X-CRC12 of CRC12 [Poly=0x80F Initial=0x000 Refin=False Refout=False Xorout=0x000]"   },
    { "bbc13",   _crc13_bbc,         METH_VARARGS, "Calculate BBC of CRC13 [Poly=0x1CF5 Initial=0x0000 Refin=False Refout=False Xorout=0x0000]"   },
    { "darc14",  _crc14_darc,        METH_VARARGS, "Calculate DARC of CRC14 [Poly=0x0805 Initial=0x0000 Refin=False Refout=true Xorout=0x0000]"   },
    { "gsm14",   _crc14_gsm,         METH_VARARGS, "Calculate GSM of CRC14 [Poly=0x202D Initial=0x0000 Refin=False Refout=False Xorout=0x3FFF]"   },
    
    { "crc12_3gpp",         _crc12_umts12,      METH_VARARGS, "Calculate 3GPP of CRC12 [Poly=0x80F Initial=0x000 Refin=False Refout=true Xorout=0x000]"   },
    { "mpt1327",            _crc15_mpt1327,     METH_VARARGS, "Calculate MPT1327 of CRC15 [Poly=0x6815 Initial=0x0000 Refin=False Refout=False Xorout=0x0001]"   },
    { "flexray11",          _crc11_flexray11,   METH_VARARGS, "Calculate FLEXRAY of CRC11 [Poly=0x385 Initial=0x01a Refin=False Refout=False Xorout=0x000]"   },
    { "crc10_cdma2000",     _crc10_cdma2000,    METH_VARARGS, "Calculate CDMA2000 of CRC10  [Poly=0x3D9 Initial=0x3FF Refin=False Refout=False Xorout=0x000]"   },
    { "crc12_cdma2000",     _crc12_cdma2000,    METH_VARARGS, "Calculate CDMA2000 of CRC12  [Poly=0xF13 Initial=0xFFF Refin=False Refout=False Xorout=0x000]"   },
    { "interlaken4",        _crc4_interlaken4,  METH_VARARGS, "Calculate INTERLAKEN of CRC4 [Poly=0x03 Initial=0x0F Xorout=0x0F Refin=False Refout=False]" },
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

    PyModule_AddStringConstant( m, "__version__", "1.7" );
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
