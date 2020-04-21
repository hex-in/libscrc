/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _canxtables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-21 [Heyn] Initialize.
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


#define                 CAN15_POLYNOMIAL_00004599               0x00004599L
#define                 CAN17_POLYNOMIAL_0001685B               0x0001685BL
#define                 CAN21_POLYNOMIAL_00102899               0x00102899L


unsigned int hexin_reverse15( unsigned int data );
unsigned int hexin_reverse17( unsigned int data );
unsigned int hexin_reverse21( unsigned int data );

unsigned int hexin_calc_can15( const unsigned char *pSrc, unsigned int len, unsigned int crc32 );
unsigned int hexin_calc_can17( const unsigned char *pSrc, unsigned int len, unsigned int crc32 );
unsigned int hexin_calc_can21( const unsigned char *pSrc, unsigned int len, unsigned int crc32 );

#endif //__CANX_TABLES_H__
