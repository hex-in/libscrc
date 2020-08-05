# -*- coding:utf-8 -*-
""" Test library for CRC32 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CRC32 Module.
# Package:  pip install libscrc.
# History:  2020-03-13 Wheel Ver:0.1.6 [Heyn] Initialize
#           2020-04-26 Wheel Ver:1.3   [Heyn] Optimized Code
#           2020-08-05 Wheel Ver:1.4   [Heyn] New add gradually calculating

import unittest

import libscrc
from libscrc import _crc32

class TestCRC32( unittest.TestCase ):
    """ Test CRC32 variant.
    """
    def do_gradually( self , module ):
        """ Test gradually calculating functionality.
            (libscrc >= 1.4)
            ##### Gradually calculate invalid functions #####
            libscrc.fletcher32()
        """
        crc = module.mpeg2( b'12345' )
        crc = module.mpeg2( b'6789', crc )
        self.assertEqual( crc, 0x0376E6E7 )

        crc = module.crc32( b'12345' )
        crc = module.crc32( b'6789', crc )
        self.assertEqual( crc, 0xCBF43926 )

        self.assertEqual( module.posix(   b'6789', module.posix(   b'12345') ), 0x765E7680 )
        self.assertEqual( module.bzip2(   b'6789', module.bzip2(   b'12345') ), 0xFC891918 )
        self.assertEqual( module.jamcrc(  b'6789', module.jamcrc(  b'12345') ), 0x340BC6D9 )
        self.assertEqual( module.autosar( b'6789', module.autosar( b'12345') ), 0x1697D06A )
        self.assertEqual( module.crc32_c( b'6789', module.crc32_c( b'12345') ), 0xE3069283 )
        self.assertEqual( module.crc32_d( b'6789', module.crc32_d( b'12345') ), 0x87315576 )
        self.assertEqual( module.crc32_q( b'6789', module.crc32_q( b'12345') ), 0x3010BF7F )
        self.assertEqual( module.xfer(    b'6789', module.xfer(    b'12345') ), 0xBD0BE338 )
        self.assertEqual( module.cdma(    b'6789', module.cdma(    b'12345') ), 0x04C34ABF )
        self.assertEqual( module.philips( b'6789', module.philips( b'12345') ), 0x0CE9E46C )

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.fsc(b'123456789'),     0x0376E6E7 )
        self.assertEqual( module.crc32(b'123456789'),   0xCBF43926 )
        self.assertEqual( module.iso_hdlc32(b'123456789'),0xCBF43926 )
        self.assertEqual( module.xz32(b'123456789'),    0xCBF43926 )
        self.assertEqual( module.pkzip(b'123456789'),   0xCBF43926 )
        self.assertEqual( module.adccp(b'123456789'),   0xCBF43926 )
        self.assertEqual( module.v_42(b'123456789'),    0xCBF43926 )
        self.assertEqual( module.mpeg2(b'123456789'),   0x0376E6E7 )
        self.assertEqual( module.posix(b'123456789'),   0x765E7680 )
        self.assertEqual( module.cksum(b'123456789'),   0x765E7680 )
        self.assertEqual( module.bzip2(b'123456789'),   0xFC891918 )
        self.assertEqual( module.jamcrc(b'123456789'),  0x340BC6D9 )
        self.assertEqual( module.autosar(b'123456789'), 0x1697D06A )
        self.assertEqual( module.crc32_c(b'123456789'), 0xE3069283 )
        self.assertEqual( module.base91_c(b'123456789'),0xE3069283 )
        self.assertEqual( module.iscsi(b'123456789'),   0xE3069283 )
        self.assertEqual( module.crc32_d(b'123456789'), 0x87315576 )
        self.assertEqual( module.crc32_q(b'123456789'), 0x3010BF7F )
        self.assertEqual( module.aixm(b'123456789'),    0x3010BF7F )
        self.assertEqual( module.xfer(b'123456789'),    0xBD0BE338 )
        self.assertEqual( module.base91_d(b'123456789'),0x87315576 )
        self.assertEqual( module.aal5(b'123456789'),    0xFC891918 )

        self.assertEqual( module.cdma(b'123456789'),    0x04C34ABF )
        self.assertEqual( module.philips(b'123456789'), 0x0CE9E46C )

        self.assertEqual( module.crc32( b'A' * 4096),           0xFEA63440 )
        self.assertEqual( module.crc32( b'A' * 4096 * 10),      0x1521C066 )
        self.assertEqual( module.fletcher32( b'123456789' ),    0xDF09D509 )

        # test when there are no data
        self.assertEqual( module.crc32( b'' ),                  0x00000000 )

        # test hacker.
        self.assertEqual( module.hacker32( b'123456789' ),      0xCBF43926 )
        self.assertEqual( module.hacker32( data=b'123456789',
                                           poly=0x04C11DB7,
                                           init=0xFFFFFFFF,
                                           xorout=0xFFFFFFFF,
                                           refin=True,
                                           refout=True ),       0xCBF43926 )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics( libscrc )
        self.do_gradually( libscrc )

    def test_basics_c( self ):
        """Test basic functionality of the extension module.
        """
        self.do_gradually( _crc32 )
        self.do_basics( _crc32 )

if __name__ == '__main__':
    unittest.main()
