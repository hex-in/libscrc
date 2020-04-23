/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc64tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-13 [Heyn] Initialize
*                       2020-04-23 [Heyn] New add we() and xz() functions.
*
*   SEE : http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat-bits.64
*
*********************************************************************************************************
*/

#include "_crc64tables.h"

unsigned long long	    crc64_table_iso[     MAX_TABLE_ARRAY ]  = { 0x0000000000000000L };
unsigned long long      crc64_table_ecma182[ MAX_TABLE_ARRAY ]  = { 0x0000000000000000L };
unsigned long long      crc64_table_xz[      MAX_TABLE_ARRAY ]  = { 0x0000000000000000L };
unsigned long long      crc64_table_hacker[  MAX_TABLE_ARRAY ]  = { 0x0000000000000000L };

static int				crc64_table_iso_init		            = FALSE;
static int				crc64_table_ecma182_init	            = FALSE;
static int				crc64_table_xz_init	                    = FALSE;
unsigned long long      crc64_table_hacker_init                 = FALSE;


unsigned long long hexin_reverse64( unsigned long long data )
{
    unsigned int i = 0;
    unsigned long long t = 0;
    for ( i=0; i<64; i++ ) {
        t |= ( ( data >> i ) & 0x0000000000000001L ) << ( 63-i );
    }
    return t;
}

unsigned char hexin_crc64_init_table_poly_is_high( unsigned long long polynomial, unsigned long long *table )
{
    unsigned int i = 0, j = 0;
	unsigned long long crc = 0x0000000000000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = ( unsigned long long ) i;
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x0000000000000001L ) {
                crc = ( crc >> 1 ) ^ polynomial;
            } else {
                crc = crc >> 1;
            }
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned char hexin_crc64_init_table_poly_is_low( unsigned long long polynomial, unsigned long long *table )
{
    unsigned int i = 0, j = 0;
    unsigned long long crc = 0x0000000000000000L;
    unsigned long long c   = 0x0000000000000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
		crc = 0;
		c	= ( ( unsigned long long ) i ) << 56;
        for ( j=0; j<8; j++ ) {
			if ( ( crc ^ c ) & 0x8000000000000000L ) {
                crc = ( crc << 1 ) ^ polynomial;
            } else {
                crc = crc << 1;
            }
			c = c << 1;
        }
		table[i] = crc;
    }
	return TRUE;
}

unsigned long long hexin_crc64_poly_is_high_calc( unsigned long long crc64, unsigned char c, const unsigned long long *table )
{
    unsigned long long crc = crc64;
    unsigned long long tmp, long_c;

    long_c = 0x00000000000000FFL & ( unsigned long long ) c;
    tmp = crc ^ long_c;
    crc = ( crc >> 8 ) ^ table[ tmp & 0xFF ];
    return crc;
}

unsigned long long hexin_crc64_poly_is_low_calc( unsigned long long crc64, unsigned char c, const unsigned long long *table )
{
    unsigned long long crc = crc64;
    unsigned long long tmp, long_c;

    long_c = 0x00000000000000FFL & ( unsigned long long ) c;
    tmp = ( crc >> 56 ) ^ long_c;
    crc = ( crc << 8  ) ^ table[ tmp & 0xFF ];
    return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xD800000000000000L [ISO]
*********************************************************************************************************
*/
unsigned long long hexin_calc_crc64_iso( const unsigned char *pSrc, unsigned int len, unsigned long long crc64 ) 
{
    unsigned int i = 0;
    unsigned long long crc = crc64;

    if ( ! crc64_table_iso_init ) {
        crc64_table_iso_init = hexin_crc64_init_table_poly_is_high( CRC64_POLYNOMIAL_ISO, crc64_table_iso );
    }

	for ( i=0; i<len; i++ ) {
        crc = hexin_crc64_poly_is_high_calc( crc, pSrc[i], crc64_table_iso );
    }

	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x42F0E1EBA9EA3693L [ECMA182]
*********************************************************************************************************
*/
unsigned long long hexin_calc_crc64_ecma182( const unsigned char *pSrc, unsigned int len, unsigned long long crc64 )
{
    unsigned int i = 0;
    unsigned long long crc = crc64;

    if ( ! crc64_table_ecma182_init ) {
        crc64_table_ecma182_init = hexin_crc64_init_table_poly_is_low( CRC64_POLYNOMIAL_ECMA182, crc64_table_ecma182 );
    }

	for ( i=0; i<len; i++ ) {
        crc = hexin_crc64_poly_is_low_calc( crc, pSrc[i], crc64_table_ecma182 );
    }
    return crc;
}

unsigned long long hexin_calc_crc64_xz( const unsigned char *pSrc, unsigned int len, unsigned long long crc64 )
{
    unsigned int i = 0;
    unsigned long long crc = crc64;

    if ( ! crc64_table_xz_init ) {
        crc64_table_xz_init = hexin_crc64_init_table_poly_is_high( hexin_reverse64( CRC64_POLYNOMIAL_ECMA182 ), crc64_table_xz );
    }

	for ( i=0; i<len; i++ ) {
        crc = hexin_crc64_poly_is_high_calc( crc, pSrc[i], crc64_table_xz );
    }
    return crc;
}

/*
*********************************************************************************************************
*                                   For hacker
*********************************************************************************************************
*/
static unsigned char hexin_init_crc64_table_hacker( unsigned long long polynomial ) 
{
    if ( crc64_table_hacker_init == polynomial ) {
        return FALSE;
    }

    if ( polynomial & 0x8000000000000000L ) {
        hexin_crc64_init_table_poly_is_high( polynomial, crc64_table_hacker );
    } else {
        hexin_crc64_init_table_poly_is_low(  polynomial, crc64_table_hacker );
    }
    crc64_table_hacker_init = polynomial;
    return TRUE;
}

unsigned long long hexin_calc_crc64_hacker( const unsigned char *pSrc, unsigned int len, unsigned long long crc64, unsigned long long polynomial )
{
    unsigned int i = 0;
    unsigned long long crc = crc64;

    hexin_init_crc64_table_hacker( polynomial );

    switch ( polynomial & 0x8000000000000000L ) {
        case 0x8000000000000000L:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc64_poly_is_high_calc( crc, pSrc[i], crc64_table_hacker );
            }
            break;
        
        default:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc64_poly_is_low_calc(  crc, pSrc[i], crc64_table_hacker );
            }
            break;
    }

	return crc;
}