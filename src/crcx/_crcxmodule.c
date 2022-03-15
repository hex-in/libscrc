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
*                       2020-11-17 [Heyn] Fixed Issues #6 (Python2 vc9 error C2059 )
*                       2021-06-07 [Heyn] Update gradually calculate functions.
*                                         ( CRCx -> libscrc.umts12()  libscrc.crc12_3gpp() ).
*
*********************************************************************************************************
*/

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
    static struct _hexin_crcx crc3_param_gsm;

    crc3_param_gsm.is_initial = FALSE;
    crc3_param_gsm.width      = 3;
    crc3_param_gsm.poly       = 0x03;
    crc3_param_gsm.init       = 0x00;
    crc3_param_gsm.refin      = FALSE;
    crc3_param_gsm.refout     = FALSE;
    crc3_param_gsm.xorout     = 0x07;
    crc3_param_gsm.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc3_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc3_param_gsm.result );
}

static PyObject * _crc3_rohc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc3_param_rohc;

    crc3_param_rohc.is_initial = FALSE;
    crc3_param_rohc.width      = 3;
    crc3_param_rohc.poly       = 0x03;
    crc3_param_rohc.init       = 0x07;
    crc3_param_rohc.refin      = TRUE;
    crc3_param_rohc.refout     = TRUE;
    crc3_param_rohc.xorout     = 0x00;
    crc3_param_rohc.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc3_param_rohc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc3_param_rohc.result );
}

static PyObject * _crc4_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc4_param_itu4;

    crc4_param_itu4.is_initial = FALSE;
    crc4_param_itu4.width      = 4;
    crc4_param_itu4.poly       = 0x03;
    crc4_param_itu4.init       = 0x00;
    crc4_param_itu4.refin      = TRUE;
    crc4_param_itu4.refout     = TRUE;
    crc4_param_itu4.xorout     = 0x00;
    crc4_param_itu4.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc4_param_itu4 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc4_param_itu4.result );
}

static PyObject * _crc4_interlaken4( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc4_param_interlaken;

    crc4_param_interlaken.is_initial = FALSE;
    crc4_param_interlaken.width      = 4;
    crc4_param_interlaken.poly       = 0x03;
    crc4_param_interlaken.init       = 0x0F;
    crc4_param_interlaken.refin      = FALSE;
    crc4_param_interlaken.refout     = FALSE;
    crc4_param_interlaken.xorout     = 0x0F;
    crc4_param_interlaken.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc4_param_interlaken ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc4_param_interlaken.result );
}

static PyObject * _crc5_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc5_param_itu5;

    crc5_param_itu5.is_initial = FALSE;
    crc5_param_itu5.width      = 5;
    crc5_param_itu5.poly       = 0x15;
    crc5_param_itu5.init       = 0x00;
    crc5_param_itu5.refin      = TRUE;
    crc5_param_itu5.refout     = TRUE;
    crc5_param_itu5.xorout     = 0x00;
    crc5_param_itu5.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc5_param_itu5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc5_param_itu5.result );
}

static PyObject * _crc5_epc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc5_param_epc5;

    crc5_param_epc5.is_initial = FALSE;
    crc5_param_epc5.width      = 5;
    crc5_param_epc5.poly       = 0x09;
    crc5_param_epc5.init       = 0x09;
    crc5_param_epc5.refin      = FALSE;
    crc5_param_epc5.refout     = FALSE;
    crc5_param_epc5.xorout     = 0x00;
    crc5_param_epc5.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc5_param_epc5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc5_param_epc5.result );
}

static PyObject * _crc5_usb( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc5_param_usb5;

    crc5_param_usb5.is_initial = FALSE;
    crc5_param_usb5.width      = 5;
    crc5_param_usb5.poly       = 0x05;
    crc5_param_usb5.init       = 0x1F;
    crc5_param_usb5.refin      = TRUE;
    crc5_param_usb5.refout     = TRUE;
    crc5_param_usb5.xorout     = 0x1F;
    crc5_param_usb5.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc5_param_usb5 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc5_param_usb5.result );
}

static PyObject * _crc6_itu( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc6_param_itu6;

    crc6_param_itu6.is_initial = FALSE;
    crc6_param_itu6.width      = 6;
    crc6_param_itu6.poly       = 0x03;
    crc6_param_itu6.init       = 0x00;
    crc6_param_itu6.refin      = TRUE;
    crc6_param_itu6.refout     = TRUE;
    crc6_param_itu6.xorout     = 0x00;
    crc6_param_itu6.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc6_param_itu6 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc6_param_itu6.result );
}

