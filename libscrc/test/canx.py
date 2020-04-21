# -*- coding:utf-8 -*-
""" Test library for CANx """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Test library CANx Module.
# Package:  pip install libscrc.
# History:  2020-04-21 Wheel Ver:1.1 [Heyn] Initialize

import unittest

import libscrc
from libscrc import _canx

class TestCANx( unittest.TestCase ):
    """ TestCANx variant.
    """

    def do_basics( self, module ):
        """ Test basic functionality.
        """
        self.assertEqual( module.can15(b'123456789'),   0x059E  )
        self.assertEqual( module.can17(b'123456789'),   0x04F03 )
        self.assertEqual( module.can21(b'123456789'),   0xED841 )

    def test_basics( self ):
        """ Test basic functionality.
        """
        self.do_basics( libscrc )


    def test_basics_c( self ):
        """Test basic functionality of the extension module.
        """
        self.do_basics( _canx )


if __name__ == '__main__':
    unittest.main()
