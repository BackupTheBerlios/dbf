/********************************************************************************
 * endian.h
 ********************************************************************************
 * Routines for Little Endian and Big Endian Systems
 * Library version
 *
 * Version 0.3, 2003-04-18
 * Author: Björn Berg, clergyman@gmx.de
 *
 * History:
 * 2003-04-18	berg	endian.h splitted to endian.h and endian.c
 *						
 * 2003-02-16	jones	the #ifndef inclusion guard only protected the #defines 
 *						where it really needs to protect the whole file. Just 
 *						moved the #endif to the end of the file. 
 * 2003-02-09	jones	improved IsBigEndian function
 *						changes in rotate4b
 * 2003-02-01	berg	rotate2b / rotate4b added	
 * 2002-12-12	berg	first implementation
 *******************************************************************************/	

#ifndef _ANUBISNET_ENDIAN_
#define _ANUBISNET_ENDIAN_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define _true 0x01
#define _false 0x00

typedef int _bool;

/* FUNCTIONS */
_bool IsBigEndian ();
short rotate2b ( short var);
unsigned int rotate4b ( unsigned int var );

#endif
