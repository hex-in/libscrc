/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crc16tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.0
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-20 21:57PM [Heyn]
*                       WebTool :
*                       http://www.scadacore.com/field-tools/programming-calculators/online-checksum-calculator/
*                       https://www.lammertbies.nl/comm/info/crc-calculation.html
*                       http://www.ip33.com/crc.html
*
*********************************************************************************************************
*/

#include "_crc16tables.h"

static unsigned short   crc16_tab_shift_8408[MAX_TABLE_ARRAY]   = {0x0000};     // Used for X25 Kermit
static unsigned short   crc16_tab_shift_1021[MAX_TABLE_ARRAY]   = {0x0000};     // Used for CCITT-FALSE XModem
static unsigned short   crc16_tab_shift_a001[MAX_TABLE_ARRAY]   = {0x0000};     // Used for Modbus USB
static unsigned short   crc16_tab_shift_8005[MAX_TABLE_ARRAY]   = {0x0000};     // Used for
static unsigned short   crc16_tab_shift_a6bc[MAX_TABLE_ARRAY]   = {0x0000};     // Used for
static unsigned short   crc16_tab_shift_91a0[MAX_TABLE_ARRAY]   = {0x0000};     // Used for Cordless Telephones

static int              crc16_tab_shift_8408_init               = FALSE;
static int              crc16_tab_shift_1021_init               = FALSE;
static int              crc16_tab_shift_a001_init               = FALSE;
static int              crc16_tab_shift_8005_init               = FALSE;
static int              crc16_tab_shift_a6bc_init               = FALSE;
static int              crc16_tab_shift_91a0_init               = FALSE;


/*
*********************************************************************************************************
                                    POLY=0x91A0 [Cordless Telephones]
*********************************************************************************************************
*/
static void _init_crc16_table_91a0( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned short crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = (unsigned short) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC16_POLYNOMIAL_91A0;
            else                      crc =   crc >> 1;
            c = c >> 1;
        }
        crc16_tab_shift_91a0[i] = crc;
    }
    crc16_tab_shift_91a0_init = TRUE;
}

static unsigned short _hz_update_crc16_91a0( unsigned short crc16, unsigned char c ) 
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c = 0x00FF & (unsigned short) c;
    if ( ! crc16_tab_shift_91a0_init ) _init_crc16_table_91a0();

    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ crc16_tab_shift_91a0[ tmp & 0xFF ];

    return crc;
}

unsigned short hz_calc_crc16_91a0( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

	for ( i=0; i<len; i++ ) {
		crc	= _hz_update_crc16_91a0(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x8408 [X25 Kermit]
*********************************************************************************************************
*/

static void _init_crc16_table_8408( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned short crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = (unsigned short) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC16_POLYNOMIAL_8408;
            else                      crc =   crc >> 1;
            c = c >> 1;
        }
        crc16_tab_shift_8408[i] = crc;
    }
    crc16_tab_shift_8408_init = TRUE;
}

static unsigned short _hz_update_crc16_8408( unsigned short crc16, unsigned char c ) 
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c = 0x00FF & (unsigned short) c;
    if ( ! crc16_tab_shift_8408_init ) _init_crc16_table_8408();

    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ crc16_tab_shift_8408[ tmp & 0xFF ];

    return crc;
}

unsigned short hz_calc_crc16_8408( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

	for ( i=0; i<len; i++ ) {
		crc	= _hz_update_crc16_8408(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x1021 [CCITT-FALSE XModem]
*********************************************************************************************************
*/

static void _init_crc16_table_1021( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned short crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = ((unsigned short) i) << 8;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x8000 ) crc = ( crc << 1 ) ^ CRC16_POLYNOMIAL_1021;
            else                      crc =   crc << 1;
            c = c << 1;
        }
        crc16_tab_shift_1021[i] = crc;
    }
    crc16_tab_shift_1021_init = TRUE;
}

static unsigned short _hz_update_crc16_1021( unsigned short crc16, unsigned char c )
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c  = 0x00FF & (unsigned short) c;

    if ( ! crc16_tab_shift_1021_init ) _init_crc16_table_1021();

    tmp = (crc >> 8) ^ short_c;
    crc = (crc << 8) ^ crc16_tab_shift_1021[tmp];

    return crc;
}

