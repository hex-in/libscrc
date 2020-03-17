/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crcxtables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized Code.
*
*********************************************************************************************************
*/

#include "_crcxtables.h"

static unsigned char    crc4_table_0c[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for ITU
static unsigned char    crc5_table_15[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for ITU
static unsigned char    crc5_table_48[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for EPC
static unsigned char    crc5_table_14[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for USB
static unsigned char    crc6_table_30[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for ITU
static unsigned char    crc7_table_12[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for MMC, SD

static int              crc4_table_0c_init               = FALSE;
static int              crc5_table_15_init               = FALSE;
static int              crc5_table_48_init               = FALSE;
static int              crc5_table_14_init               = FALSE;
static int              crc6_table_30_init               = FALSE;
static int              crc7_table_12_init               = FALSE;


unsigned int hexin_crcx_init_table_poly_is_high( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x01 )   crc = ( crc >> 1 ) ^ polynomial;
            else                      crc =   crc >> 1;
            c = c >> 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crcx_init_table_poly_is_low( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ polynomial;
			else                    crc =   crc << 1;
			c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned char hexin_crcx_poly_calc( unsigned char crcx, unsigned char c, const unsigned char *table )
{
    return table[ crcx ^ c ];
}

unsigned int hexin_crcx_init_table( enum hexin_crcx_mask mask, unsigned char polynomial, unsigned char *table )
{
    unsigned int result = FALSE;

    if ( mask & polynomial ) {
        result = hexin_crcx_init_table_poly_is_high( polynomial, table );
    } else {
        result = hexin_crcx_init_table_poly_is_low ( polynomial, table );
    }
    return result;
}

/*
*********************************************************************************************************
                                    POLY=0x0C [ITU4]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc4_0c( const unsigned char *pSrc, unsigned int len, unsigned char crc4 )
{
    unsigned int i = 0;
    unsigned char crc = crc4;

    if ( crc4_table_0c_init == FALSE ) {
        crc4_table_0c_init = hexin_crcx_init_table( HEXIN_CRC4_MASK, 0x0C, crc4_table_0c );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc4_table_0c );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x15 [ITU5]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc5_15( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

    if ( crc5_table_15_init == FALSE ) {
        crc5_table_15_init = hexin_crcx_init_table( HEXIN_CRC5_MASK, 0x15, crc5_table_15 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc5_table_15 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x48 [EPC]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc5_48( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

    if ( crc5_table_48_init == FALSE ) {
        crc5_table_48_init = hexin_crcx_init_table( HEXIN_CRC5_MASK, 0x48, crc5_table_48 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc5_table_48 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x14 [USB]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc5_14( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

    if ( crc5_table_14_init == FALSE ) {
        crc5_table_14_init = hexin_crcx_init_table( HEXIN_CRC5_MASK, 0x14, crc5_table_14 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc5_table_14 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x30 [ITU]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc6_30( const unsigned char *pSrc, unsigned int len, unsigned char crc6 )
{
    unsigned int i = 0;
    unsigned char crc = crc6;

    if ( crc6_table_30_init == FALSE ) {
        crc6_table_30_init = hexin_crcx_init_table( HEXIN_CRC6_MASK, 0x30, crc6_table_30 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc6_table_30 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x12 [MMC]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc7_12( const unsigned char *pSrc, unsigned int len, unsigned char crc7 )
{
    unsigned int i = 0;
    unsigned char crc = crc7;

    if ( crc7_table_12_init == FALSE ) {
        crc7_table_12_init = hexin_crcx_init_table( HEXIN_CRC7_MASK, 0x30, crc7_table_12 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc7_table_12 );
	}
	return crc;
}
