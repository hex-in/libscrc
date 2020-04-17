# -*- coding:utf-8 -*-
""" Test library for CRC16 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC16 Module.
# Package:  pip install libscrc.
# History:  2017-08-17 Wheel Ver:0.0.3 [Heyn] Initialize

import unittest

import libscrc
from libscrc import _crc16

class TestCRC16( unittest.TestCase ):
    """ Test CRC16 variant.
    """

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.x25(b'123456789'),         0x906E )
        self.assertEqual(module.xmodem(b'123456789'),       0x31C3 )
        self.assertEqual( module.ccitt(b'123456789'),       0x2189 )
        self.assertEqual( module.kermit(b'123456789'),      0x2189 )
        self.assertEqual( module.mcrf4xx(b'123456789'),     0x6F91 )
        self.assertEqual( module.ccitt_false(b'123456789'), 0x29B1 )
        self.assertEqual( module.sick(b'123456789'),        0xA656 )
        self.assertEqual( module.dnp(b'123456789'),         0xEA82 )
        self.assertEqual( module.ibm(b'123456789'),         0xBB3D )
        self.assertEqual( module.modbus(b'123456789'),      0x4B37 )
        self.assertEqual( module.usb16(b'123456789'),       0xB4C8 )
        self.assertEqual( module.maxim16(b'123456789'),     0x44C2 )
        self.assertEqual( module.dect(b'123456789'),        0xCAB0 )
        self.assertEqual( module.udp(b'\x45\x00\x00\x3c\x00\x00\x00\x00\x40\x11\x00\x00\xc0\xa8\x2b\xc3\x08\x08\x08\x08\x11'), 0x366D )
        self.assertEqual( module.fletcher16(b'123456789'),  0x1EDE )
        self.assertEqual( module.profibus(b'123456789'),    0xA819 )

        # the same in two steps
        crc = module.x25( b'12345' )
        crc = module.x25( b'6789', crc )
        self.assertNotEqual( crc, 0x906E )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics( libscrc )


    def test_basics_c( self ):
        """ Test basic functionality of the extension module.
        """
        self.do_basics( _crc16 )


if __name__ == '__main__':
    unittest.main()
