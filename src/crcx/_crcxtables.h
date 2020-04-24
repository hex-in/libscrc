/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crcxtables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized Code.
*
*********************************************************************************************************
*/

#ifndef __CRCX_TABLES_H__
#define __CRCX_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif


enum hexin_crcx_mask {
    HEXIN_CRC4_MASK = 0x08,
    HEXIN_CRC5_MASK = 0x10,
    HEXIN_CRC6_MASK = 0x20,
    HEXIN_CRC7_MASK = 0x40,
} CRCx_MASK;

unsigned char hexin_reverse8( unsigned char data );

unsigned char hexin_calc_crc3_gsm(  const unsigned char *pSrc, unsigned int len, unsigned char crc3 );
unsigned char hexin_calc_crc3_rohc( const unsigned char *pSrc, unsigned int len, unsigned char crc3 );

unsigned char hexin_calc_crc4_0c( const unsigned char *pSrc, unsigned int len, unsigned char crc4 );
unsigned char hexin_calc_crc4_03( const unsigned char *pSrc, unsigned int len, unsigned char crc4 );

unsigned char hexin_calc_crc5_15( const unsigned char *pSrc, unsigned int len, unsigned char crc5 );
unsigned char hexin_calc_crc5_09( const unsigned char *pSrc, unsigned int len, unsigned char crc5 );
unsigned char hexin_calc_crc5_14( const unsigned char *pSrc, unsigned int len, unsigned char crc5 );
unsigned char hexin_calc_crc6_30( const unsigned char *pSrc, unsigned int len, unsigned char crc6 );
unsigned char hexin_calc_crc6_2f( const unsigned char *pSrc, unsigned int len, unsigned char crc6 );
unsigned char hexin_calc_crc6_19( const unsigned char *pSrc, unsigned int len, unsigned char crc6 );
unsigned char hexin_calc_crc7_09( const unsigned char *pSrc, unsigned int len, unsigned char crc7 );
unsigned char hexin_calc_crc7_45( const unsigned char *pSrc, unsigned int len, unsigned char crc7 );
unsigned char hexin_calc_crc7_4f( const unsigned char *pSrc, unsigned int len, unsigned char crc7 );
#endif //__CRCX_TABLES_H__
