# -*- coding:utf8 -*-
""" Test library for CRC16 """
# !/usr/bin/python
# Python:   3.5.2
# Platform: Windows/Linux/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC16 Module.
# History:  2017-08-17 Wheel Ver:0.0.3 [Heyn] Initialize

import unittest
import libscrc

from libscrc import _crc16

class TestCRC16Modbus(unittest.TestCase):
    """Test CRC16 Modbus variant.
    """

    def do_basics(self, module):
        """Test basic functionality.
        """
        # very basic example
        self.assertEqual(module.x25(b'123456789'), 0x906e)
        self.assertEqual(module.xmodem(b'123456789'), 0x31C3)
        self.assertEqual(module.ccitt(b'123456789'), 0x2189)
        self.assertEqual(module.kermit(b'123456789'), 0x2189)
        self.assertEqual(module.ccitt_false(b'123456789'), 0x29B1)
        self.assertEqual(module.sick(b'123456789'), 0xA656)
        self.assertEqual(module.dnp(b'123456789'), 0xEA82)
        self.assertEqual(module.ibm(b'123456789'), 0xBB3D)
        self.assertEqual(module.modbus(b'123456789'), 0x4B37)
        self.assertEqual(module.usb16(b'123456789'), 0xB4C8)
        self.assertEqual(module.maxim16(b'123456789'), 0x44C2)
        self.assertEqual(module.dect(b'123456789'), 0xCAB0)

        # the same in two steps
        crc = module.x25(b'12345')
        crc = module.x25(b'6789', crc)
        self.assertNotEqual(crc, 0x906e)

    def test_basics(self):
        """Test basic functionality.
        """
        self.do_basics(libscrc)


    def test_basics_c(self):
        """Test basic functionality of the extension module.
        """
        self.do_basics(_crc16)


if __name__ == '__main__':
    unittest.main()
