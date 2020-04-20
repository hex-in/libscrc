/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc24tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-17 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#ifndef __CRC24_TABLES_H__
#define __CRC24_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x800000L )
#define                 HEXIN_REFIN_OR_REFOUT_IS_TRUE(x)        ( x == 0x000001L ? TRUE : FALSE )


#define                 CRC24_POLYNOMIAL_00065B                 0x00065B
#define                 CRC24_POLYNOMIAL_5D6DCB                 0x5D6DCB
#define                 CRC24_POLYNOMIAL_864CFB                 0x864CFB
#define                 CRC24_POLYNOMIAL_800063                 0x800063
#define                 CRC24_POLYNOMIAL_328B63                 0x328B63


unsigned int hexin_reverse24( unsigned int data );
unsigned int hexin_calc_crc24_hacker( const unsigned char *pSrc, unsigned int len, unsigned int crc24, unsigned int polynomial );
unsigned int hexin_calc_crc24_00065b( const unsigned char *pSrc, unsigned int len, unsigned int crc24 );
unsigned int hexin_calc_crc24_5d6dcb( const unsigned char *pSrc, unsigned int len, unsigned int crc24 );
unsigned int hexin_calc_crc24_864cfb( const unsigned char *pSrc, unsigned int len, unsigned int crc24 );
unsigned int hexin_calc_crc24_800063( const unsigned char *pSrc, unsigned int len, unsigned int crc24 );
unsigned int hexin_calc_crc24_328b63( const unsigned char *pSrc, unsigned int len, unsigned int crc24 );

#endif //__CRC24_TABLES_H__
