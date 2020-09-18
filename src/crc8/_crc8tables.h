/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2020, Hexin
*                                           All Rights Reserved
* File    : _crc8tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.5
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2020-09-18 [Heyn] New add lin and lin2x checksum.
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

#define                 HEXIN_CRC8_WIDTH                        16
#define                 HEXIN_REFIN_REFOUT_IS_TRUE(x)           ( ( x->refin == TRUE ) && ( x->refout == TRUE ) )

#define                 HEXIN_POLYNOMIAL_IS_HIGH(x)             ( x & 0x80 )
#define                 HEXIN_GRADUAL_CALCULATE_IS_TRUE(x)      ( x->is_gradual == 2 )

#define CRC8_POLYNOMIAL_07                                      0x07
#define CRC8_POLYNOMIAL_31                                      0x31
#define CRC8_POLYNOMIAL_39                                      0x39
#define CRC8_POLYNOMIAL_49                                      0x49
#define CRC8_POLYNOMIAL_1D                                      0x1D
#define CRC8_POLYNOMIAL_2F                                      0x2F
#define CRC8_POLYNOMIAL_9B                                      0x9B
#define CRC8_POLYNOMIAL_A7                                      0xA7
#define CRC8_POLYNOMIAL_D5                                      0xD5

struct _hexin_crc8 {
    unsigned int   is_initial;
    unsigned int   is_gradual;
    unsigned char  width;
    unsigned char  poly;
    unsigned char  init;
    unsigned int   refin;
    unsigned int   refout;
    unsigned char  xorout;
    unsigned char  result;
    unsigned char  table[MAX_TABLE_ARRAY];
};

unsigned char hexin_reverse8( unsigned char data );

unsigned int hexin_crc8_init_table_poly_is_high( unsigned char polynomial, unsigned char *table );
unsigned int hexin_crc8_init_table_poly_is_low(  unsigned char polynomial, unsigned char *table );

unsigned char hexin_calc_crc8_bcc(      const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_lrc(      const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_sum(      const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_fletcher( const unsigned char *pSrc, unsigned int len, unsigned char crc8 /*reserved*/ );
unsigned char hexin_crc8_compute(       const unsigned char *pSrc, unsigned int len, struct _hexin_crc8 *param, unsigned char init );

unsigned char hexin_calc_crc8_lin(      const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hexin_calc_crc8_lin2x(    const unsigned char *pSrc, unsigned int len, unsigned char crc8 );

#endif //__CRC8_TABLES_H__
