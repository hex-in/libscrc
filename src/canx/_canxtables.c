/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _canxtables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.1
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-04-21 [Heyn] Initialize.
*
*********************************************************************************************************
*/

#include "_canxtables.h"

static unsigned int   canx_table_shared[MAX_TABLE_ARRAY]  = { 0x00000000L };
static unsigned int   canx_table_shared_init              = FALSE;

unsigned int hexin_reverse15( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<15; i++ ) {
        t |= ( ( data >> i ) & 0x0001 ) << ( 14-i );
    }
    return t;
}

unsigned int hexin_reverse17( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<17; i++ ) {
        t |= ( ( data >> i ) & 0x0001 ) << ( 16-i );
    }
    return t;
}

unsigned int hexin_reverse21( unsigned int data )
{
    unsigned int i = 0;
    unsigned int t = 0;
    for ( i=0; i<21; i++ ) {
        t |= ( ( data >> i ) & 0x0001 ) << ( 20-i );
    }
    return t;
}

unsigned int hexin_can15_init_table_poly_is_low( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned int )i ) << 7;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x4000L )  crc = ( crc << 1 ) ^ polynomial;
            else                          crc =   crc << 1;
            c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_can17_init_table_poly_is_low( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned int )i ) << 9;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x10000L )  crc = ( crc << 1 ) ^ polynomial;
            else                           crc =   crc << 1;
            c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_can21_init_table_poly_is_low( unsigned int polynomial, unsigned int *table )
{
    unsigned int i = 0, j = 0;
    unsigned int crc = 0x00000000L;
    unsigned int c   = 0x00000000L;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c	= ( ( unsigned int )i ) << 13;
        for ( j=0; j<8; j++ ) {
            if ( ( crc ^ c ) & 0x100000L )  crc = ( crc << 1 ) ^ polynomial;
            else                            crc =   crc << 1;
            c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_can15_poly_is_low_calc( unsigned int crc32, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FF & (unsigned int) c;
	tmp = (crc >> 7) ^ int_c;
    crc = (crc << 8) ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned int hexin_can17_poly_is_low_calc( unsigned int crc32, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FF & (unsigned int) c;
	tmp = (crc >> 9) ^ int_c;
    crc = (crc << 8) ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned int hexin_can21_poly_is_low_calc( unsigned int crc32, unsigned char c, const unsigned int *table )
{
    unsigned int crc = crc32;
    unsigned int tmp = 0x00000000L;
    unsigned int int_c = 0x00000000L;

    int_c = 0x000000FF & (unsigned int) c;
	tmp = (crc >> 13) ^ int_c;
    crc = (crc <<  8) ^ table[ tmp & 0xFF ];

    return crc;
}

unsigned int hexin_calc_can15( const unsigned char *pSrc, unsigned int len, unsigned int crc32 )
{
    unsigned int i = 0;
    unsigned int crc = crc32;

    if ( canx_table_shared_init != CAN15_POLYNOMIAL_00004599 ) {
        hexin_can15_init_table_poly_is_low( CAN15_POLYNOMIAL_00004599, canx_table_shared );
        canx_table_shared_init = CAN15_POLYNOMIAL_00004599;
    }

    for ( i=0; i<len; i++ ) {
        crc = hexin_can15_poly_is_low_calc(  crc, pSrc[i], canx_table_shared );
    }

	return crc;
}

unsigned int hexin_calc_can17( const unsigned char *pSrc, unsigned int len, unsigned int crc32 )
{
    unsigned int i = 0;
    unsigned int crc = crc32;

    if ( canx_table_shared_init != CAN17_POLYNOMIAL_0001685B ) {
        hexin_can17_init_table_poly_is_low( CAN17_POLYNOMIAL_0001685B, canx_table_shared );
        canx_table_shared_init = CAN17_POLYNOMIAL_0001685B;
    }

    for ( i=0; i<len; i++ ) {
        crc = hexin_can17_poly_is_low_calc(  crc, pSrc[i], canx_table_shared );
    }

	return crc;
}

unsigned int hexin_calc_can21( const unsigned char *pSrc, unsigned int len, unsigned int crc32 )
{
    unsigned int i = 0;
    unsigned int crc = crc32;

    if ( canx_table_shared_init != CAN21_POLYNOMIAL_00102899 ) {
        hexin_can21_init_table_poly_is_low( CAN21_POLYNOMIAL_00102899, canx_table_shared );
        canx_table_shared_init = CAN21_POLYNOMIAL_00102899;
    }

    for ( i=0; i<len; i++ ) {
        crc = hexin_can21_poly_is_low_calc(  crc, pSrc[i], canx_table_shared );
    }

	return crc;
}