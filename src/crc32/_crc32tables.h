/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc32tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#ifndef __CRC32_TABLES_H__
#define __CRC32_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 HEXIN_MOD_ADLER                         65521

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x80000000L )
#define                 HEXIN_REFIN_OR_REFOUT_IS_TRUE(x)        ( x == 0x00000001L ? TRUE : FALSE )

#define		            CRC32_POLYNOMIAL_04C11DB7		        0x04C11DB7L
#define		            CRC32_POLYNOMIAL_EDB88320		        0xEDB88320L


unsigned int hexin_reverse32( unsigned int data );

unsigned int hexin_crc32_init_table_poly_is_high( unsigned int polynomial, unsigned int *table );
unsigned int hexin_crc32_init_table_poly_is_low(  unsigned int polynomial, unsigned int *table );

unsigned int hexin_calc_crc32_04c11db7( const unsigned char *pSrc, unsigned int len, unsigned int crc32 );
unsigned int hexin_calc_crc32_edb88320( const unsigned char *pSrc, unsigned int len, unsigned int crc32 );
unsigned int hexin_calc_crc32_hacker(   const unsigned char *pSrc, unsigned int len, unsigned int crc32, unsigned int polynomial );

unsigned int hexin_calc_crc32_adler(    const unsigned char *pSrc, unsigned int len );

#endif //__CRC32_TABLES_H__
