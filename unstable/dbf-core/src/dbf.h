/***********************************************************************************
 * dbf.h
 ***********************************************************************************			    														
 * Author: Bjoern Berg, June 2002	    											
 * Email: clergyman@gmx.de		    												
 * dbf Reader and Converter for dBASE files    											
 * Version 0.6																	 
 ***********************************************************************************/

#ifndef __DBF_CORE__
#define __DBF_CORE__

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
/*#define VERSION1 0
#define VERSION2 5
#define VERSION3 0
#define VERSION4 1*/
#define SQL_OUTPUT 1
#define CSV_OUTPUT 2
#define INFO_OUTPUT 3
/*#define DBF_VERSION 0x07
#define DBF_MEMO3 0x80
#define DBF_MEMO4 0x08*/

/* maximum number of fields within a database, increased from 50 to 500
 * hint by Uwe Steinmann, 2003-04-14 */
#define MAX_FIELDS 500	

/* special anubisnet and dbf includes */
#include "codepages.h"
#include "iodbf.h"
#include "an_string.h"
#include "statistic.h"
#include "endian.h"
#include "csv.h" 

/* SQL relies on some things that are defined above .. this is not ideal */
#include "sql.h"

/* F U N C T I O N S */
int dbf_read_header (int fh, char *file);
int getHeaderValues (int fh, char *file, int header_length);
void show_help(char *pname);


/* V A R I A B L E S */
struct DB_HEADER db_buf, *db = &db_buf;
struct DB_FIELD *header[MAX_FIELDS], *dbf;
char export[4];						/* stores the format to convert into: csv, txt */
char *export_filename;				/* stores the filename to export to */
int isbigendian;					/* verifies if the used system is of type Big Endian or not */

#endif