static PyObject * _crc6_gsm( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc6_param_gsm6;

    crc6_param_gsm6.is_initial = FALSE;
    crc6_param_gsm6.width      = 6;
    crc6_param_gsm6.poly       = 0x2F;
    crc6_param_gsm6.init       = 0x00;
    crc6_param_gsm6.refin      = FALSE;
    crc6_param_gsm6.refout     = FALSE;
    crc6_param_gsm6.xorout     = 0x3F;
    crc6_param_gsm6.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc6_param_gsm6 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc6_param_gsm6.result );
}

static PyObject * _crc6_darc6( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc6_param_darc;

    crc6_param_darc.is_initial = FALSE;
    crc6_param_darc.width      = 6;
    crc6_param_darc.poly       = 0x19;
    crc6_param_darc.init       = 0x00;
    crc6_param_darc.refin      = TRUE;
    crc6_param_darc.refout     = TRUE;
    crc6_param_darc.xorout     = 0x00;
    crc6_param_darc.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc6_param_darc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc6_param_darc.result );
}

static PyObject * _crc7_mmc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc7_param_mmc7;

    crc7_param_mmc7.is_initial = FALSE;
    crc7_param_mmc7.width      = 7;
    crc7_param_mmc7.poly       = 0x09;
    crc7_param_mmc7.init       = 0x00;
    crc7_param_mmc7.refin      = FALSE;
    crc7_param_mmc7.refout     = FALSE;
    crc7_param_mmc7.xorout     = 0x00;
    crc7_param_mmc7.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc7_param_mmc7 ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc7_param_mmc7.result );
}

static PyObject * _crc7_umts7( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc7_param_umts;

    crc7_param_umts.is_initial = FALSE;
    crc7_param_umts.width      = 7;
    crc7_param_umts.poly       = 0x45;
    crc7_param_umts.init       = 0x00;
    crc7_param_umts.refin      = FALSE;
    crc7_param_umts.refout     = FALSE;
    crc7_param_umts.xorout     = 0x00;
    crc7_param_umts.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc7_param_umts ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc7_param_umts.result );
}

static PyObject * _crc7_rohc7( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc7_param_rohc;

    crc7_param_rohc.is_initial = FALSE;
    crc7_param_rohc.width      = 7;
    crc7_param_rohc.poly       = 0x4F;
    crc7_param_rohc.init       = 0x7F;
    crc7_param_rohc.refin      = TRUE;
    crc7_param_rohc.refout     = TRUE;
    crc7_param_rohc.xorout     = 0x00;
    crc7_param_rohc.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc7_param_rohc ) ) {
        return NULL;
    }

    return Py_BuildValue( "B", crc7_param_rohc.result );
}

static PyObject * _crc10_atm10( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc10_param_atm;

    crc10_param_atm.is_initial = FALSE;
    crc10_param_atm.width      = 10;
    crc10_param_atm.poly       = 0x233;
    crc10_param_atm.init       = 0x000;
    crc10_param_atm.refin      = FALSE;
    crc10_param_atm.refout     = FALSE;
    crc10_param_atm.xorout     = 0x000;
    crc10_param_atm.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc10_param_atm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc10_param_atm.result );
}

static PyObject * _crc10_cdma2000( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc10_param_cdma2000;

    crc10_param_cdma2000.is_initial = FALSE;
    crc10_param_cdma2000.width      = 10;
    crc10_param_cdma2000.poly       = 0x3D9;
    crc10_param_cdma2000.init       = 0x3FF;
    crc10_param_cdma2000.refin      = FALSE;
    crc10_param_cdma2000.refout     = FALSE;
    crc10_param_cdma2000.xorout     = 0x000;
    crc10_param_cdma2000.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc10_param_cdma2000 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc10_param_cdma2000.result );
}

static PyObject * _crc10_gsm10( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc10_param_gsm10;

    crc10_param_gsm10.is_initial = FALSE;
    crc10_param_gsm10.width      = 10;
    crc10_param_gsm10.poly       = 0x175;
    crc10_param_gsm10.init       = 0x000;
    crc10_param_gsm10.refin      = FALSE;
    crc10_param_gsm10.refout     = FALSE;
    crc10_param_gsm10.xorout     = 0x3FF;
    crc10_param_gsm10.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc10_param_gsm10 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc10_param_gsm10.result );
}

static PyObject * _crc11_flexray11( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc11_param_flexray;

    crc11_param_flexray.is_initial = FALSE;
    crc11_param_flexray.width      = 11;
    crc11_param_flexray.poly       = 0x385;
    crc11_param_flexray.init       = 0x01A;
    crc11_param_flexray.refin      = FALSE;
    crc11_param_flexray.refout     = FALSE;
    crc11_param_flexray.xorout     = 0x000;
    crc11_param_flexray.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc11_param_flexray ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc11_param_flexray.result );
}

