libscrc
=======

libscrc is a library for calculating CRC4 CRC5 CRC6 CRC7 CRC8 CRC16 CRC32 CRC64.

+------------+------------+------------+-----------+-----------+ 
| CRCx       | CRC8       | CRC16      | CRC32     | CRC64     |
+============+============+============+===========+===========+ 
| CRC4-ITU   | Intel      | Modbus     | FSC       | ISO       |
+------------+------------+------------+-----------+-----------+ 
| CRC5-ITU   | BCC        | IBM        | CRC32     | ECMA182   |
+------------+------------+------------+-----------+-----------+ 
| CRC5-EPC   | LRC        | XModem     | MPEG2     |           |
+------------+------------+------------+-----------+-----------+ 
| CRC5-USB   | MAXIM8     | CCITT      |           |           |
+------------+------------+------------+-----------+-----------+ 
| CRC6-ITU   | ROHC       | Kermit     |           |           |
+------------+------------+------------+-----------+-----------+ 
| CRC7-MMC   | ITU8       | Sick       |           |           |
+------------+------------+------------+-----------+-----------+ 
|            | CRC8       | DNP        |           |           |
+------------+------------+------------+-----------+-----------+ 
|            |            | X25        |           |           |
+------------+------------+------------+-----------+-----------+ 
|            |            | USB        |           |           |
+------------+------------+------------+-----------+-----------+
|            |            | MAXIM16    |           |           |
+------------+------------+------------+-----------+-----------+
|            |            | DECT       |           |           |
+------------+------------+------------+-----------+-----------+


Installation
------------

* Compile and install the library::

    pip3 install libscrc

  or::

    python setup.py build
    python setup.py install

  You will need the administrative privileges to execute the last command.

* Cross compile and install the library::

    # vim setup.cfg
    universal=1
    plat-name=linux_armv7l

    export CROSS_COMPILE=arm-linux-gnueabihf-
    export CC='${CROSS_COMPILE}gcc -pthread'
    export LDSHARED='${CROSS_COMPILE}gcc -shared'
    export CFLAGS='-I/usr/include/python3.5m -I/usr/include/'

    python3 setup.py build
    # gcc version 6.2.1 20161016 (Linaro GCC 6.2-2016.11)
    
* After installation you can run unit tests to make sure that the library works fine.  Execute::

    python -m libscrc.testmodbus
    python -m libscrc.testcrc64

Usage
-----

  In Python 3::

    import libscrc
    crc16 = libscrc.modbus(b'1234')  # Calculate ASCII of modbus
    crc16 = libscrc.modbus(b'\x01\x02')  # Calculate HEX of modbus

  You can also calculate CRC gradually::

    import libscrc
    crc16 = libscrc.xmodem(b'1234')
    crc16 = libscrc.xmodem(b'5678', crc16)

Example
-------
* CRCx::

    crc4 = libscrc.itu4(b'1234')
    crc5 = libscrc.itu5(b'1234')
    crc5 = libscrc.epc(b'1234')
    crc5 = libscrc.usb5(b'1234')
    crc6 = libscrc.itu6(b'1234')
    crc7 = libscrc.mmc(b'1234')

* CRC8::

    crc8 = libscrc.intel(b'1234')
    crc8 = libscrc.bcc(b'1234')  
    crc8 = libscrc.lrc(b'1234')  
    crc8 = libscrc.maxim8(b'1234')
    crc8 = libscrc.rohc(b'1234')
    crc8 = libscrc.itu8(b'1234')
    crc8 = libscrc.crc8(b'1234')

* CRC16::

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

* CRC32::
    
    crc32 = libscrc.fsc(b'1234')            # Ethernet frame sequence (FSC)
    crc32 = libscrc.mpeg2(b'1234')          # MPEG2
    crc32 = libscrc.crc32(b'1234')          # WinRAR, File

* CRC64::

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

