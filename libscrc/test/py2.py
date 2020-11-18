# -*- coding:utf-8 -*-
""" Test library for CRC """
# !/usr/bin/python
# Python:   2.7.18
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC Module.
# Package:  pip install libscrc.
# History:  2020-11-18 Wheel Ver:1.5 [Heyn] Initialize


import libscrc
import libscrc as module

def assertEqual( data, crc ):
    assert data == crc


def hexinTestCRCx( ):
    print( 'TEST CRCx' )
    assertEqual( module.gsm3(b'123456789'),     0x04 )
    assertEqual( module.rohc3(b'123456789'),    0x06 )
    assertEqual( module.itu4(b'123456789'),     0x07 )
    assertEqual( module.g_704(b'123456789'),    0x07 )
    assertEqual( module.itu5(b'123456789'),     0x07 )
    assertEqual( module.epc(b'123456789'),      0x00 )
    assertEqual( module.usb5(b'123456789'),     0x19 )
    assertEqual( module.itu6(b'123456789'),     0x06 )
    assertEqual( module.gsm6(b'123456789'),     0x13 )
    assertEqual( module.darc6(b'123456789'),    0x26 )

    assertEqual( module.mmc(b'123456789'),      0x75 )
    assertEqual( module.crc7(b'123456789'),     0x75 )
    assertEqual( module.umts7(b'123456789'),    0x61 )
    assertEqual( module.rohc7(b'123456789'),    0x53 )

    assertEqual( module.interlaken4(b'123456789'),         0x0B )

    assertEqual( module.atm10(b'123456789'),               0x199 )
    assertEqual( module.gsm10(b'123456789'),               0x12A )
    assertEqual( module.umts11(b'123456789'),              0x061 )
    assertEqual( module.umts12(b'123456789'),              0xDAF )
    assertEqual( module.crc12_3gpp(b'123456789'),          0xDAF )
    assertEqual( module.flexray11(b'123456789'),           0x5A3 )
    assertEqual( module.dect12(b'123456789'),              0xF5B )
    assertEqual( module.crc12(b'123456789'),               0xF5B )
    assertEqual( module.gsm12(b'123456789'),               0xB34 )

    assertEqual( module.crc10_cdma2000(b'123456789'),      0x233 )
    assertEqual( module.crc12_cdma2000(b'123456789'),      0xD4D )

    assertEqual( module.bbc13(b'123456789'),               0x04FA )
    assertEqual( module.darc14(b'123456789'),              0x082D )
    assertEqual( module.gsm14(b'123456789'),               0x30AE )
    assertEqual( module.mpt1327(b'123456789'),             0x2566 )

def hexinTestCANx( ):
    print( 'TEST CANx' )
    assertEqual( module.can15(b'123456789'),   0x059E  )
    assertEqual( module.can17(b'123456789'),   0x04F03 )
    assertEqual( module.can21(b'123456789'),   0xED841 )

def hexinTestCRC8( ):
    print( 'TEST CRC8' )
    crc = module.maxim8( b'12345' )
    crc = module.maxim8( b'6789', crc )
    assertEqual( crc, 0xA1 )

    crc = module.crc8( b'12345' )
    crc = module.crc8( b'6789', crc )
    assertEqual( crc, 0xF4 )

    assertEqual( module.rohc(  b'6789', module.rohc(   b'12345') ), 0xD0 )
    assertEqual( module.sum8(  b'6789', module.sum8(   b'12345') ), 0xDD )
    assertEqual( module.bcc(   b'6789', module.bcc(    b'12345') ), 0x31 )
    assertEqual( module.itu8(  b'6789', module.itu8(   b'12345') ), 0xA1 )
    assertEqual( module.smbus( b'6789', module.smbus(  b'12345') ), 0xF4 )
    assertEqual( module.lte8(  b'6789', module.lte8(   b'12345') ), 0xEA )
    assertEqual( module.ebu8(  b'6789', module.ebu8(   b'12345') ), 0x97 )
    assertEqual( module.darc8( b'6789', module.darc8(  b'12345') ), 0x15 )
    assertEqual( module.aes8(  b'6789', module.aes8(   b'12345') ), 0x97 )

    assertEqual( module.icode8(b'6789', module.icode8( b'12345') ), 0x7E )
    assertEqual( module.gsm8_a(b'6789', module.gsm8_a( b'12345') ), 0x37 )
    assertEqual( module.gsm8_b(b'6789', module.gsm8_b( b'12345') ), 0x94 )
    assertEqual( module.nrsc_5(b'6789', module.nrsc_5( b'12345') ), 0xF7 )
    assertEqual( module.wcdma8(b'6789', module.wcdma8( b'12345') ), 0x25 )
    assertEqual( module.dvb_s2(b'6789', module.dvb_s2( b'12345') ), 0xBC )

    assertEqual( module.autosar8(  b'6789', module.autosar8(   b'12345') ), 0xDF )
    assertEqual( module.sae_j1850( b'6789', module.sae_j1850(  b'12345') ), 0x4B )
    assertEqual( module.bluetooth( b'6789', module.bluetooth(  b'12345') ), 0x26 )
    assertEqual( module.mifare_mad(b'6789', module.mifare_mad( b'12345') ), 0x99 )
    assertEqual( module.tech_3250( b'6789', module.tech_3250(  b'12345') ), 0x97 )

    assertEqual( module.opensafety8(b'6789', module.opensafety8(b'12345') ), 0x3E )

