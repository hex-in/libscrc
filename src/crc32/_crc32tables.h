/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc32tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize.
*                       2020-04-26 [Heyn] Optimized Code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
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

#define                 HEXIN_CRC32_WIDTH                       32

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x80000000L )
#define                 HEXIN_REFIN_IS_TRUE(x)                  ( x->refin  == TRUE )
#define                 HEXIN_REFOUT_IS_TRUE(x)                 ( x->refout == TRUE )
#define                 HEXIN_REFIN_REFOUT_IS_TRUE(x)           ( ( x->refin == TRUE ) && ( x->refout == TRUE ) )
#define                 HEXIN_GRADUAL_CALCULATE_IS_TRUE(x)      ( x->is_gradual == 2 )

#define                 CRC30_POLYNOMIAL_2030B9C7               0x2030B9C7L
#define                 CRC31_POLYNOMIAL_04C11DB7               0x04C11DB7L
#define		            CRC32_POLYNOMIAL_04C11DB7		        0x04C11DB7L


struct _hexin_crc32 {
    unsigned int  is_initial;
    unsigned int  is_gradual;
    unsigned int  width;
    unsigned int  poly;
    unsigned int  init;
    unsigned int  refin;
    unsigned int  refout;
    unsigned int  xorout;
    unsigned int  result;
    unsigned int  table[MAX_TABLE_ARRAY];
};


unsigned int hexin_reverse32( unsigned int data );

unsigned int hexin_crc32_init_table_poly_is_high( unsigned int polynomial, unsigned int *table );
unsigned int hexin_crc32_init_table_poly_is_low(  unsigned int polynomial, unsigned int *table );

unsigned int hexin_calc_crc32_adler(    const unsigned char *pSrc, unsigned int len, unsigned int crc32 /*reserved*/ );
unsigned int hexin_calc_crc32_fletcher( const unsigned char *pSrc, unsigned int len, unsigned int crc32 /*reserved*/ );
unsigned int hexin_crc32_compute(       const unsigned char *pSrc, unsigned int len, struct _hexin_crc32 *param, unsigned int init );
unsigned int hexin_crc32_compute_stm32( const unsigned char *pSrc, unsigned int len, struct _hexin_crc32 *param, unsigned int init );

#endif //__CRC32_TABLES_H__
