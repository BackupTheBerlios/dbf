/***********************************************************************************
 * dbf.c				    														
 * Author: Bjoern Berg, June 2002	    											
 * Email: clergyman@gmx.de		    												
 * dbf Reader and Converter for dBASE files    											
 * Version 0.5beta02																	
 *																					
 * History:
 * - Version 0.5
 *   all SQL functions are commented due to compiler problems	
 * - Version 0.5beta02 - 07.02.2003
 *   changes in help()
 *	 Big Endian Check Override Flag
 *   some minor bug fixes and feature enhancements
 * - Version 0.5beta01 - 01.02.2003
 *   support for Big Endian systems
 * - Version 0.3.1.1 - 30.11.2002
 *   code cleanups for MSVC, prevents some warnings
 * - Version 0.3.1 - 23.11.2002
 *   fixed memory leak (trying to convert a 284Mb file used 900Mb
 *   of memory and got killed) send in by:
 *   Andy Jeffries, Email: opensource@andyjeffries.co.uk
 * - Version 0.3.0 - [not yet released]
 *	 changed complete internal structure
 *   for more details see CHANGELOG
 * - Version 0.2.7 
 *   code cleanups for MSVC
 * - Version 0.2.6 - 20.09.2002
 *   dbf.c splitted to dbf.c, statistic.h, iodbf.h, an_string.h
 *	 minor bugfixes in output
 * - Version 0.2.5 - 14.09.2002
 *	 Code Page support for umlauts and other special characters added (codepages.h)
 * - Version 0.2 - 28.07.2002
 *	 Converter for csv implemented
 *   function trim_spaces implemented
 * - Version 0.1 - June 2002
 *	 Output for dBase3 databases, based on a version by Joachim Astel, 1989
 ************************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
/*#define NEWLINE_LENGTH 1*/
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
#endif
#define VERSION1 0
#define VERSION2 5
#define VERSION3 0
#define VERSION4 1
#define SQL_OUTPUT 1
#define CSV_OUTPUT 2
#define INFO_OUTPUT 3
#define IS_STRING 1
#define IS_NUMERIC 2
#define DBF_VERSION 0x07
#define DBF_MEMO3 0x80
#define DBF_MEMO4 0x08


/* special anubisnet and dbf includes */
#include "codepages.h"
#include "iodbf.h"
#include "an_string.h"
#include "statistic.h"
#include "endian.h"
/*#include "sql.h"*/

#define MAX_FIELDS 50				/* maximum number of fields within a database */

struct DB_HEADER db_buf, *db = &db_buf;
struct DB_FIELD *header[MAX_FIELDS], *dbf;
char export[4];						/* stores the format to convert into: csv, txt */
char *export_filename;				/* stores the filename to export to */
int isbigendian;					/* verifies if the used system is of type Big Endian or not */


/* get the header, dBase standard */
/* header similar for all dBASE versions, see tables.h for more details */
int dbf_read_header (int fh, char *file)
{	
	if ((read( fh, (char *)db, sizeof(struct DB_HEADER))) == -1 ) {
		printf("Error reading file %s.\n", file);
		exit(1);
	}
	
	return 1;
}

/* getHeaderValues */
/* fills the struct DB_FIELD with field names and other values*/
int getHeaderValues (int fh, char *file, int header_length)
{
	int i;
	for (i = 1; i < header_length; i++)
	{
		if((header[i] = (struct DB_FIELD *)malloc(sizeof(struct DB_FIELD))) == NULL) {
			printf("Fatal error: Cannot malloc.\n");
			exit(1);
		}
		if((read(fh, (char *)header[i], sizeof(struct DB_FIELD))) == -1) {
			printf("Error reading file %s.\n", file);
			exit(1);
		}
	}
	return 0;
}

