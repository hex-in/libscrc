# -*- coding:utf-8 -*-
""" CONVERT. """
# !/usr/bin/python
# Python:   3.6.5+
# Platform: Windows/Linux/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  CONVERT.
# Package:  None
# History:  2021-07-23 Ver:1.0 [Heyn] Initialize

import sys

def str2hex( string ) -> bytes:
    """ string = 'AABBCCDDEEFF'
        return b'\xaa\xbb\xcc\xdd\xee\xff'
    """
    if sys.version_info <= ( 2, 7, 18 ):
        return string.decode('hex')
    return bytes.fromhex( string )

def hex2str( a_bytes ) -> str:
    """ a_bytes = b'\xaa\xbb\xcc\xdd\xee\xff'
        return AABBCCDDEEFF
    """
    if sys.version_info <= ( 2, 7, 18 ):
        return a_bytes.encode('hex').upper()
    return a_bytes.hex().upper()

