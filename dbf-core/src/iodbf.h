/***********************************************************************************
 * iodbf.h
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

#ifndef _DBF_IO_
#define _DBF_IO_
 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef __unix__	
	#include <sys/stat.h>
	#include <unistd.h>
#elif __MSDOS__
	#include <io.h>
	#include <sys\stat.h>
#elif _WIN32
	#include <io.h>
	#include <sys\stat.h>
#else
	#include <sys/stat.h>
	#include <unistd.h>
	#include <stdlib.h>
#endif	

int dbf_open (char *file);
int dbf_close (int fh, char *file);
int export_open (char *file);
int export_close (int fh, char *file);

#endif