def hexinTestCRC16():
    print( 'TEST CRC16' )

    crc = libscrc.xmodem( b'12345' )
    crc = libscrc.xmodem( b'6789', crc )
    assert crc == 0x31C3

    assertEqual( module.x25(           b'6789', module.x25(        b'12345') ), 0x906E )
    assertEqual( module.x25(           b'6789', module.x25(        b'12345') ), 0x906E )
    assertEqual( module.ccitt_aug(     b'6789', module.ccitt_aug(  b'12345') ), 0xE5CC )
    assertEqual( module.kermit(        b'6789', module.kermit(     b'12345') ), 0x2189 )
    assertEqual( module.mcrf4xx(       b'6789', module.mcrf4xx(    b'12345') ), 0x6F91 )
    assertEqual( module.ccitt_false(   b'6789', module.ccitt_false(b'12345') ), 0x29B1 )
    assertEqual( module.dnp(           b'6789', module.dnp(        b'12345') ), 0xEA82 )
    assertEqual( module.ibm(           b'6789', module.ibm(        b'12345') ), 0xBB3D )
    assertEqual( module.modbus(        b'6789', module.modbus(     b'12345') ), 0x4B37 )
    assertEqual( module.usb16(         b'6789', module.usb16(      b'12345') ), 0xB4C8 )
    assertEqual( module.maxim16(       b'6789', module.maxim16(    b'12345') ), 0x44C2 )
    assertEqual( module.dect_r(        b'6789', module.dect_r(     b'12345') ), 0x007E )
    assertEqual( module.dect_x(        b'6789', module.dect_x(     b'12345') ), 0x007F )
    assertEqual( module.profibus(      b'6789', module.profibus(   b'12345') ), 0xA819 )
    assertEqual( module.buypass(       b'6789', module.buypass(    b'12345') ), 0xFEE8 )
    assertEqual( module.genibus(       b'6789', module.genibus(    b'12345') ), 0xD64E )
    assertEqual( module.gsm16(         b'6789', module.gsm16(      b'12345') ), 0xCE3C )
    assertEqual( module.riello(        b'6789', module.riello(     b'12345') ), 0x63D0 )
    assertEqual( module.crc16_a(       b'6789', module.crc16_a(    b'12345') ), 0xBF05 )
    assertEqual( module.cdma2000(      b'6789', module.cdma2000(   b'12345') ), 0x4C06 )
    assertEqual( module.teledisk(      b'6789', module.teledisk(   b'12345') ), 0x0FB3 )
    assertEqual( module.tms37157(      b'6789', module.tms37157(   b'12345') ), 0x26B1 )
    assertEqual( module.cdma2000(      b'6789', module.cdma2000(   b'12345') ), 0x4C06 )
    assertEqual( module.t10_dif(       b'6789', module.t10_dif(    b'12345') ), 0xD0DB )
    assertEqual( module.dds_110(       b'6789', module.dds_110(    b'12345') ), 0x9ECF )
    assertEqual( module.lj1200(        b'6789', module.lj1200(     b'12345') ), 0xBDF4 )
    assertEqual( module.cms(           b'6789', module.cms(        b'12345') ), 0xAEE7 )
    assertEqual( module.nrsc5(         b'6789', module.nrsc5(      b'12345') ), 0xA066 )
    assertEqual( module.en13757(       b'6789', module.en13757(    b'12345') ), 0xC2B7 )

    assertEqual( module.opensafety_a(  b'6789', module.opensafety_a(b'12345') ), 0x5D38 )
    assertEqual( module.opensafety_b(  b'6789', module.opensafety_b(b'12345') ), 0x20FE )

