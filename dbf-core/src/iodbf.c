/***********************************************************************************
 * iodbf.c
 * inherits the standard I/O file commands
 * Author: Bjoern Berg, September 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBase 3
 * Version 0.2
 *
 * History:
 * 	2003-02-16	jones	Added another default for the #ifdef OS block ..
 * - Version 0.1 - September 2002
 *	 first implementation
 ************************************************************************************/

#include <stdio.h>
#include "iodbf.h"
#include "dbf.h"

/* * * * DBF_OPEN
 * open the the current dbf file
 * dbfhandle is defined as a global variable */
int
dbf_open(const char *file)
{
	int dbfhandle;

	if (file[0] == '-' && file[1] == '\0')
		return fileno(stdin);

	if ((dbfhandle = open(file, O_RDONLY)) == -1) {
		fprintf(stderr, "Can not open file %s.\n"
		    "try --help or -h for usage\n", file);
		exit(1);
	}

	return dbfhandle;
}

/* * * * DBF_CLOSE
 * close the current open dbf file
 * incl. error handling routines */
int
dbf_close(int fh, const char *file)
{
	if (fh == fileno(stdin))
		return 0;

	if((close(fh)) == -1) {
		fputs("Can not close ", stderr);
		perror(file);
		return 1;
	}

	if (verbosity > 2)
		fprintf(stderr, "File %s was closed successfully.\n", file);
	return 0;
}

/* * * * EXPORT_OPEN
 * open the export file for writing */
FILE *
export_open(const char *file)
{
	FILE *result;
	if (file == NULL || (file[0] == '-' && file[1] == '\0'))
		return stdout;
	if ((result = fopen(file, "w")) == NULL) {
		perror(file);
		exit(1);
	}
	return result;
}

/* * * * EXPORT_CLOSE
 * closes the opened file and stops the write-process */
int
export_close(FILE *fp, const char *file)
{
	if (fp == stdout)
		return 0;
	if (fclose(fp)) {
		fputs("Cannot close File ", stderr);
		perror(file);
		return 1;
	}
	if (verbosity > 2)
		fprintf(stderr, "Export file %s was closed successfully.\n",file);
	return 0;
}
