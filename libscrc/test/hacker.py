# -*- coding:utf-8 -*-
""" Test library for CRC hacker """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library hacker16 and hacker32 Module.
# Package:  pip install libscrc.
# History:  2020-03-16 Wheel Ver:0.1.6 [Heyn] Initialize

import unittest

import libscrc
from libscrc import _crc16
from libscrc import _crc32

class TestHacker( unittest.TestCase ):
    """ Test hacker variant.
    """

    def do_basics_16( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.hacker16( data=b'123456789', poly=0xA001 ), 0x4B37 )
        self.assertEqual( module.hacker16( data=b'123456789', poly=0x8408, xorout=0xFFFF ), 0x906E )
        self.assertEqual( module.hacker16( data=b'123456789', poly=0x1021, init=0x0000 ), 0x31C3 )

    def do_basics_32( self, module ):
        """ Test basic functionality.
        """
        ### self.assertEqual( module.fsc(b'123456789'),     0x0376E6E7 )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7 ), 0x0376E6E7 )
        ### self.assertEqual( module.crc32(b'123456789'),   0xCBF43926 )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0xEDB88320, xorout=0xFFFFFFFF ), 0xCBF43926 )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF, xorout=0xFFFFFFFF, ref=True ), 0xCBF43926 )


    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics_16( libscrc )
        self.do_basics_32( libscrc )


    def test_basics_c( self ):
        """ Test basic functionality of the extension module.
        """
        self.do_basics_16( _crc16 )
        self.do_basics_32( _crc32 )


if __name__ == '__main__':
    unittest.main()
