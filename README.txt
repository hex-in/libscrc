=====================================================
Python library for calculating CRC8 CRC16 CRC32 CRC64
=====================================================

------------
Installation
------------

* compile and install the library::

    [Win32 or Ubuntu Linux]  
    # python setup.py build
    # python setup.py install

  you will need the administrative privileges to execute the last command.

    [ARM imx6(toradex)]  
    # opkg install gcc
    # opkg install gcc-symlinks
    # opkg install binutils

    # gcc version 6.2.1 20161016 (Linaro GCC 6.2-2016.11)

    # CC=gcc LDSHARED='gcc -shared' python setup.py build 
    # CC=gcc LDSHARED='gcc -shared' python setup.py install
    # CC=gcc LDSHARED='gcc -shared' python setup.py build bdist_wheel

    [Linux]
    # vim setup.cfg
    universal=1
    plat-name=manylinux1_x86_64

    [Linux for ARMv7]

    # vim setup.cfg
    universal=1
    plat-name=linux_armv7l

    export PATH=/mnt/tools/gcc-linaro-6.2.1/bin/:$PATH
    export CROSS_COMPILE=arm-linux-gnueabihf-
    export CC='${CROSS_COMPILE}gcc -pthread'
    export LDSHARED='${CROSS_COMPILE}gcc -shared'
    export CFLAGS='-I/usr/include/python3.5m -I/usr/include/'
    
    python3 setup.py build
    # gcc version 6.2.1 20161016 (Linaro GCC 6.2-2016.11)

    [Upload]  
    #twine upload *.whl  

-----
Usage
-----

In Python 3::
    import libscrc  
    # ASCII  
    crc16 = libscrc.modbus(b'1234')  
    # HEX  
    crc16 = libscrc.modbus(b'\x01\x02')  
    # Swap Method  
    import struct  
    [ i for i in struct.pack('<H',crc16)]  
  
You can also calculate CRC gradually::
    import libscrc  
    crc16 = libscrc.xmodem(b'1234')  
    crc16 = libscrc.xmodem(b'5678', crc16)  
    print(crc16)  
  
  
Other Example:  

    crc4 = libscrc.itu4(b'1234')  
    crc5 = libscrc.itu5(b'1234')  
    crc5 = libscrc.epc(b'1234')  
    crc5 = libscrc.usb5(b'1234')  
    crc6 = libscrc.itu6(b'1234')  
    crc7 = libscrc.mmc(b'1234')  

    # CRC8 Method  
    crc8 = libscrc.intel(b'1234')  
    crc8 = libscrc.bcc(b'1234')  
    crc8 = libscrc.lrc(b'1234')  
    crc8 = libscrc.maxim8(b'1234')  
    crc8 = libscrc.rohc(b'1234')  
    crc8 = libscrc.itu8(b'1234')  
    crc8 = libscrc.crc8(b'1234')  
  
    crc16 = libscrc.ibm(b'1234')            # poly=0xA001 (default Reversed)  
    crc16 = libscrc.ibm(b'1234', 0x8005)    # poly=0x8005 (Normal)
    crc16 = libscrc.modbus(b'1234')  
    crc16 = libscrc.xmodem(b'1234')  
    crc16 = libscrc.ccitt(b'1234')  
    crc16 = libscrc.ccitt_false(b'1234')  
    crc16 = libscrc.kermit(b'1234')  
    crc16 = libscrc.sick(b'1234')  
    crc16 = libscrc.dnp(b'1234')  
    crc16 = libscrc.x25(b'1234')  
    crc16 = libscrc.usb16(b'1234')  
    crc16 = libscrc.maxim16(b'1234')  
    crc16 = libscrc.dect(b'1234')           # poly=0x0589 (Cordless Telephones)
 
  
    crc32 = libscrc.fsc(b'1234')            # Ethernet frame sequence (FSC)  
    crc32 = libscrc.mpeg2(b'1234')          # MPEG2  
    crc32 = libscrc.crc32(b'1234')          # WinRAR, File   
  
    # Wheel Ver:0.0.5  
    crc64 = libscrc.iso(b'1234')  
    crc64 = libscrc.ecma182(b'1234')  

V0.1.5 (2017-09-22)
+++++++++++++++++++
* New CRC4-ITU      Poly = 0x03 Initial = 0x00 Xorout=0x00 Refin=True Refout=True
* New CRC5-ITU      Poly = 0x15 Initial = 0x00 Xorout=0x00 Refin=True Refout=True
* New CRC5-EPC      Poly = 0x09 Initial = 0x09 Xorout=0x00 Refin=False Refout=False
* New CRC5-USB      Poly = 0x05 Initial = 0x1F Xorout=0x1F Refin=True Refout=True
* New CRC6-ITU      Poly = 0x03 Initial = 0x00 Xorout=0x00 Refin=True Refout=True
* New CRC7-MMC      Poly = 0x09 Initial = 0x00 Xorout=0x00 Refin=False Refout=False

V0.1.4 (2017-09-21)
+++++++++++++++++++
* New CRC8-MAXIM8   Poly = 0x31 Initial = 0x00 Xorout=0x00 Refin=True  Refout=True
* New CRC8-ROHC     Poly = 0x07 Initial = 0xFF Xorout=0x00 Refin=True  Refout=True
* New CRC8-ITU      Poly = 0x07 Initial = 0x00 Xorout=0x55 Refin=False Refout=False
* New CRC8-CRC8     Poly = 0x07 Initial = 0x00 Xorout=0x00 Refin=False Refout=False


V0.1.3 (2017-09-19)
+++++++++++++++++++
* New CRC16-X25  
* New CRC16-USB  
* New CRC16-MAXIM16  
* New CRC16-CCITT_FALSE
* New CRC16-DECT

**Bugfixes**
  * Calculate CRC16-IBM of poly = 0x8005 is ERROR.


V0.1.2 (2017-08-22)
+++++++++++++++++++
**Platform Support**
  * Win32
  * Linux_x86_64
  * MacOSX_10_6_intel
  * ARMv7 (Toradex Ixora iMX6 Linux-4.1.41)

**Bugfixes**
  * Coding C99 standard.
  * Python/C API parsing arguments type error in linux.

V0.1.1 (2017-08-20)
+++++++++++++++++++
* New CRC16-NDP and CRC16-SICK
  
--------------
Other projects
--------------
  
  
