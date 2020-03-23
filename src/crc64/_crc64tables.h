/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc64tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-13 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#ifndef __CRC64_TABLES_H__
#define __CRC64_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x8000000000000000L )
#define                 HEXIN_REFIN_OR_REFOUT_IS_TRUE(x)        ( x == 0x0000000000000001L ? TRUE : FALSE )

#define		            CRC64_POLYNOMIAL_ISO                    0xD800000000000000L
#define		            CRC64_POLYNOMIAL_ECMA182                0x42F0E1EBA9EA3693L

unsigned long long hexin_reverse64( unsigned long long data );

unsigned char hexin_crc64_init_table_poly_is_high( unsigned long long polynomial, unsigned long long *table );
unsigned char hexin_crc64_init_table_poly_is_low(  unsigned long long polynomial, unsigned long long *table );

unsigned long long hexin_calc_crc64_iso(     const unsigned char *pSrc, unsigned int len, unsigned long long crc64 );
unsigned long long hexin_calc_crc64_ecma182( const unsigned char *pSrc, unsigned int len, unsigned long long crc64 );
unsigned long long hexin_calc_crc64_hacker(  const unsigned char *pSrc, unsigned int len, unsigned long long crc64, unsigned long long polynomial );


#endif //__CRC64_TABLES_H__
