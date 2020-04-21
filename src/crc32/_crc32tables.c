/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc32tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize
*                       2020-03-20 [Heyn] New add hexin_calc_crc32_adler
*                       2020-03-23 [Heyn] New add hexin_calc_crc32_fletcher
*
*********************************************************************************************************
*/

#include "_crc32tables.h"

static unsigned int     crc32_table_04c11db7[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc32_table_edb88320[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc32_table_hacker[  MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc32_table_shared[  MAX_TABLE_ARRAY] = { 0x00000000L };

static unsigned int  	crc32_table_04c11db7_init		      = FALSE;
static unsigned int		crc32_table_edb88320_init	          = FALSE;
static unsigned int     crc32_table_hacker_init               = FALSE;
static unsigned int     crc32_table_shared_init               = FALSE;

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
            else                            crc =   crc << 1;
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

/*
*********************************************************************************************************
                                    POLY=0x04C11DB7L [FSC]
*********************************************************************************************************
*/
unsigned int hexin_calc_crc32_04c11db7( const unsigned char *pSrc, unsigned int len, unsigned int crc32 )
{
    unsigned int i = 0;
    unsigned int crc = crc32;

    if ( crc32_table_04c11db7_init == FALSE ) {
        crc32_table_04c11db7_init = hexin_crc32_init_table_poly_is_low( CRC32_POLYNOMIAL_04C11DB7, crc32_table_04c11db7 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc32_poly_is_low_calc( crc, pSrc[i], crc32_table_04c11db7 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xEDB88320L [CRC32 for file]
*********************************************************************************************************
*/
unsigned int hexin_calc_crc32_edb88320( const unsigned char *pSrc, unsigned int len, unsigned int crc32 )
{
    unsigned int i = 0;
    unsigned int crc = crc32;
    if ( crc32_table_edb88320_init == FALSE ) {
        crc32_table_edb88320_init = hexin_crc32_init_table_poly_is_high( CRC32_POLYNOMIAL_EDB88320, crc32_table_edb88320 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc32_poly_is_high_calc( crc, pSrc[i], crc32_table_edb88320 );
	}

	return crc;    
}

/*
*********************************************************************************************************
*                                   For hacker
*********************************************************************************************************
*/
static unsigned char hexin_init_crc32_table_hacker( unsigned int polynomial ) 
{
    if ( crc32_table_hacker_init == polynomial ) {
        return FALSE;
    }

    if ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        hexin_crc32_init_table_poly_is_high( polynomial, crc32_table_hacker );
    } else {
        hexin_crc32_init_table_poly_is_low(  polynomial, crc32_table_hacker );
    }
    crc32_table_hacker_init = polynomial;

    return TRUE;
}

unsigned int hexin_calc_crc32_hacker( const unsigned char *pSrc, unsigned int len, unsigned int crc32, unsigned int polynomial )
{
    unsigned int i = 0;
    unsigned int crc = crc32;

    hexin_init_crc32_table_hacker( polynomial );

    switch ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        case 0x80000000L:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc32_poly_is_high_calc( crc, pSrc[i], crc32_table_hacker );
            }
            break;
        
        default:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc32_poly_is_low_calc(  crc, pSrc[i], crc32_table_hacker );
            }
            break;
    }

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

/*
*********************************************************************************************************
                                    For fletcher32 checksum
*********************************************************************************************************
*/
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


/*
*********************************************************************************************************
                                    For User
*********************************************************************************************************
*/
unsigned int hexin_calc_crc32_shared( const unsigned char *pSrc,
                                      unsigned int len,
                                      unsigned int crc32,
                                      unsigned int polynomial,
                                      unsigned char mask /* TRUE -> high, FALSE -> low */ )
{
    unsigned int i = 0;
    unsigned int crc = crc32;

    if ( crc32_table_shared_init != polynomial ) {
        if ( TRUE == mask ) {
            hexin_crc32_init_table_poly_is_high( polynomial, crc32_table_shared );
        } else {
            hexin_crc32_init_table_poly_is_low(  polynomial, crc32_table_shared );
        }
        crc32_table_shared_init = polynomial;
    }

    switch ( mask ) {
        case TRUE:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc32_poly_is_high_calc( crc, pSrc[i], crc32_table_shared );
            }
            break;
        
        default:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc32_poly_is_low_calc(  crc, pSrc[i], crc32_table_shared );
            }
            break;
    }

	return crc;
}