/* writeCSVHeader */
/* creates the CSV Header with the information provided by DB_FIELD */
int writeCSVHeader (int fh, struct DB_FIELD *header[], int header_length)
{
	int i;
	char *q;
	char buffer[65536];	
	
	for(i=1; i < header_length; i++) {
		memset(buffer, 0, 65535);
		dbf = header[i];			
		strcpy(buffer,dbf->field_name);									
		q = buffer+strlen(buffer);
		*q++ = ';';
		*q = '\0';
		if((write(fh, buffer, strlen(buffer))) == -1)
		{
			printf("Cannot write data to CSV File - Aborting!\n"); exit(1);
		}
		free(buffer);												
	}
	if((write(fh, "\n", NEWLINE_LENGTH)) == -1) {
		printf("Cannot write data to CSV File - Aborting!\n"); exit(1);
	}
	
	return 0;	
}

/* writeCSVLine */
/* creates a line in the CSV document for each data set */
int writeCSVLine (int fh, char *value, int header_length) {
	char *p, *q;
	int i, x;
	char buffer[65536];
	char NewString[65536];
	
	p = value;
	
	for (i=1; i < header_length; i++)
	{
		memset(buffer, 0, 65535);
		memset(NewString, 0, 65535);
		dbf = header[i];													    
		q = buffer;		
		x = dbf->field_length;
		while(x--)
			*q++ = *p++;
		*q++ = ';';				
				
		trim_spaces(NewString, buffer);																
													
		if((write(fh, NewString, strlen(NewString))) == -1) {
			printf("Cannot write data to CSV File - Aborting!\n");
			exit(1);
		}
	}
	if((write(fh, "\n", NEWLINE_LENGTH)) == -1) {
		printf("Cannot write data to CSV File - Aborting!\n"); exit(1);
	}
	free(buffer);
	free(NewString);
	return 0;				
}

/* Help */
void show_help(char *pname) {
    printf("Usage: %s [option][argument] dbf-file, -h for help\n", pname);
	printf("\n Options:");
    printf("\n  -h or --help \t\t shows this screen");
	printf("\n  --csv [csv-file] \t convert dBASE File to csv (readable with spread sheets)");
    /*printf("\n  --sql [sql-file] \t converts dBASE File to sql (tested with Postgres)");*/ 	
	printf("\n  --view-info \t\t displays statistics about current dbf file");
	printf("\n  --ppc \t\t force dbf not to check the system architecture");
	printf("\n\nPlease note that the current version automatically detects\n\
the codepage used in the dbf file.\n\
At the moment only english, american and western european codepages are supported.\n");				 
}

