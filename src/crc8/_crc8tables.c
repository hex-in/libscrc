/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2022, Hexin
*                                           All Rights Reserved
* File    : _crc8tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.8
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize
*                       2020-03-20 [Heyn] New add hexin_calc_crc8_fletcher
*                       2020-08-04 [Heyn] Fixed Issues #4.
*                       2020-09-18 [Heyn] New add lin and lin2x checksum.
*                       2021-03-16 [Heyn] New add ID checksum.
*                       2021-06-07 [Heyn] Fixed Issues #8.
*                       2022-03-04 [Heyn] New add modbus(ASCII) checksum(LRC).
*
*   SEE : http://reveng.sourceforge.net/crc-catalogue/1-15.htm#crc.cat-bits.8
*
*********************************************************************************************************
*/

#include "_crc8tables.h"

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

unsigned char hexin_calc_crc8_fletcher( const unsigned char *pSrc, unsigned int len, unsigned char crc8 /*reserved*/ )
{
    unsigned int i = 0;
    unsigned char sum1 = 0, sum2 = 0;

    for ( i = 0; i < len; i++ ) {
        sum1 += pSrc[i];
        sum2 += sum1;
    }
    return ( sum1 & 0xF ) | ( sum2 << 4 );    
}

static unsigned int hexin_crc8_compute_init_table( struct _hexin_crc8 *param )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ param->poly;
            else                    crc =   crc << 1;
            c = c << 1;
        }
        param->table[i] = crc;
    }

    return TRUE;
}

static unsigned char hexin_crc8_compute_char( unsigned char crc8, unsigned char c, struct _hexin_crc8 *param )
{
    return param->table[ crc8 ^ c ];
}

unsigned char hexin_crc8_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc8 *param, unsigned char init )
{
    unsigned int i = 0;
    unsigned char crc = init;

    if ( param->is_initial == FALSE ) {
        param->is_initial = hexin_crc8_compute_init_table( param );
    }

    /* Fixed Issues #4 #8 */
    if ( HEXIN_REFOUT_IS_TRUE( param ) && ( HEXIN_GRADUAL_CALCULATE_IS_TRUE( param ) ) ) {
        crc = hexin_reverse8( init );
    }

    /* Fixed Issues #8  */
    if ( HEXIN_REFIN_IS_TRUE( param ) ) { 
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc8_compute_char( crc, hexin_reverse8(pSrc[i]), param );
        }
    } else {
        for ( i=0; i<len; i++ ) {
            crc = hexin_crc8_compute_char( crc, pSrc[i], param );
        }
    }

    /* Fixed Issues #8  */
    if ( HEXIN_REFOUT_IS_TRUE( param ) ) {
        crc = hexin_reverse8( crc );
    }

	return ( crc ^ param->xorout );
}

/*
 * See -> https://linchecksumcalculator.machsystems.cz/
 */
const unsigned char LIN2X_PID_TABLE[60] = { 0x80, 0xC1, 0x42, 0x03, 0xC4, 0x85, 0x06, 0x47, 0x08, 0x49,
                                            0xCA, 0x8B, 0x4C, 0x0D, 0x8E, 0xCF, 0x50, 0x11, 0x92, 0xD3,
                                            0x14, 0x55, 0xD6, 0x97, 0xD8, 0x99, 0x1A, 0x5B, 0x9C, 0xDD,
                                            0x5E, 0x1F, 0x20, 0x61, 0xE2, 0xA3, 0x64, 0x25, 0xA6, 0xE7,
                                            0xA8, 0xE9, 0x6A, 0x2B, 0xEC, 0xAD, 0x2E, 0x6F, 0xF0, 0xB1,
                                            0x32, 0x73, 0xB4, 0xF5, 0x76, 0x37, 0x78, 0x39, 0xBA, 0xFB };

unsigned char hexin_calc_crc8_lin( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int   i   = 0;
    unsigned short sum = ( (unsigned short)crc8 ) & 0x00FF;

	for ( i=1; i<len; i++ ) {
		sum += pSrc[i];
        sum = ( ( sum >> 8 ) & 0x00FF ) + ( sum & 0x00FF );
	}
    return ( 0xFF - sum );
}

unsigned char hexin_crc8_get_lin2x_pid( const unsigned char id )
{
    unsigned char p0  = 0, p1 = 0;

    p0 =  ( ( ( id & 0x01 ) >> 0 ) ^ ( ( id & 0x02 ) >> 1 ) ^ ( ( id & 0x04 ) >> 2 ) ^ ( ( id & 0x10 ) >> 4 ) );
    p1 = ~( ( ( id & 0x02 ) >> 1 ) ^ ( ( id & 0x08 ) >> 3 ) ^ ( ( id & 0x10 ) >> 4 ) ^ ( ( id & 0x20 ) >> 5 ) );
	
    return ( ( p1 & 0x01 ) << 7 ) | ( ( p0 & 0x01 ) << 6 ) | id;
}

unsigned char hexin_calc_crc8_lin2x( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned char id  = pSrc[0];

    /*
     * Frames with identifiers 0x3C and 0x3D can only use classic check,
     * these two groups of frames are LIN diagnostic frames.
     */
    if ( ( id == 0x3C ) || ( id == 0x3D ) ) {
        return hexin_calc_crc8_lin( pSrc, len, 0 );
    }
	return hexin_calc_crc8_lin( pSrc, len, hexin_crc8_get_lin2x_pid( id ) );
}

unsigned char hexin_calc_crc8_id8( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    const unsigned char coefficient[17] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2   };
    const unsigned char _last_array[11] = { '1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2' };
    
    unsigned int  i   = 0;
    unsigned int  sum = crc8;

    if ( ( len != 17 ) || ( crc8 != 0 ) ) {
        return 'N';
    }

	for ( i=0; i<len; i++ ) {
        if ( ( pSrc[i] >= 0 ) && ( pSrc[i] <= 9 ) ) {
            sum += pSrc[i] * coefficient[i];
        } 
        else if ( ( pSrc[i] >= 0x30 ) && ( pSrc[i] <= 0x39 ) ) {
            sum += ( pSrc[i] % 0x30 )*coefficient[i]; 
        }
        else {
            return 'N';
        }
	}

    return _last_array[ sum % 11 ];
}

unsigned char hexin_calc_crc8_nmea( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    const unsigned char *ptr = ( const unsigned char * )pSrc;
    unsigned int  crc = crc8;

    if ( *ptr == '$' ) {
        ptr++;
    }

    while ( *ptr && ( *ptr != '\r' ) && ( *ptr != '\n' ) && ( *ptr != '*' ) ) {
        crc ^= *ptr++;
    }

    return crc;
}

const unsigned char ascii2hex[MAX_TABLE_ARRAY] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char hexin_calc_modbus_ascii( const unsigned char *pSrc, unsigned int len, unsigned char crc8 )
{
    unsigned int i = 0;
    unsigned char crc = 0;

    if ( ( i % 2 ) != 0 ) {
        return 0;
    }

    for ( i=0; i<len; i+=2 ) {
        crc += ( ascii2hex[ *( pSrc + i ) ] << 4 ) | ( ascii2hex[ *( pSrc + i + 1 ) ] );
    }

    return (~crc) + 0x01;
}