def hexinTestCRC24():
    print( 'TEST CRC24' )

    crc = module.ble( b'12345' )
    crc = module.ble( b'6789', crc )
    assertEqual( crc, 0xC25A56 )

    assertEqual( module.flexraya(b'6789', module.flexraya(b'12345') ), 0x7979BD )
    assertEqual( module.flexrayb(b'6789', module.flexrayb(b'12345') ), 0x1F23B8 )

    assertEqual( module.crc24(b'6789', module.crc24(b'12345') ), 0x21CF02 )
    assertEqual( module.lte_a(b'6789', module.lte_a(b'12345') ), 0xCDE703 )
    assertEqual( module.lte_b(b'6789', module.lte_b(b'12345') ), 0x23EF52 )

    assertEqual( module.interlaken(b'6789', module.interlaken(b'12345') ), 0xB4F3E6 )

def hexinTestCRC32():
    print( 'TEST CRC32' )

    crc = module.mpeg2( b'12345' )
    crc = module.mpeg2( b'6789', crc )
    assertEqual( crc, 0x0376E6E7 )

    crc = module.crc32( b'12345' )
    crc = module.crc32( b'6789', crc )
    assertEqual( crc, 0xCBF43926 )

    assertEqual( module.posix(   b'6789', module.posix(   b'12345') ), 0x765E7680 )
    assertEqual( module.bzip2(   b'6789', module.bzip2(   b'12345') ), 0xFC891918 )
    assertEqual( module.jamcrc(  b'6789', module.jamcrc(  b'12345') ), 0x340BC6D9 )
    assertEqual( module.autosar( b'6789', module.autosar( b'12345') ), 0x1697D06A )
    assertEqual( module.crc32_c( b'6789', module.crc32_c( b'12345') ), 0xE3069283 )
    assertEqual( module.crc32_d( b'6789', module.crc32_d( b'12345') ), 0x87315576 )
    assertEqual( module.crc32_q( b'6789', module.crc32_q( b'12345') ), 0x3010BF7F )
    assertEqual( module.xfer(    b'6789', module.xfer(    b'12345') ), 0xBD0BE338 )
    assertEqual( module.cdma(    b'6789', module.cdma(    b'12345') ), 0x04C34ABF )
    assertEqual( module.philips( b'6789', module.philips( b'12345') ), 0x0CE9E46C )

def hexinTestCRC64():
    print( 'TEST CRC64' )

    assertEqual( module.gsm40(b'123456789'),   0xD4164FC646 )
    assertEqual( module.iso(b'123456789'),     0xB90956C775A41001 )
    assertEqual( module.ecma182(b'123456789'), 0x6C40DF5F0B497347 )
    assertEqual( module.we(b'123456789'),      0x62EC59E3F1A4F00A )
    assertEqual( module.xz64(b'123456789'),    0x995DC9BBDF1939FA )
    # assertEqual( module.hacker64(b'123456789', poly=0x42F0E1EBA9EA3693, init=0, xorout=0, refin=False, refout=False), 0x6C40DF5F0B497347 )

    assertEqual( module.iso(b'A' * 16 * 1024 * 1024),     0x799F69518B336624 )
    assertEqual( module.ecma182(b'A' * 16 * 1024 * 1024), 0xF8A0D6AC4D42F9A6 )

def hexinTestHacker():
    print( 'TEST HACKER' )
    assertEqual( module.hacker16( data=b'123456789', poly=0xA001 ), 0x3D7B )
    assertEqual( module.hacker16( data=b'123456789', poly=0x8408, xorout=0xFFFF ), 0xE245 )
    assertEqual( module.hacker16( data=b'123456789', poly=0x8005, init=0xFFFF, xorout=0x0000, refin=True, refout=True ),   0x4B37 )

    assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7 ), 0xCBF43926 )
    assertEqual( module.hacker32( data=b'123456789', poly=0xEDB88320, xorout=0xFFFFFFFF ), 0xFC4F2BE9 )
    assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF, xorout=0xFFFFFFFF, refin=True, refout=True ), 0xCBF43926 )



hexinTestCRCx( )
hexinTestCANx( )
hexinTestCRC8( )
hexinTestCRC16()
hexinTestCRC24()
hexinTestCRC32()
hexinTestCRC64()

hexinTestHacker()

print( 'TEST DONE' )