static PyObject * _crc11_umts11( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc11_param_umts11;

    crc11_param_umts11.is_initial = FALSE;
    crc11_param_umts11.width      = 11;
    crc11_param_umts11.poly       = 0x307;
    crc11_param_umts11.init       = 0x000;
    crc11_param_umts11.refin      = FALSE;
    crc11_param_umts11.refout     = FALSE;
    crc11_param_umts11.xorout     = 0x000;
    crc11_param_umts11.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc11_param_umts11 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc11_param_umts11.result );
}

static PyObject * _crc12_cdma2000( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_cdma2000;

    crc12_param_cdma2000.is_initial = FALSE;
    crc12_param_cdma2000.width      = 12;
    crc12_param_cdma2000.poly       = 0xF13;
    crc12_param_cdma2000.init       = 0xFFF;
    crc12_param_cdma2000.refin      = FALSE;
    crc12_param_cdma2000.refout     = FALSE;
    crc12_param_cdma2000.xorout     = 0x000;
    crc12_param_cdma2000.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_cdma2000 ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc12_param_cdma2000.result );
}

static PyObject * _crc12_dect12( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_dect;

    crc12_param_dect.is_initial = FALSE;
    crc12_param_dect.width      = 12;
    crc12_param_dect.poly       = 0x80F;
    crc12_param_dect.init       = 0x000;
    crc12_param_dect.refin      = FALSE;
    crc12_param_dect.refout     = FALSE;
    crc12_param_dect.xorout     = 0x000;
    crc12_param_dect.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_dect ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc12_param_dect.result );
}

static PyObject * _crc12_gsm12( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_gsm;

    crc12_param_gsm.is_initial = FALSE;
    crc12_param_gsm.width      = 12;
    crc12_param_gsm.poly       = 0xD31;
    crc12_param_gsm.init       = 0x000;
    crc12_param_gsm.refin      = FALSE;
    crc12_param_gsm.refout     = FALSE;
    crc12_param_gsm.xorout     = 0xFFF;
    crc12_param_gsm.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc12_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc12_param_gsm.result );
}

static PyObject * _crc12_umts12( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc12_param_umts12;

    crc12_param_umts12.is_initial = FALSE;
    crc12_param_umts12.width      = 12;
    crc12_param_umts12.poly       = 0x80F;
    crc12_param_umts12.init       = 0x000;
    crc12_param_umts12.refin      = FALSE;
    crc12_param_umts12.refout     = TRUE;
    crc12_param_umts12.xorout     = 0x000;
    crc12_param_umts12.result     = 0;

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
    static struct _hexin_crcx crc13_param_bbc;

    crc13_param_bbc.is_initial = FALSE;
    crc13_param_bbc.width      = 13;
    crc13_param_bbc.poly       = 0x1CF5;
    crc13_param_bbc.init       = 0x0000;
    crc13_param_bbc.refin      = FALSE;
    crc13_param_bbc.refout     = FALSE;
    crc13_param_bbc.xorout     = 0x0000;
    crc13_param_bbc.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc13_param_bbc ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc13_param_bbc.result );
}

static PyObject * _crc14_darc( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc14_param_darc;

    crc14_param_darc.is_initial = FALSE;
    crc14_param_darc.width      = 14;
    crc14_param_darc.poly       = 0x0805;
    crc14_param_darc.init       = 0x0000;
    crc14_param_darc.refin      = TRUE;
    crc14_param_darc.refout     = TRUE;
    crc14_param_darc.xorout     = 0x0000;
    crc14_param_darc.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc14_param_darc ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc14_param_darc.result );
}

static PyObject * _crc14_gsm( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc14_param_gsm;

    crc14_param_gsm.is_initial = FALSE;
    crc14_param_gsm.width      = 14;
    crc14_param_gsm.poly       = 0x202D;
    crc14_param_gsm.init       = 0x0000;
    crc14_param_gsm.refin      = FALSE;
    crc14_param_gsm.refout     = FALSE;
    crc14_param_gsm.xorout     = 0x3FFF;
    crc14_param_gsm.result     = 0;

    if ( !hexin_PyArg_ParseTuple_Paramete( self, args, &crc14_param_gsm ) ) {
        return NULL;
    }

    return Py_BuildValue( "H", crc14_param_gsm.result );
}

static PyObject * _crc15_mpt1327( PyObject *self, PyObject *args )
{
    static struct _hexin_crcx crc15_param_mpt1327;

    crc15_param_mpt1327.is_initial = FALSE;
    crc15_param_mpt1327.width      = 15;
    crc15_param_mpt1327.poly       = 0x6815;
    crc15_param_mpt1327.init       = 0x0000;
    crc15_param_mpt1327.refin      = FALSE;
    crc15_param_mpt1327.refout     = FALSE;
    crc15_param_mpt1327.xorout     = 0x0001;
    crc15_param_mpt1327.result     = 0;

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