/* - - - THE MAIN PROGRAM - - -*/
/* the part of Joachim Astel was completely re-written and adopted for version 0.2 */
int main (int argc, char *argv[])
{	
	int dbfhandle, handle;
	int header_length, i;
	char *filename;
	int ppc_override; // Big Endian Check Override
	int type;
	ppc_override = 0;
	
	
	printf("dBase Reader and Converter V. 0.5, (c) 2003 by Bjoern Berg\n");
	/*printf("Build: %s, %s\n", __DATE__, __TIME__);*/	
		
	if (argc < 2) {
		printf("Usage: %s [option][argument] dbf-file, -h for help\n", *argv);
		exit(1);
	} 
	
	/* check for override flags, developer versions */
	for(i=1; i < argc; i++) {
		if(strcmp(argv[i], "--ppc")==0) ppc_override = 1;
	} 
		
	/* Check if someone needs help */
	for(i=1; i < argc; i++) {
		if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0) {
			show_help(*argv);
			exit(1);
		}
	}	
	
	/* check architecture: little-endian / big-endian */
	if(ppc_override == 1) {
		isbigendian = _false;
		printf("Override: Do not check on Big Endian / Little Endian System\n");
	} else isbigendian = IsBigEndian();	 	
	
	if(isbigendian == _true) {
		printf("System: Big Endian\n");
		printf("The current version does not support architectures using Big Endian coding\n");
		printf("A B O R T I N G!\n");
		exit(1);
	} else printf("System: Little Endian\n");
	
	/* fill filename with last argument */
	/* Test if last argument is an option or a possible valid filename */
	if(argv[(argc-1)][0] == 0x2D) {
		printf("\nERROR: Found no file for input\n");
		printf("Please make sure that the last argument is a valid dBASE file\n");
		exit(1);
	} else 	filename = argv[(argc-1)]; 
	
	/* fill export_filename, if necessary */
	/* -c csv export */
	for(i=1; i < argc; i++) {	     
		if(strcmp(argv[i],"--csv")==0) {
			  if((i+2) < argc) {
		    	export_filename = argv[i+1];
                type = CSV_OUTPUT;
               } else {
		     	printf("Usage: %s --csv export_filename dbf-file, -h for help\n", *argv);
		     	exit(1);
		   	   }
		}
	}	

    /* sql export */
    /*for(i=1; i < argc; i++) {            
    	if(strcmp(argv[i],"--sql")==0) {
        	if((i+2) < argc) {
            	export_filename = argv[i+1];
                type = SQL_OUTPUT;
            } else {
                printf("Usage: %s --sql export_filename dbf-file, -h for help\n", *argv);
                exit(1);
            } 
        }
    } */      

	/* Statistic Output */
	/* new since version 0.3.0 BETA 1 */
	for(i=1; i < argc; i++) {	     
		if(strcmp(argv[i],"--view-info")==0) {
			dbfhandle = dbf_open(argv[(argc-1)]);
			if(dbfhandle) {
				dbf_read_header(dbfhandle, filename);
				dbf_file_info(db);
				header_length = rotate2b(db->header_length) / 32;
				getHeaderValues(dbfhandle,filename,header_length);
				dbf_field_stat(header,header_length);
				exit(1);				
			} else {
				printf("Cannot open file %s\n", argv[(argc-1)]);
				printf("Usage: %s file --view-info\n", argv[0]);
				exit(1);
			}
			dbf_close(dbfhandle, filename);
		}
	}
	
	dbfhandle = dbf_open(filename);
	if(dbfhandle)
	{		
		dbf_read_header(dbfhandle, filename);		
		header_length = rotate2b(db->header_length) / 32;
		getHeaderValues(dbfhandle,filename,header_length);
		char *foo, *p;
		int i, k;						
		lseek(dbfhandle, rotate2b(db->header_length) + 1, 0);
		
		if((foo = (char *)malloc(rotate2b(db->record_length))) == NULL)	{
			printf("Fatal error: Cannot malloc.\n"); exit(1);
		}
		
		/* Export, write the Header to export_filename (SQL or CSV)*/			
		if(export_filename) {
		  printf("Export from %s to %s\n",filename,export_filename);
		  handle = export_open(export_filename);
		  switch(type) {
		    case SQL_OUTPUT:
			    /*writeSQLHeader(handle,header,header_length, filename, export_filename);*/
		    	break;
            default:
			    writeCSVHeader(handle, header,header_length);                           
		        break;
          }                                               
		}
		
		/* foo, p inherits full data set */
		while ((read(dbfhandle, (p = foo), rotate2b(db->record_length))))
		{
			/* automatic convert options */
		  	cp850convert(p);					
		  	ASCIInumbers(p);
			/* If export_filename is not set output data to screen, otherwise to export_filename */
		  	if(!export_filename) {
		    	for (i=1; i < header_length; i++)
				{
			  		dbf = header[i];										
			  		k = dbf->field_length;
	                printf("%11.11s: ", dbf->field_name);
			  		while (k--)
			     		putchar(*p++);
			  		putchar('\n');
				}
				putchar('\n');
		  	} else {
		      	switch(type) {
              	case SQL_OUTPUT:
					/*writeSQLLine(handle, p, header_length, filename, export_filename);*/
					break;
		      	default:
					writeCSVLine(handle, p, header_length);
					break;
				}	
		  	} /* End of --> inner if */		  
		} /* End of --> while */
	} /* End of --> if dbfhandle */
	dbf_close(dbfhandle,filename);
	export_close(handle,export_filename);

	return 0;
}
