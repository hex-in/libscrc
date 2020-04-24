/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crcxtables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*                       2020-03-17 [Heyn] Optimized Code.
*
*********************************************************************************************************
*/

#include "_crcxtables.h"

static unsigned char    crc4_table_0c[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for ITU
static unsigned char    crc5_table_15[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for ITU
static unsigned char    crc5_table_09[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for EPC
static unsigned char    crc5_table_14[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for USB
static unsigned char    crc6_table_30[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for ITU
static unsigned char    crc6_table_2f[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for GSM
static unsigned char    crc6_table_19[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for DARC
static unsigned char    crc7_table_45[MAX_TABLE_ARRAY]   = { 0x00 };     //
static unsigned char    crc7_table_4f[MAX_TABLE_ARRAY]   = { 0x00 };     //
static unsigned char    crc7_table_09[MAX_TABLE_ARRAY]   = { 0x00 };     // Used for MMC, SD

static int              crc4_table_0c_init               = FALSE;
static int              crc5_table_15_init               = FALSE;
static int              crc5_table_09_init               = FALSE;
static int              crc5_table_14_init               = FALSE;
static int              crc6_table_30_init               = FALSE;
static int              crc6_table_2f_init               = FALSE;
static int              crc6_table_19_init               = FALSE;
static int              crc7_table_45_init               = FALSE;
static int              crc7_table_4f_init               = FALSE;
static int              crc7_table_09_init               = FALSE;

unsigned char hexin_reverse7( unsigned char data )
{
    unsigned int  i = 0;
    unsigned char t = 0;
    for ( i=0; i<7; i++ ) {
        t |= ( ( data >> i ) & 0x01 ) << ( 6-i );
    }
    return t;
}

unsigned char hexin_reverse8( unsigned char data )
{
    unsigned int  i = 0;
    unsigned char t = 0;
    for ( i=0; i<8; i++ ) {
        t |= ( ( data >> i ) & 0x01 ) << ( 7-i );
    }
    return t;
}

/*
width=3 poly=0x3 init=0x0
refin=False refout=False
xorout=0x7 check=0x4 residue=0x2 name="CRC-3/GSM"
*/
unsigned char hexin_calc_crc3_gsm(  const unsigned char *pSrc, unsigned int len, unsigned char crc3 )
{
    unsigned int i = 0, j = 0;
    unsigned char crc  = crc3;
    unsigned char poly = ( 0x03 << ( 8-3 ) );

	for ( i=0; i<len; i++ ) {
        crc = ( crc ^ pSrc[i] );
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x80 )   crc = ( crc << 1 ) ^ poly;
            else                crc = ( crc << 1 );
        }
	}

    return (crc >> ( 8 - 3));
}

/*
width=3 poly=0x3 init=0x7
refin=True refout=True
xorout=0x0 check=0x6 residue=0x0 name="CRC-3/ROHC
*/
unsigned char hexin_calc_crc3_rohc( const unsigned char *pSrc, unsigned int len, unsigned char crc3 )
{
    unsigned int i = 0, j = 0;
    unsigned char crc  = crc3;
    unsigned char poly = ( hexin_reverse8( 0x03 ) >> ( 8 - 3 ) );

	for ( i=0; i<len; i++ ) {
        crc = ( crc ^ pSrc[i] );
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x01 )   crc = ( crc >> 1 ) ^ poly;
            else                crc = ( crc >> 1 );
        }
	}

    return crc;
}

unsigned int hexin_crcx_init_table_poly_is_high( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = i;
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x01 )   crc = ( crc >> 1 ) ^ ( polynomial );
            else                crc =   crc >> 1;
            c = c >> 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crcx_init_table_poly_is_low( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = i;
        for ( j=0; j<8; j++ ) {
			if ( crc & 0x80 ) crc = ( crc << 1 ) ^ ( polynomial );
			else              crc =   crc << 1;
        }
        table[i] = ( crc );
    }
    return TRUE;
}

unsigned char hexin_crcx_poly_calc( unsigned char crcx, unsigned char c, const unsigned char *table )
{
    return table[ crcx ^ c ];
}


unsigned int hexin_crcx_init_table( enum hexin_crcx_mask mask, unsigned char polynomial, unsigned char *table )
{
    unsigned int result = FALSE;
    if ( mask & polynomial ) {
        result = hexin_crcx_init_table_poly_is_high( polynomial, table );
    } else {
        result = hexin_crcx_init_table_poly_is_low ( polynomial, table );
    }
    return result;
}

/*
*********************************************************************************************************
                                    POLY=0x0C [ITU4]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc4_0c( const unsigned char *pSrc, unsigned int len, unsigned char crc4 )
{
    unsigned int i = 0;
    unsigned char crc = crc4;

    if ( crc4_table_0c_init == FALSE ) {
        crc4_table_0c_init = hexin_crcx_init_table( HEXIN_CRC4_MASK, 0x0C, crc4_table_0c );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc4_table_0c );
	}
	return crc;
}

/*
width=4 poly=0x3 init=0xF
refin=False refout=False
xorout=0xf check=0xb residue=0x2 name="CRC-4/INTERLAKEN"
*/
unsigned char hexin_calc_crc4_03( const unsigned char *pSrc, unsigned int len, unsigned char crc4 )
{
    unsigned int i = 0, j = 0;
    unsigned char crc  = ( crc4 << ( 8-4 ) );
    unsigned char poly = ( 0x03 << ( 8-4 ) );

	for ( i=0; i<len; i++ ) {
        crc = ( crc ^ pSrc[i] );
        for ( j=0; j<8; j++ ) {
            if ( crc & 0x80 )   crc = ( crc << 1 ) ^ poly;
            else                crc = ( crc << 1 );
        }
	}

    return (crc >> ( 8 - 4));
}

/*
*********************************************************************************************************
                                    POLY=0x15 [ITU5]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc5_15( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

    if ( crc5_table_15_init == FALSE ) {
        crc5_table_15_init = hexin_crcx_init_table( HEXIN_CRC5_MASK, 0x15, crc5_table_15 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc5_table_15 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x48 [EPC]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc5_09( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

    if ( crc5_table_09_init == FALSE ) {
        crc5_table_09_init = hexin_crcx_init_table( HEXIN_CRC5_MASK, 0x48, crc5_table_09 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc5_table_09 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x14 [USB]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc5_14( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

    if ( crc5_table_14_init == FALSE ) {
        crc5_table_14_init = hexin_crcx_init_table( HEXIN_CRC5_MASK, 0x14, crc5_table_14 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc5_table_14 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x30 [ITU]
*********************************************************************************************************
*/

unsigned char hexin_calc_crc6_30( const unsigned char *pSrc, unsigned int len, unsigned char crc6 )
{
    unsigned int i = 0;
    unsigned char crc = crc6;

    if ( crc6_table_30_init == FALSE ) {
        crc6_table_30_init = hexin_crcx_init_table( HEXIN_CRC6_MASK, 0x30, crc6_table_30 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc6_table_30 );
	}
	return crc;
}

unsigned char hexin_calc_crc6_2f( const unsigned char *pSrc, unsigned int len, unsigned char crc6 )
{
    unsigned int i = 0;
    unsigned char crc = crc6;

    if ( crc6_table_2f_init == FALSE ) {
        crc6_table_2f_init = hexin_crcx_init_table_poly_is_low ( 0xBC, crc6_table_2f );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc6_table_2f );
	}

	return ( crc >> 2 );
}

unsigned char hexin_calc_crc6_19( const unsigned char *pSrc, unsigned int len, unsigned char crc6 )
{
    unsigned int i = 0;
    unsigned char crc = crc6;

    if ( crc6_table_19_init == FALSE ) {
        crc6_table_19_init = hexin_crcx_init_table( HEXIN_CRC6_MASK, hexin_reverse8( 0x19 ) >> (8-6), crc6_table_19 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crcx_poly_calc( crc, pSrc[i], crc6_table_19 );
	}

	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x09 [MMC]
*********************************************************************************************************
*/

unsigned int hexin_crc7_init_table_poly_is_low( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = i;
        for ( j=0; j<8; j++ ) {
			if ( crc & 0x80 ) crc = ( crc << 1 ) ^ ( polynomial << 1 );
			else              crc =   crc << 1;
        }
        table[i] = ( crc >> 1 );
    }
    return TRUE;
}

unsigned char hexin_crc7_poly_is_high_calc( unsigned char crcx, unsigned char c, const unsigned char *table )
{
    return table[ crcx ^ c ];
}

unsigned char hexin_crc7_poly_is_low_calc( unsigned char crcx, unsigned char c, const unsigned char *table )
{
    return table[ (crcx << 1) ^ c ];
}

unsigned char hexin_calc_crc7_09( const unsigned char *pSrc, unsigned int len, unsigned char crc7 )
{
    unsigned int i = 0;
    unsigned char crc = crc7;

    if ( crc7_table_09_init == FALSE ) {
        crc7_table_09_init = hexin_crc7_init_table_poly_is_low ( 0x09, crc7_table_09 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc7_poly_is_low_calc( crc, pSrc[i], crc7_table_09 );
	}
	return crc;
}

unsigned char hexin_calc_crc7_45( const unsigned char *pSrc, unsigned int len, unsigned char crc7 )
{
    unsigned int i = 0;
    unsigned char crc = crc7;

    if ( crc7_table_45_init == FALSE ) {
        crc7_table_45_init = hexin_crc7_init_table_poly_is_low ( 0x45, crc7_table_45 );
    }

	for ( i=0; i<len; i++ ) {
        crc = hexin_crc7_poly_is_low_calc( crc, pSrc[i], crc7_table_45 );
	}
	return crc;
}

unsigned char hexin_calc_crc7_4f( const unsigned char *pSrc, unsigned int len, unsigned char crc7 )
{
    unsigned int i = 0;
    unsigned char crc = crc7;

    if ( crc7_table_4f_init == FALSE ) {
        crc7_table_4f_init = hexin_crcx_init_table_poly_is_high ( hexin_reverse7( 0x4F ), crc7_table_4f );
    }

	for ( i=0; i<len; i++ ) {
        crc = hexin_crc7_poly_is_high_calc( crc, pSrc[i], crc7_table_4f );
	}
	return crc;
}
