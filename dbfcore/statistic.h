/***********************************************************************************
 * statistic.h		
 * inherits the statistic functions for dBASE files	    														
 * Author: Bjoern Berg, September 2002	    											
 * Email: clergyman@gmx.de		    												
 * dbf Reader and Converter for dBase III, IV, 5.0	    											
 * Version 0.3																		
 *																					
 * History:
 * - Version 0.3 - 2003-01-25
 *   improved handling for dBASE y2k problem
 *	 improved output of table structure
 * - Version 0.2 - 2003-01-21
 *	 added new debug information -> Hex-codes    
 * - Version 0.1.1 - September 2002
 * 	 dbf.c splitted to dbf.c and statistic.h, introducing new interfaces
 * - Version 0.1 - June 2002														
 *	 first implementation ind dbf.c			
 ************************************************************************************/

#ifndef DB_TABLES
#include "tables.h"
#define DBTABLES 1
#endif

#include "congraph.h"
#include "dbfversion.h"

/* output for header statistic */
void dbf_file_info (struct DB_HEADER *db)
{    
	char yk;
	int *dbfinit;
	yk = 19;
	
	if(db->last_update[0] > 100) {
		db->last_update[0]=(((int)db->last_update[0])-100);		
		yk = 20;
	}
	
	dbfinit = (int*)dbfexamine(db->version);
	
	printf("\n-- File statistics\n");
	printf("dBase version.........: \t %d (%s)\n", dbfinit[0], dbfinit[1]?"with memo":"without memo");
	printf("Date of last update...: \t %d%02d-%02d-%02d\n", yk, db->last_update[0], db->last_update[1], db->last_update[2]);
	printf("Number of records.....: \t %d (%08xd)\n", rotate4b(db->records), rotate4b(db->records));
	printf("Length of header......: \t %d (%04xd)\n", rotate2b(db->header_length),
	rotate2b(db->header_length));
	printf("Record length.........: \t %d (%04xd)\n", rotate2b(db->record_length), rotate2b(db->record_length));
	printf("Columns in file.......: \t %d \n", rotate2b(db->header_length)/32);
	printf("Rows in file..........: \t %d\n\n", rotate4b(db->records));
}

/* output for field statistic */
void dbf_field_stat (struct DB_FIELD *header[], int header_length)
{
	int i, linelength;
	struct DB_FIELD *dbf;	
	int cross[] = {1,17,25,41,57,73};	
	linelength=73;		
	
	drawline(linelength, cross, (sizeof(cross)/sizeof(int)));
	printf("| field name\t| type\t| field adress\t| length\t| field dec.\t|\n");
	drawline(linelength, cross, sizeof(cross)/sizeof(int));	
	for (i = 1; i < header_length; i++)
	{
		dbf = header[i];		
		printf("|%13.11s\t| %3c\t| %8x\t| %3d\t\t| %3d\t\t|\n", 
			   dbf->field_name, dbf->field_type, rotate4b(dbf->field_adress), dbf->field_length, dbf->field_decimals);
	}	
	drawline(linelength, cross, sizeof(cross)/sizeof(int));
}
