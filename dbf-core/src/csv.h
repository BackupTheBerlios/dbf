/******************************************************************************
 * csv.h
 ******************************************************************************
 * dbf Reader and Converter for dBASE files
 * Author: Bjoern Berg <clergyman@gmx.de>
 *
 ******************************************************************************
 * This includes enable dbf to write CSV files
 ******************************************************************************
 * History:
 * $Log: csv.h,v $
 * Revision 1.7  2004/08/30 10:24:56  steinm
 * - removed include file an_string.h
 *
 * Revision 1.6  2004/08/27 05:44:11  steinm
 * - used libdbf for reading the dbf file
 *
 * Revision 1.5  2004/04/25 15:21:10  rollinhand
 * replaced tables.h with dbf.h
 *
 * Revision 1.4  2003/11/11 15:07:08  rollin_hand
 * added cvs keyword log:
 *
 *
 ******************************************************************************/

#ifndef _CSV_H_
#define _CSV_H_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#ifdef __unix__
	#include <sys/stat.h>
    #include <unistd.h>
    #define NEWLINE_LENGTH 1
#elif __MSDOS__ || _WIN32
    #include <io.h>
    #include <sys\stat.h>
    #define NEWLINE_LENGTH 2
#else
    #define NEWLINE_LENGTH 1
#endif

/*
 * Special anubisnet includes
 */
#include "dbf.h"

int setCSVSep (FILE *fp, P_DBF * p_dbf,
    const char *input, const char *separator);
int writeCSVHeader (FILE *fp, P_DBF * p_dbf,
    const char *input, const char *output);
int writeCSVLine (FILE *fp, P_DBF * p_dbf, const unsigned char *value,
    int header_length, const char *input, const char *output);
#endif
