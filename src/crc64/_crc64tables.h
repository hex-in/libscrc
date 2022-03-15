/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc64tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-13 [Heyn] Initialize.
*                       2020-04-23 [Heyn] New add we() and xz() functions.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
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

#define                 HEXIN_CRC64_WIDTH                       64

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x8000000000000000L )
#define                 HEXIN_REFIN_IS_TRUE(x)                  ( x->refin  == TRUE )
#define                 HEXIN_REFOUT_IS_TRUE(x)                 ( x->refout == TRUE )
#define                 HEXIN_REFIN_REFOUT_IS_TRUE(x)           ( ( x->refin == TRUE ) && ( x->refout == TRUE ) )
#define                 HEXIN_GRADUAL_CALCULATE_IS_TRUE(x)      ( x->is_gradual == 2 )

#define		            CRC64_POLYNOMIAL_ECMA182                0x42F0E1EBA9EA3693L


struct _hexin_crc64 {
    unsigned int  is_initial;
    unsigned int  is_gradual;
    unsigned int  width;
    unsigned long long  poly;
    unsigned long long  init;
    unsigned int  refin;
    unsigned int  refout;
    unsigned long long  xorout;
    unsigned long long  result;
    unsigned long long  table[MAX_TABLE_ARRAY];
};


unsigned long long hexin_reverse64( unsigned long long data );

unsigned char hexin_crc64_init_table_poly_is_high( unsigned long long polynomial, unsigned long long *table );
unsigned char hexin_crc64_init_table_poly_is_low(  unsigned long long polynomial, unsigned long long *table );

unsigned long long hexin_crc64_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc64 *param, unsigned long long init );

#endif //__CRC64_TABLES_H__
