/***********************************************************************************
 * csv.c
 + Implementation				    														
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

#include "csv.h"

/* writeCSVHeader */
/* creates the CSV Header with the information provided by DB_FIELD */
int writeCSVHeader (int fh, struct DB_FIELD *header[], struct DB_FIELD *dbf, int header_length)
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
	}
	if((write(fh, "\n", NEWLINE_LENGTH)) == -1) {
		printf("Cannot write data to CSV File - Aborting!\n"); exit(1);
	}
	
	return 0;	
}

/* writeCSVLine */
/* creates a line in the CSV document for each data set */
int writeCSVLine (int fh, char *value, struct DB_FIELD *header[], struct DB_FIELD *dbf, int header_length) {
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
	return 0;				
}
