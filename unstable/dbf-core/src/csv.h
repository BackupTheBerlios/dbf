/***********************************************************************************
 * csv.h				    														
 * Author: Bjoern Berg, March 2003	    											
 * Email: clergyman@gmx.de		    												
 * dbf Reader and Converter for dBASE files    											
 * Version 0.1																	
 ***********************************************************************************
 * This includes enable dbf to write CSV files
 ***********************************************************************************				
 * History:
 * 2003-03-17	berg	first implementation
 *						copied CSV-specific functions to separate file
 ***********************************************************************************/
 
#ifndef _ANUBISNET_CSV_
#define _ANUBISNET_CSV_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "tables.h"
#include "an_string.h"
#ifdef __unix__	
	#include <sys/stat.h>
    #define NEWLINE_LENGTH 2
	#include <unistd.h>
#elif __MSDOS__
	#include <io.h>
	#include <sys\stat.h>
#elif _WIN32
	#include <io.h>
	#include <sys\stat.h>
    #define NEWLINE_LENGTH 2
#else    
    #define NEWLINE_LENGTH 1
#endif

int writeCSVHeader (int fh, struct DB_FIELD *header[], struct DB_FIELD *dbf, int header_length);
int writeCSVLine (int fh, char *value, struct DB_FIELD *header[], struct DB_FIELD *dbf, int
header_length);
#endif
