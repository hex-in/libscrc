# -*- coding:utf-8 -*-
""" Library for calculating CRCx/CRC8/CRC16/CRC24/CRC32/CRC64 """
# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Library CRCx/CRC8/CRC16/CRC24/CRC32/CRC64 Modules.
# History:  2017-08-09 Wheel Ver:0.0.1 [Heyn] Initialize
#           2017-08-10 Wheel Ver:0.0.2 [Heyn] New add CRC8 (IntelHex/BCC/LRC/CRC8(VERB))
#           2017-08-17 Wheel Ver:0.0.3 [Heyn] Optimized Code.
#           2017-08-19 Wheel Ver:0.0.5 [Heyn] New CRC64-ISO / CRC64-ECMA182.
#           2017-09-22 Wheel Ver:0.1.5 [Heyn] New _crcx.
#           2020-03-16 Wheel Ver:0.1.6 [Heyn] New hacker16 / hacker32 / hacker64.
#           2020-04-17 Wheel Ver:1.1   [Heyn] New add CRC24

from ._crcx  import *
from ._canx  import *
from ._crc8  import *
from ._crc16 import *
from ._crc24 import *
from ._crc32 import *
from ._crc64 import *
from ._crc82 import *
