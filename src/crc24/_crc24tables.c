/*
*********************************************************************************************************
*                              		(c) Copyright 2020-2022, Hexin
*                                           All Rights Reserved
* File    : _crc24tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-17 [Heyn] Initialize.
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
*
*********************************************************************************************************
*/

#include "_crc24tables.h"

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

static unsigned int hexin_reverse24( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<24; i++ ) {
        t |= ( ( data >> i ) & 0x00000001 ) << ( 23-i );
    }
    return t;
}

unsigned int hexin_crc24_compute_init_table( struct _hexin_crc24 *param )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned int )i ) << 16;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x00800000L )  crc = ( crc << 1 ) ^ param->poly;
            else                              crc = ( crc << 1 );
            c = c << 1;
        }
        param->table[i] = crc;
    }

    return TRUE;
}

unsigned int hexin_crc24_compute_char( unsigned int crc24, unsigned char c, struct _hexin_crc24 *param )
{
    unsigned int crc = crc24;
    crc = (crc << 8) ^ param->table[ ((crc >> 16) ^ (0x000000FFL & (unsigned int)c)) & 0xFF ];
    return crc;
}

unsigned int hexin_crc24_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc24 *param, unsigned int init )
{
    unsigned int i = 0;
    unsigned int crc = init;

    if ( param->is_initial == FALSE ) {
        param->is_initial = hexin_crc24_compute_init_table( param );
    }

    /* Fixed Issues #4 8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) && ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) ) { 
        crc = hexin_reverse24( init );
    }

    if ( HEXIN_REFIN_IS_TRUE( param ) ) {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc24_compute_char( crc, __hexin_reverse8(pSrc[i]), param );
        }
    } else {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc24_compute_char( crc, pSrc[i], param );
        }
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) ) {
        crc = hexin_reverse24( crc );
    }
    
	return ( ( crc & 0xFFFFFF ) ^ param->xorout ); 
}
