/*
*********************************************************************************************************
*                              		(c) Copyright 2006-2017, HZ, Studio
*                                           All Rights Reserved
* File    : _crcxtables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.0
* Web	  : http://heyunhuan513.blog.163.com
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-09-22 09:36AM
*
*********************************************************************************************************
*/

#include "_crcxtables.h"

static unsigned char    crc4_tab_shift_0c[MAX_TABLE_ARRAY]   = {0x0000};     // Used for ITU
static unsigned char    crc5_tab_shift_15[MAX_TABLE_ARRAY]   = {0x0000};     // Used for ITU
static unsigned char    crc5_tab_shift_48[MAX_TABLE_ARRAY]   = {0x0000};     // Used for EPC
static unsigned char    crc5_tab_shift_14[MAX_TABLE_ARRAY]   = {0x0000};     // Used for USB
static unsigned char    crc6_tab_shift_30[MAX_TABLE_ARRAY]   = {0x0000};     // Used for ITU
static unsigned char    crc7_tab_shift_12[MAX_TABLE_ARRAY]   = {0x0000};     // Used for MMC, SD

static int              crc4_tab_shift_0c_init               = FALSE;
static int              crc5_tab_shift_15_init               = FALSE;
static int              crc5_tab_shift_48_init               = FALSE;
static int              crc5_tab_shift_14_init               = FALSE;
static int              crc6_tab_shift_30_init               = FALSE;
static int              crc7_tab_shift_12_init               = FALSE;

/*
*********************************************************************************************************
                                    POLY=0x0C [ITU4]
*********************************************************************************************************
*/
static void _init_crc4_table_0c( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x01 ) crc = ( crc >> 1 ) ^ 0x0C;
			else                    crc =   crc >> 1;
			c = c >> 1;
        }
        crc4_tab_shift_0c[i] = crc;
    }
	crc4_tab_shift_0c_init = TRUE;
}

static unsigned char _hz_update_crc4_0c( unsigned char crc4, unsigned char c ) 
{
    unsigned char crc = crc4;

    if ( ! crc4_tab_shift_0c_init ) _init_crc4_table_0c();
    crc = crc4_tab_shift_0c[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc4_0c( const unsigned char *pSrc, unsigned int len, unsigned char crc4 )
{
    unsigned int i = 0;
    unsigned char crc = crc4;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc4_0c(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x15 [ITU5]
*********************************************************************************************************
*/
static void _init_crc5_table_15( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x01 ) crc = ( crc >> 1 ) ^ 0x15;
			else                    crc =   crc >> 1;
			c = c >> 1;
        }
        crc5_tab_shift_15[i] = crc;
    }
	crc5_tab_shift_15_init = TRUE;
}

static unsigned char _hz_update_crc5_15( unsigned char crc5, unsigned char c ) 
{
    unsigned char crc = crc5;

    if ( ! crc5_tab_shift_15_init ) _init_crc5_table_15();
    crc = crc5_tab_shift_15[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc5_15( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc5_15(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x48 [EPC]
*********************************************************************************************************
*/

static void _init_crc5_table_48( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ 0x48;
			else                    crc =   crc << 1;
			c = c << 1;
        }
        crc5_tab_shift_48[i] = crc;
    }
	crc5_tab_shift_48_init = TRUE;
}

static unsigned char _hz_update_crc5_48( unsigned char crc5, unsigned char c ) 
{
    unsigned char crc = crc5;

    if ( ! crc5_tab_shift_48_init ) _init_crc5_table_48();
    crc = crc5_tab_shift_48[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc5_48( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc5_48(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x14 [USB]
*********************************************************************************************************
*/
static void _init_crc5_table_14( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x01 ) crc = ( crc >> 1 ) ^ 0x14;
			else                    crc =   crc >> 1;
			c = c >> 1;
        }
        crc5_tab_shift_14[i] = crc;
    }
	crc5_tab_shift_14_init = TRUE;
}

static unsigned char _hz_update_crc5_14( unsigned char crc5, unsigned char c ) 
{
    unsigned char crc = crc5;

    if ( ! crc5_tab_shift_14_init ) _init_crc5_table_14();
    crc = crc5_tab_shift_14[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc5_14( const unsigned char *pSrc, unsigned int len, unsigned char crc5 )
{
    unsigned int i = 0;
    unsigned char crc = crc5;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc5_14(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x30 [ITU]
*********************************************************************************************************
*/
static void _init_crc6_table_30( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x01 ) crc = ( crc >> 1 ) ^ 0x30;
			else                    crc =   crc >> 1;
			c = c >> 1;
        }
        crc6_tab_shift_30[i] = crc;
    }
	crc6_tab_shift_30_init = TRUE;
}

static unsigned char _hz_update_crc6_30( unsigned char crc6, unsigned char c ) 
{
    unsigned char crc = crc6;

    if ( ! crc6_tab_shift_30_init ) _init_crc6_table_30();
    crc = crc6_tab_shift_30[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc6_30( const unsigned char *pSrc, unsigned int len, unsigned char crc6 )
{
    unsigned int i = 0;
    unsigned char crc = crc6;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc6_30(crc, pSrc[i]);
	}
	return crc;
}

/*
*********************************************************************************************************
                                    POLY=0x12 [MMC]
*********************************************************************************************************
*/

static void _init_crc7_table_12( void ) 
{
    unsigned int i = 0, j = 0;
    unsigned char crc, c;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ 0x12;
			else                    crc =   crc << 1;
			c = c << 1;
        }
        crc7_tab_shift_12[i] = crc;
    }
	crc7_tab_shift_12_init = TRUE;
}

static unsigned char _hz_update_crc7_12( unsigned char crc7, unsigned char c ) 
{
    unsigned char crc = crc7;

    if ( ! crc7_tab_shift_12_init ) _init_crc7_table_12();
    crc = crc7_tab_shift_12[ crc ^ c ];
    return crc;
}

unsigned char hz_calc_crc7_12( const unsigned char *pSrc, unsigned int len, unsigned char crc7 )
{
    unsigned int i = 0;
    unsigned char crc = crc7;

	for ( i=0; i<len; i++ ) {
		crc = _hz_update_crc7_12(crc, pSrc[i]);
	}
	return crc;
}
