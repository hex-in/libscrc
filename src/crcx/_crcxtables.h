/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2020, Hexin
*                                           All Rights Reserved
* File    : _crcxtables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.3
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized code.
*                       2020-04-27 [Heyn] Optimized code.
*
*********************************************************************************************************
*/

#ifndef __CRCX_TABLES_H__
#define __CRCX_TABLES_H__

#ifndef TRUE
#define                 TRUE                                    1
#endif

#ifndef FALSE
#define                 FALSE                                   0
#endif

#ifndef MAX_TABLE_ARRAY
#define                 MAX_TABLE_ARRAY                         256
#endif

#define                 HEXIN_CRCX_WIDTH                        16
#define                 HEXIN_REFIN_REFOUT_IS_TRUE(x)           ( ( x->refin == TRUE ) && ( x->refout == TRUE ) )


struct _hexin_crcx {
    unsigned int    is_initial;
    unsigned short  width;
    unsigned short  poly;
    unsigned short  init;
    unsigned int    refin;
    unsigned int    refout;
    unsigned short  xorout;
    unsigned short  result;
    unsigned short  table[MAX_TABLE_ARRAY];
};

unsigned short hexin_crcx_reverse12( unsigned short data );
unsigned short hexin_crcx_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crcx *param );

#endif //__CRCX_TABLES_H__
