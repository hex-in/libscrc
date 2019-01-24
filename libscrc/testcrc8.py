# -*- coding:utf8 -*-
""" Test library for CRC8 """
# !/usr/bin/python
# Python:   3.5.2
# Platform: Windows/Linux/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC8 Module.
# History:  2017-09-21 Wheel Ver:0.1.3 [Heyn] Initialize

import unittest
import libscrc

from libscrc import _crc8

class TestCRC16Modbus(unittest.TestCase):
    """Test CRC8 variant.
    """

    def do_basics(self, module):
        """Test basic functionality.
        """
        # very basic example
        self.assertEqual(module.maxim8(b'123456789'), 0xA1)
        self.assertEqual(module.rohc(b'123456789'), 0xD0)
        self.assertEqual(module.itu(b'123456789'), 0xA1)
        self.assertEqual(module.crc8(b'123456789'), 0xF4)

    def test_basics(self):
        """Test basic functionality.
        """
        self.do_basics(libscrc)


    def test_basics_c(self):
        """Test basic functionality of the extension module.
        """
        self.do_basics(_crc8)


if __name__ == '__main__':
    unittest.main()
