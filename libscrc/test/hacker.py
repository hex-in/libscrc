# -*- coding:utf-8 -*-
""" Test library for CRC hacker """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library hacker8 hacker16 hacker24 hacker32 and hacker64 Module.
# Package:  pip install libscrc.
# History:  2020-03-16 Wheel Ver:0.1.6 [Heyn] Initialize
#           2020-04-30 Wheel Ver:1.3   [Heyn] Optimized code
#           2021-06-07 Wheel Ver:1.7   [Heyn] New add hacker8 hacker24 hacker64 test code.
#           2021-06-08 Wheel Ver:1.7   [Heyn] If the polynomial changes, you need to set reinit=True

import unittest

import libscrc
from libscrc import _crc8
from libscrc import _crc16
from libscrc import _crc24
from libscrc import _crc32
from libscrc import _crc64

class TestHacker( unittest.TestCase ):
    """ Test hacker variant.
    """

    def do_basics_08( self, module ):
        """ Test basic functionality.
            *** If the polynomial changes, you need to set reinit=True
        """
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0x00, xorout=0x00, refin=False, refout=False, reinit=True ), 0xF4 )
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0x00, xorout=0x00, refin=True,  refout=False, reinit=True ), 0x04 )
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0x00, xorout=0x00, refin=False, refout=True,  reinit=True ), 0x2F )
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0x00, xorout=0x00, refin=True,  refout=True,  reinit=True ), 0x20 )
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0x00, xorout=0x01, refin=False, refout=True,  reinit=True ), 0x2E )
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0x01, xorout=0x01, refin=False, refout=True,  reinit=True ), 0xB0 )
        self.assertEqual( module.hacker8(b'123456789', poly=0x07, init=0xFF, xorout=0x00, refin=True,  refout=True,  reinit=True ), 0xD0 )

    def do_basics_16( self, module ):
        """ Test basic functionality.
            *** If the polynomial changes, you need to set reinit=True
        """
        self.assertEqual( module.hacker16( data=b'123456789', poly=0xA001, reinit=True ),                0x3D7B )
        self.assertEqual( module.hacker16( data=b'123456789', poly=0x8408, xorout=0xFFFF, reinit=True ), 0xE245 )

        self.assertEqual( module.hacker16( data=b'123456789', poly=0x8005, init=0xFFFF, xorout=0x0000, refin=False, refout=False, reinit=True  ), 0xAEE7 )
        self.assertEqual( module.hacker16( data=b'123456789', poly=0x8005, init=0xFFFF, xorout=0x0000, refin=True,  refout=False, reinit=False ), 0xECD2 )
        self.assertEqual( module.hacker16( data=b'123456789', poly=0x8005, init=0xFFFF, xorout=0x0000, refin=False, refout=True,  reinit=False ), 0xE775 )
        self.assertEqual( module.hacker16( data=b'123456789', poly=0x8005, init=0xFFFF, xorout=0x0000, refin=True,  refout=True,  reinit=False ), 0x4B37 )

    def do_basics_24( self, module ):
        """ Test basic functionality.
            *** If the polynomial changes, you need to set reinit=True
        """
        self.assertEqual( module.hacker24( data=b'123456789', poly=0x800063, init=0xFFFFFF, xorout=0xFFFFFF, refin=False, refout=False, reinit=True  ), 0x200FA5 )
        self.assertEqual( module.hacker24( data=b'123456789', poly=0x800063, init=0xFFFFFF, xorout=0xFFFFFF, refin=True,  refout=False, reinit=False ), 0x23503A )
        self.assertEqual( module.hacker24( data=b'123456789', poly=0x800063, init=0xFFFFFF, xorout=0xFFFFFF, refin=False, refout=True,  reinit=False ), 0xA5F004 )
        self.assertEqual( module.hacker24( data=b'123456789', poly=0x800063, init=0xFFFFFF, xorout=0xFFFFFF, refin=True,  refout=True,  reinit=False ), 0x5C0AC4 )

    def do_basics_32( self, module ):
        """ Test basic functionality.
            *** If the polynomial changes, you need to set reinit=True
        """
        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, reinit=True ),                    0xCBF43926 )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0xEDB88320, xorout=0xFFFFFFFF, reinit=True ), 0xFC4F2BE9 )

        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF, xorout=0xFFFFFFFF, refin=False, refout=False, reinit=True  ), 0xFC891918 )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF, xorout=0xFFFFFFFF, refin=True,  refout=False, reinit=False ), 0x649C2FD3 )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF, xorout=0xFFFFFFFF, refin=False, refout=True,  reinit=False ), 0x1898913F )
        self.assertEqual( module.hacker32( data=b'123456789', poly=0x04C11DB7, init=0xFFFFFFFF, xorout=0xFFFFFFFF, refin=True,  refout=True,  reinit=False ), 0xCBF43926 )

    def do_basics_64( self, module ):
        """ Test basic functionality.
            *** If the polynomial changes, you need to set reinit=True
        """
        self.assertEqual( module.hacker64(b'123456789', poly=0x42F0E1EBA9EA3693, init=0, xorout=0, refin=False, refout=False, reinit=True ), 0x6C40DF5F0B497347 )
        self.assertEqual( module.hacker64(b'123456789', poly=0x42F0E1EBA9EA3693, init=0, xorout=0, refin=True,  refout=False, reinit=True ), 0x51301E47277E39D4 )
        self.assertEqual( module.hacker64(b'123456789', poly=0x42F0E1EBA9EA3693, init=0, xorout=0, refin=False, refout=True,  reinit=True ), 0xE2CE92D0FAFB0236 )
        self.assertEqual( module.hacker64(b'123456789', poly=0x42F0E1EBA9EA3693, init=0, xorout=0, refin=True,  refout=True,  reinit=True ), 0x2B9C7EE4E2780C8A )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics_08( libscrc )
        self.do_basics_16( libscrc )
        self.do_basics_24( libscrc )
        self.do_basics_32( libscrc )
        self.do_basics_64( libscrc )

    def test_basics_c( self ):
        """ Test basic functionality of the extension module.
        """
        self.do_basics_08( _crc8  )
        self.do_basics_16( _crc16 )
        self.do_basics_24( _crc24 )
        self.do_basics_32( _crc32 )
        self.do_basics_64( _crc64 )

if __name__ == '__main__':
    unittest.main()
