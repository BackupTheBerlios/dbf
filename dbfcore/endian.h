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
 static char isbigendian=-1;
 if(isbigendian==-1) { 
     union ID {  
         unsigned char bytebuffer[4];
         unsigned long value;
     } id;
     //ID id;
     id.value=0x44332211L;
     if(id.bytebuffer[0]==0x11)
		isbigendian=_false;  //little endian
     if(id.bytebuffer[0]==0x44)
        isbigendian=_true; // bigendian
 }
 return((char)isbigendian);
}

int rotate2b(short var) {
	short tmp;	
	if(IsBigEndian() == _true) {
		tmp = var;
		tmp>>=8;
		var<<=8;
		var=var^tmp;
	}	
	return var;	
}

int rotate4b(int var) {
	int old, tmp;
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
		tmp&=0x00FF0000;
		old&=0x0000FF00;
		tmp|=old;	
		var|=tmp;
	}	
	return var;
}
