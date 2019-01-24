/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crc16tables.h
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.0
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-19 21:01PM [Heyn] New CRC16-X25 Table.
*
*********************************************************************************************************
*/

#ifndef __CRC16_TABLES_H__
#define __CRC16_TABLES_H__

#define                 TRUE                                    1
#define                 FALSE                                   0

#define                 MAX_TABLE_ARRAY                         256

#define                 CRC16_POLYNOMIAL_8408                   0x8408
#define                 CRC16_POLYNOMIAL_1021                   0x1021
#define                 CRC16_POLYNOMIAL_A001                   0xA001
#define                 CRC16_POLYNOMIAL_8005                   0x8005
#define                 CRC16_POLYNOMIAL_A6BC                   0xA6BC
#define                 CRC16_POLYNOMIAL_91A0                   0x91A0

unsigned short hz_calc_crc16_8408( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hz_calc_crc16_1021( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hz_calc_crc16_a001( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hz_calc_crc16_8005( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hz_calc_crc16_sick( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hz_calc_crc16_a6bc( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );
unsigned short hz_calc_crc16_91a0( const unsigned char *pSrc, unsigned int len, unsigned short crc16 );

#endif //__CRC16_TABLES_H__
