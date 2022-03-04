/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc64tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-13 [Heyn] Initialize
*                       2020-04-23 [Heyn] New add we() and xz() functions.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
*
*   SEE : http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat-bits.64
*
*********************************************************************************************************
*/

#include "_crc64tables.h"

/* Fixed Issues #8 */
static unsigned char __hexin_reverse8( unsigned char data )
{
    unsigned int  i = 0;
    unsigned char t = 0;
    for ( i=0; i<8; i++ ) {
        t |= ( ( data >> i ) & 0x01 ) << ( 7-i );
    }
    return t;
}

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

static unsigned int hexin_crc64_compute_init_table( struct _hexin_crc64 *param )
{
    unsigned int i = 0, j = 0;
    unsigned long long crc = 0x0000000000000000L;
    unsigned long long c   = 0x0000000000000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned long long ) i ) << 56;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x8000000000000000L ) {
                crc = ( crc << 1 ) ^ param->poly;
            } else { 
                crc = ( crc << 1 );
            }
            c = c << 1;
        }
        param->table[i] = crc;
    }

    return TRUE;
}

static unsigned long long hexin_crc64_compute_char( unsigned long long crc64, unsigned char c, struct _hexin_crc64 *param )
{
    unsigned long long crc = crc64;
    crc = (crc << 8) ^ param->table[ ((crc >> 56) ^ (0x00000000000000FFL & (unsigned long long)c)) & 0xFF ];
    return crc;
}

unsigned long long hexin_crc64_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc64 *param, unsigned long long init )
{
    unsigned int i = 0;
    unsigned long long result = 0;
    unsigned long long crc  = ( init << ( HEXIN_CRC64_WIDTH - param->width ) );

    if ( param->is_initial == FALSE ) {
        param->poly = ( param->poly << ( HEXIN_CRC64_WIDTH - param->width ) );
        param->is_initial = hexin_crc64_compute_init_table( param );
    }

    /* Fixed Issues #4 #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) && ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) ) { 
        crc = hexin_reverse64( init );
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFIN_IS_TRUE( param ) ) {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc64_compute_char( crc, __hexin_reverse8(pSrc[i]), param );
        }
    } else {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc64_compute_char( crc, pSrc[i], param );
        }
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) ) {
        crc = hexin_reverse64( crc );
    }

    result = ( crc >> ( HEXIN_CRC64_WIDTH - param->width ) );
	return ( result ^ param->xorout ); 
}
