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

#include "iodbf.h"

/* * * * DBF_OPEN 
 * open the the current dbf file
 * dbfhandle is defined as a global variable */
int dbf_open (char *file)
{
	int dbfhandle;
	if ((dbfhandle = open(file, O_RDONLY)) == -1) {
		printf("Cannot open file %s.\n", file);
		printf("Call dbf --help or dbf -h for usage\n");
		exit(1);
	}
	return (int)dbfhandle;
}

/* * * * DBF_CLOSE 
 * close the current open dbf file
 * incl. error handling routines */
int dbf_close (int fh, char *file)
{
	if((close(fh)) == -1)
		printf("Cannot close %s.\n", file);
	else
		printf("File %s was closed successfully.\n", file);
	
	return 1;
}

/* * * * EXPORT_OPEN 
 * open the export file for writing */
int export_open (char *file)
{
	int handle;
	if((handle = open(file, O_RDWR|O_CREAT,0644|O_TRUNC)) == -1) {
		printf("Cannot create/open file %s.\n", file);
		exit(1);		
	}
	return handle;
}

/* * * * EXPORT_CLOSE 
 * closes the opened file and stops the write-process */
int export_close (int fh, char *file)
{
	if((close(fh)) == -1)
		printf("Cannot close File %s.\n",file);
	else
		printf("Export file %s was closed successfully.\n",file);
	return 1;		
}