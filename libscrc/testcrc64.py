# -*- coding:utf8 -*-
""" Test library for CRC16 """
# !/usr/bin/python
# Python:   3.5.2
# Platform: Windows/Linux/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC64 Module.
# History:  2017-08-19 Wheel Ver:0.0.5 [Heyn] Initialize

import unittest
import libscrc

from libscrc import _crc64

class TestCRC64(unittest.TestCase):
    """Test CRC64 IOS&ECMA182 variant.
    """

    def do_basics(self, module):
        """Test basic functionality.
        """
        # very basic example
        self.assertEqual(module.iso(b'123456789'), 0x46A5A9388A5BEFFE)
        self.assertNotEqual(module.iso(b'123456'), 0x288A5BEFFE4CB001)

        self.assertEqual(module.ecma182(b'123456789'), 0x62EC59E3F1A4F00A)
        self.assertNotEqual(module.ecma182(b'123456'), 0x3DBD4712E4D9E786)

        # the same in two steps
        crc = module.iso(b'12345')
        crc = module.iso(b'6789', crc)
        self.assertEqual(crc, 0x46A5A9388A5BEFFE)

        crc = module.ecma182(b'12345')
        crc = module.ecma182(b'6789', crc^0xFFFFFFFFFFFFFFFF)
        self.assertEqual(crc, 0x62EC59E3F1A4F00A)


    def test_basics(self):
        """Test basic functionality.
        """
        self.do_basics(libscrc)


    def test_basics_c(self):
        """Test basic functionality of the extension module.
        """
        self.do_basics(_crc64)

    def test_big_chunks(self):
        """Test calculation of CRC on big chunks of data.
        """
        self.assertEqual(_crc64.iso(b'A' * 16 * 1024 * 1024), 0x9936c796b73deb69)
        self.assertEqual(_crc64.ecma182(b'A' * 16 * 1024 * 1024), 0x856716c3b2f186a2)


if __name__ == '__main__':
    unittest.main()
