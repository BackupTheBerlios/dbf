/***********************************************************************************
 * iodbf.h		
 * inherits the standard I/O file commands	    														
 * Author: Bjoern Berg, September 2002	    											
 * Email: clergyman@gmx.de		    												
 * dbf Reader and Converter for dBase 3	    											
 * Version 0.1																		
 *																					
 * History:																														
 * - Version 0.1 - September 2002														
 *	 first implementation			
 ************************************************************************************/

#include <stdio.h>
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
#endif	

/*int dbfhandle,csvhandle;*/

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

/* * * * CSV_OPEN 
 * open the csv file for writing */
int csv_open (char *file)
{
	int csvhandle;
	if((csvhandle = open(file, O_RDWR|O_CREAT,0777|O_TRUNC)) == -1) {
		printf("Cannot create/open file %s.\n", file);
		exit(1);		
	}
	return 1;
}

/* * * * CSV_CLOSE 
 * closes the opened csv-file and stops the write-process */
int csv_close (int fh, char *file)
{
	if((close(fh)) == -1)
		printf("Cannot close CSV-File.\n");
	else
		printf("CSV-File was closes successfully.\n");
	return 1;		
}
