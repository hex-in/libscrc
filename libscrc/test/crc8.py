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

import unittest

import libscrc
from libscrc import _crc8

class TestCRC8( unittest.TestCase ):
    """ Test CRC8 variant.
    """

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
        self.assertEqual( module.wcdma8(b'123456789'),       0x25 )
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


    def test_basics_c( self ):
        """Test basic functionality of the extension module.
        """
        self.do_basics( _crc8 )


if __name__ == '__main__':
    unittest.main()
