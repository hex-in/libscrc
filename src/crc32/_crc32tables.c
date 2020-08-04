/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2020, Hexin
*                                           All Rights Reserved
* File    : _crc32tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.4
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize
*                       2020-03-20 [Heyn] New add hexin_calc_crc32_adler
*                       2020-03-23 [Heyn] New add hexin_calc_crc32_fletcher
*                       2020-04-26 [Heyn] Optimized Code
*                       2020-08-04 [Heyn] Fixed Issues #4.
*
*********************************************************************************************************
*/

#include "_crc32tables.h"

unsigned int hexin_reverse32( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<32; i++ ) {
        t |= ( ( data >> i ) & 0x00000001 ) << ( 31-i );
    }
    return t;
}

unsigned int hexin_crc32_init_table_poly_is_high( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = i;
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x00000001L ) crc = ( crc >> 1 ) ^ polynomial;
            else                     crc =   crc >> 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crc32_init_table_poly_is_low( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned int )i ) << 24;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x80000000L )  crc = ( crc << 1 ) ^ polynomial;
            else                              crc =   crc << 1;
            c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crc32_poly_is_high_calc( unsigned int crc32, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FFL & (unsigned int) c;
    tmp = crc ^ int_c;
    crc = (crc >> 8) ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned int hexin_crc32_poly_is_low_calc( unsigned int crc32, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FF & (unsigned int) c;
	tmp = (crc >> 24) ^ int_c;
    crc = (crc << 8)  ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned int hexin_calc_crc32_adler( const unsigned char *pSrc, unsigned int len, unsigned int crc32 /*reserved*/ )
{
    unsigned int sum1 = 1, sum2 = 0;
    unsigned int i = 0x00000000L;

    for ( i = 0; i < len; i++ ) {
        sum1 = ( sum1 + pSrc[i] ) % HEXIN_MOD_ADLER;
        sum2 = ( sum2 + sum1 ) % HEXIN_MOD_ADLER;
    }
    return ( sum2 << 16 ) | sum1;
}

unsigned int hexin_calc_crc32_fletcher( const unsigned char *pSrc, unsigned int len, unsigned int crc32 /*reserved*/ )
{
    unsigned long sum1 = 0xFFFF, sum2 = 0xFFFF;

    while ( len > 1 ) {
        sum1 += *( unsigned short * )pSrc;
        sum2 += sum1;
        sum1 = ( sum1 & 0xFFFF ) + ( sum1 >> 16 );
        sum2 = ( sum2 & 0xFFFF ) + ( sum2 >> 16 );
        len  -= 2;
        pSrc += 2;
    }

    if ( len ) {
        sum1 += *(unsigned char *) pSrc;
        sum2 += sum1;
        sum1 = ( sum1 & 0xFFFF ) + ( sum1 >> 16 );
        sum2 = ( sum2 & 0xFFFF ) + ( sum2 >> 16 );
    }

    return ( sum1 & 0xFFFF ) | ( sum2 << 16 );
}

unsigned int hexin_crc32_compute_init_table( struct _hexin_crc32 *param )
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

unsigned int hexin_crc32_compute_char( unsigned int crc32, unsigned char c, struct _hexin_crc32 *param )
{
    unsigned int crc = crc32;

    if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
        crc = (crc >> 8) ^ param->table[ ((crc >> 0 ) ^ (0x000000FFL & (unsigned int)c)) & 0xFF ];
    } else {
        crc = (crc << 8) ^ param->table[ ((crc >> 24) ^ (0x000000FFL & (unsigned int)c)) & 0xFF ];
    }

    return crc;
}

unsigned int hexin_crc32_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc32 *param, unsigned int init )
{
    unsigned int i = 0, result = 0;
    unsigned int crc  = ( init << ( HEXIN_CRC32_WIDTH - param->width ) );       /* Fixed Issues #4  */

    if ( param->is_initial == FALSE ) {
        if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
            param->poly = ( hexin_reverse32( param->poly ) >> ( HEXIN_CRC32_WIDTH - param->width ) );
        } else {
            param->poly = ( param->poly << ( HEXIN_CRC32_WIDTH - param->width ) );
        }
        param->is_initial = hexin_crc32_compute_init_table( param );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc32_compute_char( crc, pSrc[i], param );
	}

    result = ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) ? crc : ( crc >> ( HEXIN_CRC32_WIDTH - param->width ) );
    
	return ( result ^ param->xorout ); 
}
