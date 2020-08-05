libscrc
=======

libscrc is a library for calculating CRC3 CRC4 CRC5 CRC6 CRC7 CRC8 CRC16 CRC24 CRC32 CRC64 CRC82.

+------------+------------+------------+-----------+-----------+-----------+ 
| CRCx       | CRC8       | CRC16      | CRC24     | CRC32     | CRC64     |
+============+============+============+===========+===========+===========+ 
| CRC3-GSM   | INTEL      | MODBUS     | BLE       |    FSC    | GO-ISO    |
+------------+------------+------------+-----------+-----------+-----------+ 
| CRC3-ROHC  | BCC        | IBM        | OPENPGP   |   CRC32   | ECMA182   |
+------------+------------+------------+-----------+-----------+-----------+
| CRC4-ITU   | LRC        | XMODEM     | LTE-A     |   MPEG2   | WE        |
+------------+------------+------------+-----------+-----------+-----------+ 
| CRC5-ITU   | MAXIM8     | CCITT      | LTE-B     |  ADLER32  | XZ64      |
+------------+------------+------------+-----------+-----------+-----------+ 
| CRC5-EPC   | ROHC       | KERMIT     | OS9       | FLETCHER32|           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC5-USB   | ITU8       | MCRF4XX    | FLEXRAY-A |   POSIX   |           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC6-ITU   | CRC8       | SICK       | FLEXRAY-B |   BZIP2   |           |
+------------+------------+------------+-----------+-----------+-----------+ 
| CRC6-GSM   | SUM8       | DNP        | INTERLAKEN|   JAMCRC  |           |
+------------+------------+------------+-----------+-----------+-----------+ 
| CRC6-DARC  | FLETCHER8  | X25        | CRC24     |  AUTOSAR  |           |
+------------+------------+------------+-----------+-----------+-----------+ 
| CRC7       | SMBUS      | USB        |           | C / ISCSI |           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC7-MMC   | AUTOSAR    | MAXIM16    |           | D/BASE91-D|           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC7-UMTS  | LTE        | DECT(R/X)  |           | Q / AIXM  |           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC7-ROHC  | SAE-J1850  | TCP/UDP    |           |   XFER    |           |
+------------+------------+------------+-----------+-----------+-----------+
|            | I-CODE     | CDMA2000   |           |   CKSUM   |           |
+------------+------------+------------+-----------+-----------+-----------+
| CAN15      | GSM-A      | FLETCHER16 |           |   XZ32    |           |
+------------+------------+------------+-----------+-----------+-----------+
| CAN17      | NRSC-5     | EPC16      |           |   AAL5    |           |
+------------+------------+------------+-----------+-----------+-----------+
| CAN21      | WCDMA      | PROFIBUS   |           | ISO-HDLC  |           |
+------------+------------+------------+-----------+-----------+-----------+
|            | BLUETOOTH  | BUYPASS    |           |   PKZIP   |           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC10-ATM  | DVB-S2     | GENIBUS    |           |   ADCCP   |           |
+------------+------------+------------+-----------+-----------+-----------+
| CRC13-BBC  | EBU        | GSM16      |           |    V-42   |           |
+------------+------------+------------+-----------+-----------+-----------+
| MPT1327    | DARC       | RIELLO     |           |           |           |
+------------+------------+------------+-----------+-----------+-----------+
| CDMA2000   | MIFARE     | OPENSAFETY |           | CRC30-CDMA|           |
+------------+------------+------------+-----------+-----------+-----------+
|            |            | EN13757    |           |  PHILIPS  |           |
+------------+------------+------------+-----------+-----------+-----------+
|            |            |   CMS      |           |           |   DARC82  |
+------------+------------+------------+-----------+-----------+-----------+

Installation
------------

* Compile and install the library::

    pip3 install libscrc

  or::

    python3 setup.py build
    python3 setup.py install

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

    # python3 -m libscrc.test.modbus  
    # python3 -m libscrc.test.crc8  
    # python3 -m libscrc.test.crcx  
    # python3 -m libscrc.test.canx  
    # python3 -m libscrc.test.crc16  
    # python3 -m libscrc.test.crc24  
    # python3 -m libscrc.test.crc32  
    # python3 -m libscrc.test.crc64  

Usage
-----

  In Python 3::

    import libscrc
    crc16 = libscrc.modbus(b'1234')       # Calculate ASCII of modbus
    crc16 = libscrc.modbus(b'\x01\x02')   # Calculate HEX of modbus
    crc16 = libscrc.modbus(bytearray( [ 0x01, 0x02 ] ))

  You can also calculate CRC gradually( v1.4+ )::

    import libscrc
    crc16 = libscrc.xmodem(b'1234')
    crc16 = libscrc.xmodem(b'5678', crc16)

