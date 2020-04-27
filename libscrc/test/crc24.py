# -*- coding:utf-8 -*-
""" Test library for CRC24 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC24 Module.
# Package:  pip install libscrc.
# History:  2020-04-20 Wheel Ver:1.1 [Heyn] Initialize

import unittest

import libscrc
from libscrc import _crc24

class TestCRC24( unittest.TestCase ):
    """ Test CRC24 variant.
    """

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.ble(b'123456789'),         0xC25A56 )
        self.assertEqual( module.flexraya(b'123456789'),    0x7979BD )
        self.assertEqual( module.flexrayb(b'123456789'),    0x1F23B8 )

        self.assertEqual( module.openpgp(b'123456789'),     0x21CF02 )
        self.assertEqual( module.crc24(b'123456789'),       0x21CF02 )
        self.assertEqual( module.lte_a(b'123456789'),       0xCDE703 )
        self.assertEqual( module.lte_b(b'123456789'),       0x23EF52 )
        self.assertEqual( module.interlaken(b'123456789'),  0xB4F3E6 )

        self.assertEqual( module.os9(b'123456789'),         0x200FA5 )

        self.assertEqual( module.hacker24( data=b'123456789',
                                           poly=0x800063,
                                           init=0xFFFFFF,
                                           xorout=0xFFFFFF,
                                           refin=False,
                                           refout=False ),   0x200FA5 )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics( libscrc )


    def test_basics_c( self ):
        """Test basic functionality of the extension module.
        """
        self.do_basics( _crc24 )


if __name__ == '__main__':
    unittest.main()
