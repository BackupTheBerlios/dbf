/***********************************************************************************
* sql.h
* conversion of dbf files to sql
* only tested with postgres
* Author: Dr Georg Roesler, February 2003
* Email: groesle@gwdg.de
*
* Implemented for: dbf Reader and Converter for dBase 3
* Version 0.5
*
* History:
*	2003-02-24	jones	some minor changes
* - Version 0.1 - February 2003
*	 first implementation in dbf.c
************************************************************************************/

#ifndef _SQL_EXPORT_
#define _SQL_EXPORT_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#ifdef __unix__
#	include <sys/stat.h>
#	include <unistd.h>
#elif __MSDOS__
#	include <io.h>
#	include <sys\stat.h>
#elif _WIN32
#	include <io.h>
#	include <sys\stat.h>
#endif

#include "an_string.h"
#include "tables.h"

int setSQLTrim(FILE *fp, const struct DB_FIELD * header, int header_length,
    const char *filename, const char *mode /* ``l''. or ``r'', or ``b'' */);
int writeSQLHeader(FILE *fp, const struct DB_FIELD * header, int header_length,
    const char *filename, const char *export_filename);
int writeSQLLine(FILE *fp, const struct DB_FIELD * header,
    const unsigned char *value, int header_length,
    const char *filename,  const char *export_filename);

#endif
