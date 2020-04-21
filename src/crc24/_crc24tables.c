/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc24tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-17 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#include "_crc24tables.h"

static unsigned int     crc24_table_00065b[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc24_table_5d6dcb[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc24_table_864cfb[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc24_table_800063[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc24_table_328b63[MAX_TABLE_ARRAY] = { 0x00000000L };
static unsigned int     crc24_table_hacker[MAX_TABLE_ARRAY] = { 0x00000000L };

static unsigned int  	crc24_table_00065b_init		        = FALSE;
static unsigned int  	crc24_table_5d6dcb_init		        = FALSE;
static unsigned int  	crc24_table_864cfb_init		        = FALSE;
static unsigned int  	crc24_table_800063_init		        = FALSE;
static unsigned int  	crc24_table_328b63_init		        = FALSE;
static unsigned int     crc24_table_hacker_init             = FALSE;

unsigned int hexin_reverse24( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<24; i++ ) {
        t |= ( ( data >> i ) & 0x000001 ) << ( 23-i );
    }
    return t;
}

unsigned int hexin_crc24_init_table_poly_is_high( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = ( unsigned int ) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x00000001L ) crc = ( crc >> 1 ) ^ polynomial;
            else                           crc =   crc >> 1;
            c = c >> 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crc24_init_table_poly_is_low( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned int )i ) << 16;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x800000L )  crc = (( crc << 1 ) ^ polynomial);
            else                            crc =  ( crc << 1 );
            c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crc24_poly_is_high_calc( unsigned int crc24, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc24;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FFL & (unsigned int) c;
    tmp = crc ^ int_c;
    crc = (crc >> 8) ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned int hexin_crc24_poly_is_low_calc( unsigned int crc24, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc24;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FF & (unsigned int) c;
	tmp = (crc >> 16) ^ int_c;
    crc = (crc << 8)  ^ table[ tmp & 0xFF ];

    return crc;
}

static unsigned char hexin_init_crc24_table_hacker( unsigned int polynomial ) 
{
    if ( crc24_table_hacker_init == polynomial ) {
        return FALSE;
    }

    if ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        hexin_crc24_init_table_poly_is_high( polynomial, crc24_table_hacker );
    } else {
        hexin_crc24_init_table_poly_is_low(  polynomial, crc24_table_hacker );
    }
    crc24_table_hacker_init = polynomial;

    return TRUE;
}

unsigned int hexin_calc_crc24_hacker( const unsigned char *pSrc, unsigned int len, unsigned int crc24, unsigned int polynomial )
{
    unsigned int i = 0;
    unsigned int crc = crc24;

    hexin_init_crc24_table_hacker( polynomial );
    switch ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        case 0x800000L:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc24_poly_is_high_calc( crc, pSrc[i], crc24_table_hacker );
            }
            break;
        
        default:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc24_poly_is_low_calc(  crc, pSrc[i], crc24_table_hacker );
            }
            break;
    }

	return (crc & 0x00FFFFFF);
}

/*
*********************************************************************************************************
                                    POLY=0x00065BL [BLE]
Init   = 0x555555 ( 0xAAAAAA )
Refin  = True
Refout = True
Xorout = 0x000000
*********************************************************************************************************
*/

unsigned int hexin_calc_crc24_00065b( const unsigned char *pSrc, unsigned int len, unsigned int crc24 )
{
    unsigned int i = 0;
    unsigned int crc = crc24;

    if ( crc24_table_00065b_init == FALSE ) {
        crc24_table_00065b_init = hexin_crc24_init_table_poly_is_high( hexin_reverse24( CRC24_POLYNOMIAL_00065B ), crc24_table_00065b );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc24_poly_is_high_calc( crc, pSrc[i], crc24_table_00065b );
	}
	return (crc & 0xFFFFFF);
}


/*
*********************************************************************************************************
                                    POLY=0x5D6DCBL [FLEXRAYA\FLEXRAYB]
Init   = 0xFEDCBA(FLEXRAYA) 0xABCDEF(FLEXRAYB)
Refin  = True
Refout = True
Xorout = 0x000000
*********************************************************************************************************
*/

unsigned int hexin_calc_crc24_5d6dcb( const unsigned char *pSrc, unsigned int len, unsigned int crc24 )
{
    unsigned int i = 0;
    unsigned int crc = crc24;

    if ( crc24_table_5d6dcb_init == FALSE ) {
        crc24_table_5d6dcb_init = hexin_crc24_init_table_poly_is_low( CRC24_POLYNOMIAL_5D6DCB, crc24_table_5d6dcb );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc24_poly_is_low_calc( crc, pSrc[i], crc24_table_5d6dcb );
	}
	return (crc & 0xFFFFFF);
}

/*
*********************************************************************************************************
                                    POLY=0x864CFB [OPENPGP\LTE-A]
Init   = 0xB704CE(OPENPGP) 0x000000(LTE-A)
Refin  = False
Refout = False
Xorout = 0x000000
*********************************************************************************************************
*/

unsigned int hexin_calc_crc24_864cfb( const unsigned char *pSrc, unsigned int len, unsigned int crc24 )
{
    unsigned int i = 0;
    unsigned int crc = crc24;

    if ( crc24_table_864cfb_init == FALSE ) {
        crc24_table_864cfb_init = hexin_crc24_init_table_poly_is_low( CRC24_POLYNOMIAL_864CFB, crc24_table_864cfb );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc24_poly_is_low_calc( crc, pSrc[i], crc24_table_864cfb );
	}
	return (crc & 0xFFFFFF);
}

/*
*********************************************************************************************************
                                    POLY=0x864CFB [LTE-B\OS-9]
Init   = 0xFFFFFF(LTE-B) 0x000000(OS-9)
Refin  = False
Refout = False
Xorout = 0xFFFFFF (OS-9)
*********************************************************************************************************
*/

unsigned int hexin_calc_crc24_800063( const unsigned char *pSrc, unsigned int len, unsigned int crc24 )
{
    unsigned int i = 0;
    unsigned int crc = crc24;

    if ( crc24_table_800063_init == FALSE ) {
        crc24_table_800063_init = hexin_crc24_init_table_poly_is_low( CRC24_POLYNOMIAL_800063, crc24_table_800063 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc24_poly_is_low_calc( crc, pSrc[i], crc24_table_800063 );
	}
	return (crc & 0xFFFFFF);
}

unsigned int hexin_calc_crc24_328b63( const unsigned char *pSrc, unsigned int len, unsigned int crc24 )
{
    unsigned int i = 0;
    unsigned int crc = crc24;

    if ( crc24_table_328b63_init == FALSE ) {
        crc24_table_328b63_init = hexin_crc24_init_table_poly_is_low( CRC24_POLYNOMIAL_328B63, crc24_table_328b63 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc24_poly_is_low_calc( crc, pSrc[i], crc24_table_328b63 );
	}
	return (crc & 0xFFFFFF);
}