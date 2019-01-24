/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crcxtables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.0
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*
*********************************************************************************************************
*/

#ifndef __CRCX_TABLES_H__
#define __CRCX_TABLES_H__

#define                 TRUE                                    1
#define                 FALSE                                   0

#define                 MAX_TABLE_ARRAY                         256

unsigned char hz_calc_crc4_0c( const unsigned char *pSrc, unsigned int len, unsigned char crc8 );
unsigned char hz_calc_crc5_15( const unsigned char *pSrc, unsigned int len, unsigned char crc5 );
unsigned char hz_calc_crc5_48( const unsigned char *pSrc, unsigned int len, unsigned char crc5 );
unsigned char hz_calc_crc5_14( const unsigned char *pSrc, unsigned int len, unsigned char crc5 );
unsigned char hz_calc_crc6_30( const unsigned char *pSrc, unsigned int len, unsigned char crc6 );
unsigned char hz_calc_crc7_12( const unsigned char *pSrc, unsigned int len, unsigned char crc7 );

#endif //__CRCX_TABLES_H__
