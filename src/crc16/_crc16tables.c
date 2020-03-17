/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc16tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-20 21:57PM [Heyn]
*                       WebTool :
*                       http://www.scadacore.com/field-tools/programming-calculators/online-checksum-calculator/
*                       https://www.lammertbies.nl/comm/info/crc-calculation.html
*                       http://www.ip33.com/crc.html
*                       
*                       2020-03-13 [Heyn] New add hacker code.
*
*********************************************************************************************************
*/

#include "_crc16tables.h"

static unsigned short   crc16_table_8408[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for X25 Kermit
static unsigned short   crc16_table_1021[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for CCITT-FALSE XModem
static unsigned short   crc16_table_a001[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for Modbus USB
static unsigned short   crc16_table_8005[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for
static unsigned short   crc16_table_a6bc[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for
static unsigned short   crc16_table_91a0[MAX_TABLE_ARRAY]   = { 0x0000 };     // Used for Cordless Telephones

static unsigned int     crc16_table_8408_init               = FALSE;
static unsigned int     crc16_table_1021_init               = FALSE;
static unsigned int     crc16_table_a001_init               = FALSE;
static unsigned int     crc16_table_8005_init               = FALSE;
static unsigned int     crc16_table_a6bc_init               = FALSE;
static unsigned int     crc16_table_91a0_init               = FALSE;


static unsigned short   crc16_table_hacker[MAX_TABLE_ARRAY] = { 0x0000 };     // Used for hacker.
static unsigned short   crc16_table_hacker_init             = FALSE;          // Default value.


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

/*
*********************************************************************************************************
                                    POLY=0x91A0 [Cordless Telephones]
*********************************************************************************************************
*/

unsigned short hexin_calc_crc16_91a0( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    if ( crc16_table_91a0_init == FALSE ) {
        crc16_table_91a0_init = hexin_crc16_init_table_poly_is_high( CRC16_POLYNOMIAL_91A0, crc16_table_91a0 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc16_poly_is_high_calc( crc, pSrc[i], crc16_table_91a0 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x8408 [X25 Kermit]
*********************************************************************************************************
*/

unsigned short hexin_calc_crc16_8408( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    if ( crc16_table_8408_init == FALSE ) {
        crc16_table_8408_init = hexin_crc16_init_table_poly_is_high( CRC16_POLYNOMIAL_8408, crc16_table_8408 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc16_poly_is_high_calc( crc, pSrc[i], crc16_table_8408 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x1021 [CCITT-FALSE XModem]
*********************************************************************************************************
*/

unsigned short hexin_calc_crc16_1021( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    if ( crc16_table_1021_init == FALSE ) {
        crc16_table_1021_init = hexin_crc16_init_table_poly_is_low( CRC16_POLYNOMIAL_1021, crc16_table_1021 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc16_poly_is_low_calc( crc, pSrc[i], crc16_table_1021 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xA001 [Modbus USB]
*********************************************************************************************************
*/

unsigned short hexin_calc_crc16_a001( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    if ( crc16_table_a001_init == FALSE ) {
        crc16_table_a001_init = hexin_crc16_init_table_poly_is_high( CRC16_POLYNOMIAL_A001, crc16_table_a001 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc16_poly_is_high_calc( crc, pSrc[i], crc16_table_a001 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x8005 [IBM]
*********************************************************************************************************
*/

unsigned short hexin_calc_crc16_8005( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    if ( crc16_table_8005_init == FALSE ) {
        crc16_table_8005_init = hexin_crc16_init_table_poly_is_high( CRC16_POLYNOMIAL_8005, crc16_table_8005 );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc16_poly_is_high_calc( crc, pSrc[i], crc16_table_8005 );
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xA6BC [DNP]
*********************************************************************************************************
*/

unsigned short hexin_calc_crc16_a6bc( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    if ( crc16_table_a6bc_init == FALSE ) {
        crc16_table_a6bc_init = hexin_crc16_init_table_poly_is_high( CRC16_POLYNOMIAL_A6BC, crc16_table_a6bc );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc16_poly_is_high_calc( crc, pSrc[i], crc16_table_a6bc );
	}
	return ~crc;
}

/*
*********************************************************************************************************
                                    POLY=0x8005 [SICK]
*********************************************************************************************************
*/

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

/*
*********************************************************************************************************
                                    For hacker
*********************************************************************************************************
*/

static unsigned char hexin_init_crc16_table_hacker( unsigned short polynomial  ) 
{
    if ( crc16_table_hacker_init == polynomial ) {
        return FALSE;
    }

    if ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        hexin_crc16_init_table_poly_is_high( polynomial, crc16_table_hacker );
    } else {
        hexin_crc16_init_table_poly_is_low(  polynomial, crc16_table_hacker );
    }
    crc16_table_hacker_init = polynomial;

    return TRUE;
}

unsigned short hexin_calc_crc16_hacker( const unsigned char *pSrc, unsigned int len, unsigned short crc16, unsigned short polynomial )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

    hexin_init_crc16_table_hacker( polynomial );

    switch ( HEXIN_POLYNOMIAL_IS_HIGH( polynomial ) ) {
        case 0x8000:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc16_poly_is_high_calc( crc, pSrc[i], crc16_table_hacker );
            }
            break;
        
        default:
            for ( i=0; i<len; i++ ) {
                crc = hexin_crc16_poly_is_low_calc(  crc, pSrc[i], crc16_table_hacker );
            }
            break;
    }

	return crc;
}
