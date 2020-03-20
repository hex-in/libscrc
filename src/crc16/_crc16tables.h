/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc16tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-19 21:01PM [Heyn] New CRC16-X25 Table.
*                       2020-03-13 16:10PM [Heyn] New add hacker code.
*
*********************************************************************************************************
*/

#ifndef __CRC16_TABLES_H__
#define __CRC16_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 CRC16_POLYNOMIAL_8408                   0x8408
#define                 CRC16_POLYNOMIAL_1021                   0x1021
#define                 CRC16_POLYNOMIAL_A001                   0xA001
#define                 CRC16_POLYNOMIAL_8005                   0x8005
#define                 CRC16_POLYNOMIAL_A6BC                   0xA6BC
#define                 CRC16_POLYNOMIAL_91A0                   0x91A0

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x8000 )
#define                 HEXIN_REFIN_OR_REFOUT_IS_TRUE(x)        ( x == 0x0001 ? TRUE : FALSE )


unsigned short hexin_reverse16( unsigned short data );

unsigned int hexin_crc16_init_table_poly_is_high( unsigned short polynomial, unsigned short *table );
unsigned int hexin_crc16_init_table_poly_is_low(  unsigned short polynomial, unsigned short *table );

unsigned short hexin_calc_crc16_91a0( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_8408( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_1021( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_a001( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_8005( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_a6bc( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_sick( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );

unsigned short hexin_calc_crc16_hacker(  const unsigned char *pSrc, unsigned int len, unsigned short crc16, unsigned short polynomial );
unsigned short hexin_calc_crc16_network( const unsigned char *pSrc, unsigned int len );

#endif //__CRC16_TABLES_H__
