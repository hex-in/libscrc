/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crcxtables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*       WebTool : http://reveng.sourceforge.net/crc-catalogue/1-15.htm#crc.cat-bits.4
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized code.
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
*
*********************************************************************************************************
*/

#include "stdio.h"
#include "_crcxtables.h"

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

unsigned short hexin_crcx_reverse12( unsigned short data )
{
    unsigned int   i = 0;
    unsigned short t = 0;
    for ( i=0; i<12; i++ ) {
        t |= ( ( data >> i ) & 0x0001 ) << ( 11-i );
    }
    return t;
}

static unsigned short hexin_crcx_reverse16( unsigned short data )
{
    unsigned int   i = 0;
    unsigned short t = 0;
    for ( i=0; i<16; i++ ) {
        t |= ( ( data >> i ) & 0x0001 ) << ( 15-i );
    }
    return t;
}

static unsigned int hexin_crcx_compute_init_table( struct _hexin_crcx *param )
{
    unsigned int i = 0, j = 0;
    unsigned short crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = ( ( unsigned short ) i ) << 8;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x8000 ) crc = ( crc << 1 ) ^ param->poly;
            else                      crc =   crc << 1;
            c = c << 1;
        }
        param->table[i] = crc;
    }

    return TRUE;
}

static unsigned short hexin_crcx_compute_char( unsigned short crcx, unsigned char c, struct _hexin_crcx *param )
{
    unsigned short crc = crcx;
    crc = (crc << 8) ^ param->table[ ((crc >> 8 ) ^ (0x00FF & (unsigned short)c)) & 0xFF ];
    return crc;
}

unsigned short hexin_crcx_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crcx *param, unsigned short init )
{
    unsigned int i = 0, result = 0;
    unsigned short crc  = ( init << ( HEXIN_CRCX_WIDTH - param->width ) );

    if ( param->is_initial == FALSE ) {
        param->poly = ( param->poly << ( HEXIN_CRCX_WIDTH - param->width ) );
        param->is_initial = hexin_crcx_compute_init_table( param );
    }

    /* Fixed Issues #4 8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) && ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) ) { 
        crc = hexin_crcx_reverse16( init );
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFIN_IS_TRUE( param ) ) {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crcx_compute_char( crc, __hexin_reverse8(pSrc[i]), param );
        }
    } else {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crcx_compute_char( crc, pSrc[i], param );
        }
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) ) {
        crc = hexin_crcx_reverse16( crc );
    }

    result = ( HEXIN_REFIN_IS_TRUE( param ) || HEXIN_REFOUT_IS_TRUE( param ) ) ? crc : ( crc >> (HEXIN_CRCX_WIDTH - param->width) );

	return ( result ^ param->xorout );
}
