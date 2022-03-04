/*
*********************************************************************************************************
*                              		(c) Copyright 2020-2022, Hexin
*                                           All Rights Reserved
* File    : _canxtables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-21 [Heyn] Initialize.
*                       2020-04-27 [Heyn] Optimized Code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*
*********************************************************************************************************
*/

#ifndef __CANX_TABLES_H__
#define __CANX_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 HEXIN_CANX_WIDTH                        32
#define                 HEXIN_REFIN_REFOUT_IS_TRUE(x)           ( ( x->refin == TRUE ) && ( x->refout == TRUE ) )
#define                 HEXIN_GRADUAL_CALCULATE_IS_TRUE(x)      ( x->is_gradual == 2 )

#define                 CAN15_POLYNOMIAL_00004599               0x00004599L
#define                 CAN17_POLYNOMIAL_0001685B               0x0001685BL
#define                 CAN21_POLYNOMIAL_00102899               0x00102899L

struct _hexin_canx {
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

unsigned int hexin_canx_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_canx *param, unsigned int init );

#endif //__CANX_TABLES_H__
