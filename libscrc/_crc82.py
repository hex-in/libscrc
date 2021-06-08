# -*- coding:utf-8 -*-
""" Library CRC82. """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Library CRC82.
# History:  2020-04-28 Wheel Ver:1.3 [Heyn] Initialize
#           2021-06-08 Wheel Ver:1.7 [Heyn] Compatible with python2

import sys

def hexin_crcx_reverse82( data ):
        bits = '{:0>82b}'.format( data )
        return int( bits[::-1], 2 )

if sys.version_info > ( 3, 0 ):
    def darc82( data ):
        poly = hexin_crcx_reverse82( 0x0308c0111011401440411 )
        init = 0x000000000000000000000
        crcx = 0x000000000000000000000

        for item in data:
            crcx = crcx ^ item
            for _ in range( 8 ):
                if ( crcx & 1 ):
                    crcx = ( crcx >> 1 ) ^ poly
                else:
                    crcx = ( crcx >> 1 )
        return crcx
else:
    def darc82( data ):
        poly = hexin_crcx_reverse82( 0x0308c0111011401440411 )
        init = 0x000000000000000000000
        crcx = 0x000000000000000000000

        for item in data:
            crcx = crcx ^ ord(item)
            for _ in range( 8 ):
                if ( crcx & 1 ):
                    crcx = ( crcx >> 1 ) ^ poly
                else:
                    crcx = ( crcx >> 1 )
        return crcx
