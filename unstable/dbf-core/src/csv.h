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
#	include <sys/stat.h>
#	include <unistd.h>
#	define NEWLINE_LENGTH 1
#elif __MSDOS__ || _WIN32
#	include <io.h>
#	include <sys\stat.h>
#	define NEWLINE_LENGTH 2
#else
#	define NEWLINE_LENGTH 1
#endif

#include "dbf.h"

int setCSVSep (FILE *fp, const struct DB_FIELD * header,
    int header_length, const char *input, const char *separator);
int writeCSVHeader (FILE *fp, const struct DB_FIELD * header,
    int header_length, const char *input, const char *output);
int writeCSVLine (FILE *fp, const struct DB_FIELD * header, const unsigned char *value,
    int header_length, const char *input, const char *output);
#endif
