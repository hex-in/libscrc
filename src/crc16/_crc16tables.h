/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc16tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-19 21:01PM [Heyn] New CRC16-X25 Table.
*                       2020-03-13 16:10PM [Heyn] New add hacker code.
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
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

#define                 HEXIN_CRC16_WIDTH                       16

#define                 CRC16_POLYNOMIAL_1021                   0x1021
#define                 CRC16_POLYNOMIAL_8005                   0x8005
#define                 CRC16_POLYNOMIAL_0589                   0x0589
#define                 CRC16_POLYNOMIAL_1DCF                   0x1DCF
#define                 CRC16_POLYNOMIAL_3D65                   0x3D65
#define                 CRC16_POLYNOMIAL_C867                   0xC867

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x8000 )
#define                 HEXIN_REFIN_IS_TRUE(x)                  ( x->refin  == TRUE )
#define                 HEXIN_REFOUT_IS_TRUE(x)                 ( x->refout == TRUE )
#define                 HEXIN_REFIN_REFOUT_IS_TRUE(x)           ( ( x->refin == TRUE ) && ( x->refout == TRUE ) )
#define                 HEXIN_GRADUAL_CALCULATE_IS_TRUE(x)      ( x->is_gradual == 2 )

struct _hexin_crc16 {
    unsigned int    is_initial;
    unsigned int    is_gradual;
    unsigned short  width;
    unsigned short  poly;
    unsigned short  init;
    unsigned int    refin;
    unsigned int    refout;
    unsigned short  xorout;
    unsigned short  result;
    unsigned short  table[MAX_TABLE_ARRAY];
};

unsigned short hexin_reverse16( unsigned short data );

unsigned int hexin_crc16_init_table_poly_is_high( unsigned short polynomial, unsigned short *table );
unsigned int hexin_crc16_init_table_poly_is_low(  unsigned short polynomial, unsigned short *table );

unsigned short hexin_calc_crc16_sick(     const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hexin_calc_crc16_network(  const unsigned char *pSrc, unsigned int len, unsigned short crc16 /*reserved*/ );
unsigned short hexin_calc_crc16_fletcher( const unsigned char *pSrc, unsigned int len, unsigned short crc16 /*reserved*/ );

unsigned short hexin_crc16_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc16 *param, unsigned short init );


#endif //__CRC16_TABLES_H__
