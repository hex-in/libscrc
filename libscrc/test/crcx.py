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

        self.assertEqual( module.interlaken4(b'123456789'),         0x0B )

        self.assertEqual( module.atm10(b'123456789'),               0x199 )
        self.assertEqual( module.gsm10(b'123456789'),               0x12A )
        self.assertEqual( module.umts11(b'123456789'),              0x061 )
        self.assertEqual( module.umts12(b'123456789'),              0xDAF )
        self.assertEqual( module.crc12_3gpp(b'123456789'),          0xDAF )
        self.assertEqual( module.flexray11(b'123456789'),           0x5A3 )
        self.assertEqual( module.dect12(b'123456789'),              0xF5B )
        self.assertEqual( module.crc12(b'123456789'),               0xF5B )
        self.assertEqual( module.gsm12(b'123456789'),               0xB34 )

        self.assertEqual( module.crc10_cdma2000(b'123456789'),      0x233 )
        self.assertEqual( module.crc12_cdma2000(b'123456789'),      0xD4D )

        self.assertEqual( module.bbc13(b'123456789'),               0x04FA )
        self.assertEqual( module.darc14(b'123456789'),              0x082D )
        self.assertEqual( module.gsm14(b'123456789'),               0x30AE )
        self.assertEqual( module.mpt1327(b'123456789'),             0x2566 )

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
