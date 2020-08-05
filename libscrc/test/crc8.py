# -*- coding:utf-8 -*-
""" Test library for CRC8 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC8 Module.
# Package:  pip install libscrc.
# History:  2017-09-21 Wheel Ver:0.1.3 [Heyn] Initialize
#           2020-03-13 Wheel Ver:0.1.6 [Heyn] optimized code
#           2020-08-05 Wheel Ver:1.4   [Heyn] New add gradually calculating

import unittest

import libscrc
from libscrc import _crc8

class TestCRC8( unittest.TestCase ):
    """ Test CRC8 variant.
    """
    def do_gradually( self , module ):
        """ Test gradually calculating functionality.
            (libscrc >= 1.4)

            ##### Gradually calculate invalid functions #####
            libscrc.intel()
            libscrc.lrc()
        """
        crc = module.maxim8( b'12345' )
        crc = module.maxim8( b'6789', crc )
        self.assertEqual( crc, 0xA1 )

        crc = module.crc8( b'12345' )
        crc = module.crc8( b'6789', crc )
        self.assertEqual( crc, 0xF4 )

        self.assertEqual( module.rohc(  b'6789', module.rohc(   b'12345') ), 0xD0 )
        self.assertEqual( module.sum8(  b'6789', module.sum8(   b'12345') ), 0xDD )
        self.assertEqual( module.bcc(   b'6789', module.bcc(    b'12345') ), 0x31 )
        self.assertEqual( module.itu8(  b'6789', module.itu8(   b'12345') ), 0xA1 )
        self.assertEqual( module.smbus( b'6789', module.smbus(  b'12345') ), 0xF4 )
        self.assertEqual( module.lte8(  b'6789', module.lte8(   b'12345') ), 0xEA )
        self.assertEqual( module.ebu8(  b'6789', module.ebu8(   b'12345') ), 0x97 )
        self.assertEqual( module.darc8( b'6789', module.darc8(  b'12345') ), 0x15 )
        self.assertEqual( module.aes8(  b'6789', module.aes8(   b'12345') ), 0x97 )

        self.assertEqual( module.icode8(b'6789', module.icode8( b'12345') ), 0x7E )
        self.assertEqual( module.gsm8_a(b'6789', module.gsm8_a( b'12345') ), 0x37 )
        self.assertEqual( module.gsm8_b(b'6789', module.gsm8_b( b'12345') ), 0x94 )
        self.assertEqual( module.nrsc_5(b'6789', module.nrsc_5( b'12345') ), 0xF7 )
        self.assertEqual( module.wcdma8(b'6789', module.wcdma8( b'12345') ), 0x25 )
        self.assertEqual( module.dvb_s2(b'6789', module.dvb_s2( b'12345') ), 0xBC )

        self.assertEqual( module.autosar8(  b'6789', module.autosar8(   b'12345') ), 0xDF )
        self.assertEqual( module.sae_j1850( b'6789', module.sae_j1850(  b'12345') ), 0x4B )
        self.assertEqual( module.bluetooth( b'6789', module.bluetooth(  b'12345') ), 0x26 )
        self.assertEqual( module.mifare_mad(b'6789', module.mifare_mad( b'12345') ), 0x99 )
        self.assertEqual( module.tech_3250( b'6789', module.tech_3250(  b'12345') ), 0x97 )

        self.assertEqual( module.opensafety8(b'6789', module.opensafety8(b'12345') ), 0x3E )

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.maxim8(b'123456789'),      0xA1 )
        self.assertEqual( module.rohc(b'123456789'),        0xD0 )
        self.assertEqual( module.crc8(b'123456789'),        0xF4 )
        self.assertEqual( module.sum8(b'123456789'),        0xDD )
        self.assertEqual( module.intel(b'123456789'),       0x23 )
        self.assertEqual( module.bcc(b'123456789'),         0x31 )
        self.assertEqual( module.lrc(b'123456789'),         0x23 )
        self.assertEqual( module.itu8(b'123456789'),        0xA1 )
        self.assertEqual( module.i432_1(b'123456789'),      0xA1 )
        self.assertEqual( module.smbus(b'123456789'),       0xF4 )
        self.assertEqual( module.autosar8(b'123456789'),    0xDF )
        self.assertEqual( module.lte8(b'123456789'),        0xEA )
        self.assertEqual( module.sae_j1850(b'123456789'),   0x4B )
        self.assertEqual( module.icode8(b'123456789'),      0x7E )
        self.assertEqual( module.gsm8_a(b'123456789'),      0x37 )
        self.assertEqual( module.gsm8_b(b'123456789'),      0x94 )
        self.assertEqual( module.nrsc_5(b'123456789'),      0xF7 )
        self.assertEqual( module.wcdma8(b'123456789'),      0x25 )
        self.assertEqual( module.bluetooth(b'123456789'),   0x26 )
        self.assertEqual( module.dvb_s2(b'123456789'),      0xBC )
        self.assertEqual( module.ebu8(b'123456789'),        0x97 )
        self.assertEqual( module.darc8(b'123456789'),       0x15 )
        self.assertEqual( module.opensafety8(b'123456789'), 0x3E )
        self.assertEqual( module.mifare_mad(b'123456789'),  0x99 )

        self.assertEqual( module.tech_3250(b'123456789'),   0x97 )
        self.assertEqual( module.aes8(b'123456789'),        0x97 )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics( libscrc )
        self.do_gradually( libscrc )

    def test_basics_c( self ):
        """Test basic functionality of the extension module.
        """
        self.do_gradually( _crc8 )
        self.do_basics( _crc8 )

if __name__ == '__main__':
    unittest.main()
