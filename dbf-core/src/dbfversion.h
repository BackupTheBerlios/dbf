/*********************************************************************
 *	dbfversion.h
 *********************************************************************
 *	Detects the dBASE version and if memo fields are used
 *
 *	Author: Björn Berg, clergyman@gmx.de
 *	Version: 0.1, 2003-01-26
 *
 *	History:
 *	2003-01-26	berg	first implementation
 ********************************************************************/

#ifndef _DBF_VERSION_ 
#define _DBF_VERSION_

int dbfexamine(unsigned char byte, int *version, int *memo);

#endif
