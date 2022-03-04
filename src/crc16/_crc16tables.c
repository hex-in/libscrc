/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc16tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.7
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-20 21:57PM [Heyn]
*                       WebTool : http://reveng.sourceforge.net/crc-catalogue/16.htm#crc.cat-bits.16
*                       
*                       2020-03-13 [Heyn] New add hacker code.
*                       2020-03-20 [Heyn] New add hexin_calc_crc16_network.
*                       2020-04-27 [Heyn] Optimized code.
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2021-06-07 [Heyn] Fixed Issues #8.
*
*********************************************************************************************************
*/

#include "_crc16tables.h"

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

unsigned short hexin_reverse16( unsigned short data )
{
    unsigned int   i = 0;
    unsigned short t = 0;
    for ( i=0; i<16; i++ ) {
        t |= ( ( data >> i ) & 0x0001 ) << ( 15-i );
    }
    return t;
}

unsigned int hexin_crc16_init_table_poly_is_high( unsigned short polynomial, unsigned short *table )
{
    unsigned int i = 0, j = 0;
    unsigned short crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = ( unsigned short ) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x0001 )   crc = ( crc >> 1 ) ^ polynomial;
            else                        crc = crc >> 1;
            c = c >> 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crc16_init_table_poly_is_low( unsigned short polynomial, unsigned short *table )
{
    unsigned int i = 0, j = 0;
    unsigned short crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = ((unsigned short) i) << 8;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x8000 ) crc = ( crc << 1 ) ^ polynomial;
            else                      crc = crc << 1;
            c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned short hexin_crc16_poly_is_high_calc( unsigned short crc16, unsigned char c, const unsigned short *table )
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c = 0x00FF & (unsigned short) c;
    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned short hexin_crc16_poly_is_low_calc( unsigned short crc16, unsigned char c, const unsigned short *table )
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c  = 0x00FF & (unsigned short) c;
    tmp = (crc >> 8) ^ short_c;
    crc = (crc << 8) ^ table[tmp];

    return crc;
}

static unsigned short __hexin_crc16_sick( unsigned short crc16, unsigned char c, char prev_byte ) 
{
    unsigned short crc = crc16;
    unsigned short short_c, short_p;

    short_c  =   0x00FF & ( unsigned short ) c;
    short_p  = ( 0x00FF & ( unsigned short ) prev_byte ) << 8;

    if ( crc & 0x8000 ) crc = ( crc << 1 ) ^ CRC16_POLYNOMIAL_8005;
    else                crc =   crc << 1;

    crc &= 0xFFFF;
    crc ^= ( short_c | short_p );

    return crc;
}

unsigned short hexin_calc_crc16_sick( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int   i            = 0;
			 char  prev_byte	= 0x00;
	unsigned short crc		    = crc16;

	for ( i=0; i<len; i++ ) {
		crc	        = __hexin_crc16_sick( crc, pSrc[i], prev_byte );
		prev_byte	= pSrc[i];
    }

	return crc;
}

unsigned short hexin_calc_crc16_network( const unsigned char *pSrc, unsigned int len, unsigned short crc16 /*reserved*/ )
{
    unsigned int sum = 0;

    while ( len > 1 ) {
        sum  += *( unsigned short * )pSrc;
        len  -= 2;
        pSrc += 2;
    }
 
    if ( len ) {
        sum += *(unsigned char *) pSrc;
    }
 
    while ( sum >> 16 ) {
        sum = ( sum >> 16 ) + ( sum & 0xFFFF );
    }
 
    return ( unsigned short )( ~sum );
}

unsigned short hexin_calc_crc16_fletcher( const unsigned char *pSrc, unsigned int len, unsigned short crc16 /*reserved*/ )
{
    unsigned int i = 0;
    unsigned short sum1 = 0, sum2 = 0;

    for ( i = 0; i < len; i++ ) {
        sum1 = ( sum1 + pSrc[i] ) % 255;
        sum2 = ( sum2 + sum1 ) % 255;
    }
    return ( sum1 & 0xFF ) | ( sum2 << 8 );
}

static unsigned int hexin_crc16_compute_init_table( struct _hexin_crc16 *param )
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

static unsigned short hexin_crc16_compute_char( unsigned short crc16, unsigned char c, struct _hexin_crc16 *param )
{
    unsigned short crc = crc16;
    crc = (crc << 8) ^ param->table[ ((crc >> 8 ) ^ (0x00FF & (unsigned short)c)) & 0xFF ];
    return crc;
}

unsigned short hexin_crc16_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc16 *param, unsigned short init )
{
    unsigned int i = 0;
    unsigned short crc = init;              /* Fixed Issues #4  */

    if ( param->is_initial == FALSE ) {
        param->is_initial = hexin_crc16_compute_init_table( param );
    }

    /* Fixed Issues #4 #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) && ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) ) { 
        crc = hexin_reverse16( init );
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFIN_IS_TRUE( param ) ) {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc16_compute_char( crc, __hexin_reverse8( pSrc[i] ), param );
        }
    } else {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc16_compute_char( crc, pSrc[i], param );
        }
    }

    /* Fixed Issues #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) ) {
        crc = hexin_reverse16( crc );
    }

	return ( crc ^ param->xorout );
}
