/**************************************************
 * endian.h
 **************************************************
 * Routines for Little Endian and Big Endian
 * Systems
 *
 * Version 0.1, 2002-12-12
 * Author: Björn Berg, clergyman@gmx.de
 *
 * History:
 * 2003-02-01	berg	rotate2b / rotate4b added	
 * 2002-12-12	berg	first implementation
 **************************************************/	

#ifndef _ANUBISNET_ENDIAN_
#define _ANUBISNET_ENDIAN_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define _true 0x01
#define _false 0x00

int IsBigEndian();
int rotateshort(short);
#endif

// Routine to test if System uses Big Endian
// or Little Endian
int IsBigEndian() {
 int i = 1;
 int c = *((char*)&i);
 return c == 0;
}

int rotate2b(short var) {
	if(IsBigEndian() == _true) {
        short tmp = var;
        var = (tmp >> 8) | ( var << 8);	    
	}	
	return var;	
}

unsigned int rotate4b( unsigned int var) {
	unsigned int old, tmp;
	if(IsBigEndian() == _true) {	
		tmp = old = var;
		// change Byte 4 with Byte 1
		tmp>>=24;
		var<<=24;
		var^=tmp;
		// change Byte 3 with Byte 2
		tmp=old;	
		tmp<<=8;
		old>>=8;
		tmp &= 0x00FF0000;
		old &= 0x0000FF00;
		tmp |= old;	
		var |= tmp;
	}	
	return var;
}
