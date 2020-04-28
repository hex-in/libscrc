# -*- coding:utf-8 -*-
""" Test library for CRC16 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC16 Module.
# Package:  pip install libscrc.
# History:  2017-08-17 Wheel Ver:0.0.3 [Heyn] Initialize
#           2019-04-22 Wheel Ver:1.1   [Heyn] New add some functions.
#           2020-04-28 Wheel Ver:1.3   [Heyn] Optimized code.

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
        self.assertEqual( module.xmodem(b'123456789'),      0x31C3 )
        self.assertEqual( module.ccitt_aug(b'123456789'),   0xE5CC )
        self.assertEqual( module.kermit(b'123456789'),      0x2189 )
        self.assertEqual( module.mcrf4xx(b'123456789'),     0x6F91 )
        self.assertEqual( module.ccitt_false(b'123456789'), 0x29B1 )
        self.assertEqual( module.sick(b'123456789'),        0xA656 )
        self.assertEqual( module.dnp(b'123456789'),         0xEA82 )
        self.assertEqual( module.ibm(b'123456789'),         0xBB3D )
        self.assertEqual( module.arc(b'123456789'),         0xBB3D )
        self.assertEqual( module.lha(b'123456789'),         0xBB3D )
        self.assertEqual( module.modbus(b'123456789'),      0x4B37 )
        self.assertEqual( module.usb16(b'123456789'),       0xB4C8 )
        self.assertEqual( module.maxim16(b'123456789'),     0x44C2 )
        self.assertEqual( module.dect_r(b'123456789'),      0x007E )
        self.assertEqual( module.dect_x(b'123456789'),      0x007F )
        self.assertEqual( module.udp(b'\x45\x00\x00\x3c\x00\x00\x00\x00\x40\x11\x00\x00\xc0\xa8\x2b\xc3\x08\x08\x08\x08\x11'), 0x366D )
        self.assertEqual( module.fletcher16(b'123456789'),  0x1EDE )
        self.assertEqual( module.profibus(b'123456789'),    0xA819 )
        self.assertEqual( module.buypass(b'123456789'),     0xFEE8 )
        self.assertEqual( module.genibus(b'123456789'),     0xD64E )
        self.assertEqual( module.gsm16(b'123456789'),       0xCE3C )
        self.assertEqual( module.riello(b'123456789'),      0x63D0 )
        self.assertEqual( module.crc16_a(b'123456789'),     0xBF05 )
        self.assertEqual( module.cdma2000(b'123456789'),    0x4C06 )
        self.assertEqual( module.teledisk(b'123456789'),    0x0FB3 )
        self.assertEqual( module.tms37157(b'123456789'),    0x26B1 )
        self.assertEqual( module.en13757(b'123456789'),     0xC2B7 )
        self.assertEqual( module.t10_dif(b'123456789'),     0xD0DB )
        self.assertEqual( module.dds_110(b'123456789'),     0x9ECF )
        self.assertEqual( module.lj1200(b'123456789'),      0xBDF4 )

        self.assertEqual( module.opensafety_a(b'123456789'),0x5D38 )
        self.assertEqual( module.opensafety_b(b'123456789'),0x20FE )

        self.assertEqual( module.cms(b'123456789'),         0xAEE7 )
        self.assertEqual( module.darc(b'123456789'),        0xD64E )
        self.assertEqual( module.epc_c1g2(b'123456789'),    0xD64E )
        self.assertEqual( module.icode16(b'123456789'),     0xD64E )

        self.assertEqual( module.ibm_3740(b'123456789'),    0x29B1 )
        self.assertEqual( module.autosar16(b'123456789'),   0x29B1 )

        self.assertEqual( module.ibm_sdlc(b'123456789'),    0x906E )
        self.assertEqual( module.iso_hdlc16(b'123456789'),  0x906E )
        self.assertEqual( module.iec14443_3_b(b'123456789'),0x906E )
        self.assertEqual( module.iec14443_3_a(b'123456789'),0xBF05 )

        self.assertEqual( module.ccitt_true(b'123456789'),  0x2189 )
        self.assertEqual( module.ccitt(b'123456789'),       0x2189 )
        self.assertEqual( module.v41_lsb(b'123456789'),     0x2189 )
        self.assertEqual( module.spi_fujitsu(b'123456789'), 0xE5CC )

        self.assertEqual( module.umts(b'123456789'),        0xFEE8 )
        self.assertEqual( module.verifone(b'123456789'),    0xFEE8 )

        self.assertEqual( module.zmodem(b'123456789'),      0x31C3 )
        self.assertEqual( module.acorn(b'123456789'),       0x31C3 )
        self.assertEqual( module.v41_msb(b'123456789'),     0x31C3 )
        self.assertEqual( module.lte16(b'123456789'),       0x31C3 )
        self.assertEqual( module.nrsc5(b'123456789'),       0xA066 )

        self.assertEqual( module.hacker16( data=b'123456789',
                                           poly=0x8005,
                                           init=0xFFFF,
                                           xorout=0x0000,
                                           refin=True,
                                           refout=True ),   0x4B37 )
        # the same in two steps ( Removed v1.3+ )
        # crc = module.x25( b'12345' )
        # crc = module.x25( b'6789', crc )
        # self.assertNotEqual( crc, 0x906E )

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
