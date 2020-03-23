/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc8tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#ifndef __CRC8_TABLES_H__
#define __CRC8_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x80 )
#define                 HEXIN_REFIN_OR_REFOUT_IS_TRUE(x)        ( x == 0x01 ? TRUE : FALSE )


#define CRC8_POLYNOMIAL_31                                      0x31
#define CRC8_POLYNOMIAL_07                                      0x07

unsigned char hexin_reverse8( unsigned char data );

unsigned int hexin_crc8_init_table_poly_is_high( unsigned char polynomial, unsigned char *table );
unsigned int hexin_crc8_init_table_poly_is_low(  unsigned char polynomial, unsigned char *table );

unsigned char hexin_calc_crc8_bcc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_lrc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_sum( const unsigned char *pSrc, unsigned int len, unsigned char crc8 );

unsigned char hexin_calc_crc8_maxim( const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_rohc(  const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_07(    const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_hacker(const unsigned char *pSrc, unsigned int len, unsigned char crc8, unsigned char polynomial );

unsigned char hexin_calc_crc8_fletcher( const unsigned char *pSrc, unsigned int len );

#endif //__CRC8_TABLES_H__