unsigned short hz_calc_crc16_1021( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc16_1021(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xA001 [Modbus USB]
*********************************************************************************************************
*/

static void _init_crc16_table_a001( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned short crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = (unsigned short) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x0001 )   crc = ( crc >> 1 ) ^ CRC16_POLYNOMIAL_A001;
            else                        crc = crc >> 1;
            c = c >> 1;
        }
        crc16_tab_shift_a001[i] = crc;
    }
    crc16_tab_shift_a001_init = TRUE;
}

static unsigned short _hz_update_crc16_a001( unsigned short crc16, unsigned char c ) 
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c = 0x00FF & (unsigned short) c;
    if ( ! crc16_tab_shift_a001_init ) _init_crc16_table_a001();

    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ crc16_tab_shift_a001[ tmp & 0xFF ];

    return crc;
}

unsigned short hz_calc_crc16_a001( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc16_a001(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x8005 [IBM]
*********************************************************************************************************
*/

static void _init_crc16_table_8005( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned short crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = (unsigned short) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x0001 )   crc = ( crc >> 1 ) ^ CRC16_POLYNOMIAL_8005;
            else                        crc = crc >> 1;
            c = c >> 1;
        }
        crc16_tab_shift_8005[i] = crc;
    }
    crc16_tab_shift_8005_init = TRUE;
}

static unsigned short _hz_update_crc16_8005( unsigned short crc16, unsigned char c ) 
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c = 0x00FF & (unsigned short) c;

    if ( ! crc16_tab_shift_8005_init ) _init_crc16_table_8005();

    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ crc16_tab_shift_8005[ tmp & 0xFF ];

    return crc;
}

unsigned short hz_calc_crc16_8005( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
    unsigned short crc = crc16;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc16_8005(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0xA6BC [DNP]
*********************************************************************************************************
*/

static void _init_crc16_table_a6bc( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned short crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = (unsigned short) i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC16_POLYNOMIAL_A6BC;
            else                      crc =   crc >> 1;
            c = c >> 1;
        }
        crc16_tab_shift_a6bc[i] = crc;
    }
    crc16_tab_shift_a6bc_init = TRUE;
}

static unsigned short _hz_update_crc16_a6bc( unsigned short crc16, unsigned char c ) 
{
    unsigned short crc = crc16;
    unsigned short tmp, short_c;

    short_c = 0x00FF & (unsigned short) c;

    if ( ! crc16_tab_shift_a6bc_init ) _init_crc16_table_a6bc();

    tmp =  crc       ^ short_c;
    crc = (crc >> 8) ^ crc16_tab_shift_a6bc[ tmp & 0xFF ];

    return crc;
}

unsigned short hz_calc_crc16_a6bc( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int i = 0;
	unsigned short crc = crc16;
    
    for ( i=0; i<len; i++ ) {
        crc	= _hz_update_crc16_a6bc(crc, pSrc[i]);
    }
    return ~crc;
}

/*
*********************************************************************************************************
                                    POLY=0x8005 [SICK]
*********************************************************************************************************
*/

static unsigned short _hz_update_crc16_sick( unsigned short crc16, unsigned char c, char prev_byte ) 
{
    unsigned short crc = crc16;
    unsigned short short_c, short_p;

    short_c  =   0x00FF & (unsigned short) c;
    short_p  = ( 0x00FF & (unsigned short) prev_byte ) << 8;

    if ( crc & 0x8000 ) crc = ( crc << 1 ) ^ CRC16_POLYNOMIAL_8005;
    else                crc =   crc << 1;

    crc &= 0xFFFF;
    crc ^= ( short_c | short_p );

    return crc;
}

unsigned short hz_calc_crc16_sick( const unsigned char *pSrc, unsigned int len, unsigned short crc16 )
{
    unsigned int   i            = 0;
			 char  prev_byte	= 0x00;
	unsigned short crc		    = crc16;

	for ( i=0; i<len; i++ ) {
		crc	        = _hz_update_crc16_sick(crc, pSrc[i], prev_byte);
		prev_byte	= pSrc[i];
    }

	return crc;
}
