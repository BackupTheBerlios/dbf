/*********************************************************************
 *	dbfversion.c
 *********************************************************************
 *	Detects the dBASE version and if memo fields are used
 *
 *	Author: Björn Berg, clergyman@gmx.de
 *	Version: 0.1, 2003-01-26
 *
 *	History:
 *	2003-01-26	berg	first implementation
 ********************************************************************/

#include "dbfversion.h"

// dbfexamine gets the first byte which contains information
// about the used version, sql tables, and the use of memo files
// Bit 0-2	: version number
// Bit 3	: presence of dBASE IV memo file
// Bit 4-6	: presence of a SQL table
// Bit 7	: presence of any memo file (dBASE III or dBASE IV)
// the above description works for all dBASE versions since dBASE III


int dbfexamine(unsigned char byte, int *version, int *memo) {
	// extract version
	*version = ((int)byte & 7);		
	// detect if memofile is used
	*memo = ((int)byte & 128)==128 ? 1 : 0; 
		
	return 1;
}
