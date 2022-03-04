/*
*********************************************************************************************************
*                              		(c) Copyright 2020-2022, Hexin
*                                           All Rights Reserved
* File    : _canxtables.c
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

#include "_canxtables.h"

static unsigned int hexin_canx_reverse32( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<32; i++ ) {
        t |= ( ( data >> i ) & 0x00000001 ) << ( 31-i );
    }
    return t;
}

static unsigned int hexin_canx_compute_init_table( struct _hexin_canx *param )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
        for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
            crc = i;
            for ( j=0; j<8; j++ ) {
                if ( crc & 0x00000001L ) crc = ( crc >> 1 ) ^ param->poly;
                else                     crc = ( crc >> 1 );
            }
            param->table[i] = crc;
        }
    } else {
        for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
            crc = 0;
            c	= ( ( unsigned int )i ) << 24;
            for ( j=0; j<8; j++ ) {
                if ( ( crc ^ c ) & 0x80000000L )  crc = ( crc << 1 ) ^ param->poly;
                else                              crc = ( crc << 1 );
                c = c << 1;
            }
            param->table[i] = crc;
        }
    }
    return TRUE;
}

static unsigned int hexin_canx_compute_char( unsigned int crcx, unsigned char c, struct _hexin_canx *param )
{
    unsigned int crc = crcx;

    if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
        crc = (crc >> 8) ^ param->table[ ((crc >> 0 ) ^ (0x000000FFL & (unsigned int)c)) & 0xFF ];
    } else {
        crc = (crc << 8) ^ param->table[ ((crc >> 24) ^ (0x000000FFL & (unsigned int)c)) & 0xFF ];
    }

    return crc;
}

unsigned int hexin_canx_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_canx *param, unsigned int init )
{
    unsigned int i = 0, result = 0;
    unsigned int crc  = ( init << ( HEXIN_CANX_WIDTH - param->width ) );

    if ( param->is_initial == FALSE ) {
        if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
            param->poly = ( hexin_canx_reverse32( param->poly ) >> ( HEXIN_CANX_WIDTH - param->width ) );
        } else {
            param->poly = ( param->poly << ( HEXIN_CANX_WIDTH - param->width ) );
        }
        param->is_initial = hexin_canx_compute_init_table( param );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_canx_compute_char( crc, pSrc[i], param );
	}

    result = ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) ? crc : ( crc >> ( HEXIN_CANX_WIDTH - param->width ) );
    
	return ( result ^ param->xorout ); 
}