Example
-------
* CRCx::

    crc3 = libscrc.gsm3(b'1234')
    crc3 = libscrc.rohc3(b'1234')
    crc4 = libscrc.itu4(b'1234')
    crc4 = libscrc.g_704(b'1234')
    crc4 = libscrc.interlaken4(b'1234')
    crc5 = libscrc.itu5(b'1234')
    crc5 = libscrc.epc(b'1234')
    crc5 = libscrc.usb5(b'1234')
    crc6 = libscrc.itu6(b'1234')
    crc6 = libscrc.darc6(b'1234')
    crc7 = libscrc.mmc(b'1234')
    crc7 = libscrc.crc7(b'1234')
    crc7 = libscrc.umts7(b'1234')
    crc7 = libscrc.rohc7(b'1234')

    crc10 = libscrc.atm10(b'1234')
    crc10 = libscrc.gsm10(b'1234')
    crc10 = libscrc.crc10_cdma2000(b'1234')
    crc11 = libscrc.umts11(b'1234')
    crc11 = libscrc.flexray11(b'1234')
    crc12 = libscrc.umts12(b'1234')
    crc12 = libscrc.crc12_3gpp(b'1234')
    crc12 = libscrc.dect12(b'1234')
    crc12 = libscrc.crc12(b'1234')
    crc12 = libscrc.gsm12(b'1234')
    crc12 = libscrc.crc12_cdma2000(b'1234')
    crc13 = libscrc.bbc13(b'1234')
    crc14 = libscrc.darc14(b'1234')
    crc15 = libscrc.mpt1327(b'1234')

    crc15 = libscrc.can15(b'1234')
    crc17 = libscrc.can17(b'1234')
    crc21 = libscrc.can21(b'1234')

    crc30 = libscrc.cdma(b'1234')
    crc31 = libscrc.philips(b'1234')

    crc40 = libscrc.gsm40(b'1234')

    crc82 = libscrc.darc82(b'1234')
    
* CRC8::

    crc8 = libscrc.intel(b'1234')
    crc8 = libscrc.bcc(b'1234')  
    crc8 = libscrc.lrc(b'1234')  
    crc8 = libscrc.maxim8(b'1234')
    crc8 = libscrc.rohc(b'1234')
    crc8 = libscrc.itu8(b'1234')
    crc8 = libscrc.crc8(b'1234')
    crc8 = libscrc.smbus(b'1234')
    crc8 = libscrc.autosar8(b'1234')
    crc8 = libscrc.lte8(b'1234')
    crc8 = libscrc.sae_j1850(b'1234')
    crc8 = libscrc.icode8(b'1234')
    crc8 = libscrc.gsm8_a(b'1234')
    crc8 = libscrc.nrsc_5(b'1234')
    crc8 = libscrc.wcdma8(b'1234')
    crc8 = libscrc.bluetooth(b'1234')
    crc8 = libscrc.dvb_s2(b'1234')
    crc8 = libscrc.ebu8(b'1234')
    crc8 = libscrc.darc8(b'1234')
    crc8 = libscrc.opensafety8(b'1234')  
    crc8 = libscrc.i432_1(b'1234') 
    crc8 = libscrc.mifare_mad(b'1234') 
    crc8 = libscrc.tech_3250(b'1234')
    crc8 = libscrc.aes8(b'1234')

