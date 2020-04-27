# -*- coding:utf-8 -*-
""" Test library for CRC64 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC64 Module.
# Package:  pip install libscrc.
# History:  2017-08-19 Wheel Ver:0.0.5 [Heyn] Initialize
#           2020-03-16 Wheel Ver:0.1.6 [Heyn] New add libscrc.hacker64()
#           2020-04-23 Wheel Ver:1.1   [Heyn] Bug fixed.

import unittest

import libscrc
from libscrc import _crc64

class TestCRC64( unittest.TestCase ):
    """ Test CRC64 IOS&ECMA182 variant.
    """

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.gsm40(b'123456789'),   0xD4164FC646 )

        self.assertEqual( module.iso(b'123456789'),     0xB90956C775A41001 )
        self.assertNotEqual( module.iso(b'123456'),     0x288A5BEFFE4CB000 )

        self.assertEqual( module.ecma182(b'123456789'), 0x6C40DF5F0B497347 )
        self.assertNotEqual( module.ecma182(b'123456'), 0x4156683F2F4DCB0E )

        self.assertEqual( module.we(b'123456789'),      0x62EC59E3F1A4F00A )
        self.assertEqual( module.xz64(b'123456789'),    0x995DC9BBDF1939FA )

        self.assertEqual( module.hacker64(b'123456789', poly=0x42F0E1EBA9EA3693, init=0, xorout=0, refin=False, refout=False), 0x6C40DF5F0B497347 )


    def test_basics( self ):
        """Test basic functionality.
        """
        self.do_basics( libscrc )


    def test_basics_c( self ):
        """ Test basic functionality of the extension module.
        """
        self.do_basics( _crc64 )

    def test_big_chunks(self):
        """ Test calculation of CRC on big chunks of data.
        """
        self.assertEqual( _crc64.iso(b'A' * 16 * 1024 * 1024),     0x799F69518B336624 )
        self.assertEqual( _crc64.ecma182(b'A' * 16 * 1024 * 1024), 0xF8A0D6AC4D42F9A6 )


if __name__ == '__main__':
    unittest.main()
