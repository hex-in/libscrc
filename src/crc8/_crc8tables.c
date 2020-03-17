/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc8tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize
*
*********************************************************************************************************
*/

#include "_crc8tables.h"

static unsigned char	crc8_table_maxim[MAX_TABLE_ARRAY]   = { 0x00 };
static unsigned char	crc8_table_rohc [MAX_TABLE_ARRAY]   = { 0x00 };
static unsigned char	crc8_table_07[MAX_TABLE_ARRAY]      = { 0x00 };

static unsigned int     crc8_table_maxim_init               = FALSE;
static unsigned int     crc8_table_rohc_init                = FALSE;
static unsigned int     crc8_table_07_init                  = FALSE;


static unsigned char	crc8_table_hacker[MAX_TABLE_ARRAY]  = { 0x00 };
static unsigned char    crc8_table_hacker_init              = FALSE;

unsigned char hexin_reverse8( unsigned char data )
{
    unsigned int  i = 0;
    unsigned char t = 0;
    for ( i=0; i<8; i++ ) {
        t |= ( ( data >> i ) & 0x01 ) << ( 7-i );
    }
    return t;
}

unsigned int hexin_crc8_init_table_poly_is_high( unsigned char polynomial, unsigned char *table )
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

unsigned int hexin_crc8_init_table_poly_is_low( unsigned char polynomial, unsigned char *table )
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

unsigned char hexin_crc8_poly_calc( unsigned char crc8, unsigned char c, const unsigned char *table )
{
    return table[ crc8 ^ c ];
}

unsigned char hexin_calc_crc8_bcc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc ^= pSrc[i];
    }

	return crc;
}

unsigned char hexin_calc_crc8_lrc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc += pSrc[i];
	}
    crc = (~crc) + 0x01;

	return crc;
}

unsigned char hexin_calc_crc8_sum( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc += pSrc[i];
	}
	return crc;
}

/*
*********************************************************************************************************
*                                   POLY=0x31 [MAXIM8]
* Poly:    0x31
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Alias:   DOW-CRC,CRC-8/IBUTTON
* Use:     Maxim(Dallas)'s some devices,e.g. DS18B20
*********************************************************************************************************
*/

unsigned char hexin_calc_crc8_maxim( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

    if ( crc8_table_maxim_init == FALSE ) {
        crc8_table_maxim_init = hexin_crc8_init_table_poly_is_high( hexin_reverse8( CRC8_POLYNOMIAL_31 ), crc8_table_maxim );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc8_poly_calc( crc, pSrc[i], crc8_table_maxim );
	}
	return crc;
}

/*
*********************************************************************************************************
*                                   POLY=0x07 [ROHC]
* Poly:    0x07
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
*********************************************************************************************************
*/

unsigned char hexin_calc_crc8_rohc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

    if ( crc8_table_rohc_init == FALSE ) {
        crc8_table_rohc_init = hexin_crc8_init_table_poly_is_high( hexin_reverse8( CRC8_POLYNOMIAL_07 ), crc8_table_rohc );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc8_poly_calc( crc, pSrc[i], crc8_table_rohc );
	}
	return crc;
}

/*
*********************************************************************************************************
*                                   POLY=0x07
* Poly:    0x07
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x00
*********************************************************************************************************
*/

unsigned char hexin_calc_crc8_07( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

    if ( crc8_table_07_init == FALSE ) {
        crc8_table_07_init = hexin_crc8_init_table_poly_is_low( CRC8_POLYNOMIAL_07, crc8_table_07 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc8_poly_calc( crc, pSrc[i], crc8_table_07 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    For hacker
*********************************************************************************************************
*/

static unsigned char hexin_init_crc8_table_hacker( unsigned char polynomial  ) 
{
    if ( crc8_table_hacker_init == polynomial ) {
        return FALSE;
    }

    if ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        hexin_crc8_init_table_poly_is_high( polynomial, crc8_table_hacker );
    } else {
        hexin_crc8_init_table_poly_is_low(  polynomial, crc8_table_hacker );
    }
    crc8_table_hacker_init = polynomial;

    return TRUE;
}

unsigned char hexin_calc_crc8_hacker( const unsigned char *pSrc, unsigned int len, unsigned char crc8, unsigned char polynomial )
{
    unsigned int i = 0;
    unsigned char crc = crc8;

    hexin_init_crc8_table_hacker( polynomial );

    for ( i=0; i<len; i++ ) {
        crc = hexin_crc8_poly_calc( crc, pSrc[i], crc8_table_hacker );
    }

	return crc;
}
