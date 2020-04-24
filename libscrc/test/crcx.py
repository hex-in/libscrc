# -*- coding:utf-8 -*-
""" Test library for CRCx """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRCx Module.
# Package:  pip install libscrc.
# History:  2020-04-23 Wheel Ver:1.1 [Heyn] Initialize

import unittest

import libscrc
from libscrc import _crcx

class TestCRCx( unittest.TestCase ):
    """ Test CRCx variant.
    """

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.gsm3(b'123456789'),     0x04 )
        self.assertEqual( module.rohc3(b'123456789'),    0x06 )
        self.assertEqual( module.itu4(b'123456789'),     0x07 )
        self.assertEqual( module.g_704(b'123456789'),    0x07 )
        self.assertEqual( module.itu5(b'123456789'),     0x07 )
        self.assertEqual( module.epc(b'123456789'),      0x00 )
        self.assertEqual( module.usb5(b'123456789'),     0x19 )
        self.assertEqual( module.itu6(b'123456789'),     0x06 )
        self.assertEqual( module.gsm6(b'123456789'),     0x13 )
        self.assertEqual( module.darc6(b'123456789'),    0x26 )

        self.assertEqual( module.mmc(b'123456789'),      0x75 )
        self.assertEqual( module.crc7(b'123456789'),     0x75 )
        self.assertEqual( module.umts7(b'123456789'),    0x61 )
        self.assertEqual( module.rohc7(b'123456789'),    0x53 )

        self.assertEqual( module.interlaken4(b'123456789'),     0x0B )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics( libscrc )


    def test_basics_c( self ):
        """Test basic functionality of the extension module.
        """
        self.do_basics( _crcx )


if __name__ == '__main__':
    unittest.main()