* CRC16::

    crc16 = libscrc.ibm(b'1234')            # poly=0xA001 (default Reversed)  
    crc16 = libscrc.ibm(b'1234', 0x8005)    # poly=0x8005 (Normal)
    crc16 = libscrc.modbus(b'1234')  
    crc16 = libscrc.xmodem(b'1234')  
    crc16 = libscrc.zmodem(b'1234')  
    crc16 = libscrc.ccitt(b'1234')  
    crc16 = libscrc.ccitt_aug(b'1234')  
    crc16 = libscrc.ccitt_true(b'1234')  
    crc16 = libscrc.ccitt_false(b'1234')  
    crc16 = libscrc.kermit(b'1234')  
    crc16 = libscrc.mcrf4xx(b'1234')  
    crc16 = libscrc.sick(b'1234')  
    crc16 = libscrc.dnp(b'1234')  
    crc16 = libscrc.x25(b'1234')  
    crc16 = libscrc.usb16(b'1234')  
    crc16 = libscrc.maxim16(b'1234')  
    crc16 = libscrc.dect_r(b'1234')  
    crc16 = libscrc.dect_x(b'1234')  

    data  = b'\x45\x00\x00\x3c\x00\x00\x00\x00\x40\x11\x00\x00\xc0\xa8\x2b\xc3\x08\x08\x08\x08\x11'
    crc16 = libscrc.tcp( data )             # 13933  
    crc16 = libscrc.udp( data )             # 13933  

    crc16 = libscrc.epc16(b'1234')          # RFID EPC(CRC16-PC-EPC)  
    crc16 = libscrc.profibus(b'1234')  
    crc16 = libscrc.buypass(b'1234')  
    crc16 = libscrc.genibus(b'1234')  
    crc16 = libscrc.gsm16(b'1234')  
    crc16 = libscrc.riello(b'1234')  

    crc16 = libscrc.crc16_a(b'1234')  
    crc16 = libscrc.cdma2000(b'1234')  
    crc16 = libscrc.teledisk(b'1234')  
    crc16 = libscrc.tms37157(b'1234')  
    crc16 = libscrc.en13757(b'1234')  
    crc16 = libscrc.t10_dif(b'1234')  
    crc16 = libscrc.dds_110(b'1234')  
    crc16 = libscrc.lj1200(b'1234')  
    crc16 = libscrc.opensafety_a(b'1234')  
    crc16 = libscrc.opensafety_b(b'1234')  

    crc16 = libscrc.arc(b'1234')  
    crc16 = libscrc.lha(b'1234')  
    crc16 = libscrc.cms(b'1234')  
    crc16 = libscrc.darc(b'1234')  
    crc16 = libscrc.epc_c1g2(b'1234')  
    crc16 = libscrc.icode16(b'1234')  
    crc16 = libscrc.ibm_3740(b'1234')  
    crc16 = libscrc.autosar16(b'1234')  
    crc16 = libscrc.ibm_sdlc(b'1234')  
    crc16 = libscrc.iso_hdlc16(b'1234')  
    crc16 = libscrc.iec14443_3_b(b'1234')  
    crc16 = libscrc.iec14443_3_a(b'1234')  
    crc16 = libscrc.v41_lsb(b'1234')  
    crc16 = libscrc.v41_msb(b'1234')  
    crc16 = libscrc.spi_fujitsu(b'1234')  
    crc16 = libscrc.umts(b'1234')  
    crc16 = libscrc.verifone(b'1234')  
    crc16 = libscrc.acorn(b'1234')  
    crc16 = libscrc.lte16(b'1234')  
    crc16 = libscrc.nrsc5(b'1234')  

* CRC24::

    crc24 = libscrc.ble(b'1234')  
    crc24 = libscrc.flexraya(b'1234')  
    crc24 = libscrc.flexrayb(b'1234')  
    crc24 = libscrc.openpgp(b'1234')  
    crc24 = libscrc.lte_a(b'1234')  
    crc24 = libscrc.lte_b(b'1234')  
    crc24 = libscrc.os9(b'1234')  
    crc24 = libscrc.interlaken(b'1234')  
    crc24 = libscrc.crc24(b'1234')  

* CRC32::
    
    crc32 = libscrc.fsc(b'1234')            # Ethernet frame sequence (FSC)  
    crc32 = libscrc.mpeg2(b'1234')          # MPEG2  
    crc32 = libscrc.crc32(b'1234')          # WinRAR, File  
    crc32 = libscrc.posix(b'1234')  
    crc32 = libscrc.bzip2(b'1234')  
    crc32 = libscrc.jamcrc(b'1234')  
    crc32 = libscrc.autosar(b'1234')  
    crc32 = libscrc.crc32_c(b'1234')  
    crc32 = libscrc.crc32_d(b'1234')  
    crc32 = libscrc.crc32_q(b'1234')  
    crc32 = libscrc.xfer(b'1234')  
    crc32 = libscrc.aixm(b'1234')  
    crc32 = libscrc.base91_d(b'1234')  
    crc32 = libscrc.cksum(b'1234')  
    crc32 = libscrc.base91_c(b'1234')  
    crc32 = libscrc.iscsi(b'1234')  
    crc32 = libscrc.iso_hdlc(b'1234')  
    crc32 = libscrc.xz32(b'1234')  
    crc32 = libscrc.pkzip(b'1234')  
    crc32 = libscrc.adccp(b'1234')  
    crc32 = libscrc.v_42(b'1234')  
    crc32 = libscrc.aal5(b'1234')  

* CRC64::
    
    crc64 = libscrc.iso(b'1234')  
    crc64 = libscrc.ecma182(b'1234')  
    crc64 = libscrc.we(b'1234')  
    crc64 = libscrc.xz64(b'1234')  

NOTICE
------ 
* Revert gradually calculating function v1.4+ (2020-08-04) 

V1.4 (2020-08-04)
++++++++++++++++++
* Revert gradually calculating function. ( Fixed Issues #4 )  
* Optimized code  

V1.0 (2020-03-23)
++++++++++++++++++
* New hacker8 \ hacker16 \ hacker32 \ hacker64  
* New FLETCHER8 \ FLETCHER16 \ FLETCHER32  

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
* New CRC16-DNP and CRC16-SICK

