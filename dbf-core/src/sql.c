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

#include "sql.h"

/*#ifndef _SQL_EXPORT_ 
#define _SQL_EXPORT_

static int field_type[MAX_FIELDS];*/

/* writeSQLHeader */
/* creates the SQL Header with the information provided by DB_FIELD */
int writeSQLHeader (int handle,struct DB_FIELD *header[], struct DB_FIELD *dbf, int header_length,char *filename,char *export_filename)
{
	int unsigned i,l1,l2;
	char *q;
	char buffer[65536], table[32],lg[12];	

	strncpy(table,export_filename,strlen(export_filename)-4);
	memset(buffer, 0, 65535);
	q = buffer;
	strcat(q,"-- ");	
	strcat(q,export_filename);	
	strcat(q," -- \n--\n-- SQL code with the contents of dbf file ");
	strcat(q,filename);
	strcat(q,"\n\ndrop table ");
	strcat(q,table);
	strcat(q,";\n\nCREATE TABLE ");
	strcat(q,table);
	strcat(q,"(\n");
 	for(i=1; i < (unsigned int)header_length; i++) {
		dbf = header[i];
		strcat(q,dbf->field_name);
		strcat(q,"\t");
		switch(dbf->field_type) {
			case 'C':
				strcat(q,"character varying(");
				sprintf(lg,"%d",dbf->field_length);
				strcat(q,lg);
				strcat(q,")");
				field_type[i] = IS_STRING;
			break;
			case 'N':
				l1 = dbf->field_length;
				l2 = dbf->field_decimals;
				if((l1 < 10) && (l2 == 0))
					strcat(q, "int");
				else
					sprintf(lg,"numeric(%d, %d)",l1,l2);
					strcat(q, lg);
				field_type[i] = IS_NUMERIC;
			break;
			case 'M':
				strcat(q, "character varying(10)");
				field_type[i] = IS_STRING;
			break;
			default:
			break;
		} 								
		if(i < header_length - 1)
			strcat(q,",");
		strcat(q,"\n");
	}
	strcat(q,");\n");
printf("Writing %s\n", buffer);
 	if((write(handle, buffer, strlen(buffer))) == -1) {
		printf("Cannot write data to SQL File - Aborting!\n"); exit(1);
	}

	return 0;
}

/* writeSQLLine */
/* fills the SQL table */
int writeSQLLine (int handle,struct DB_FIELD *header[], struct DB_FIELD *dbf,char *value, int header_length,char *filename, char *export_filename) {
	char *p, *q;
	int i, x;
	char buffer[65536];
	char NewString[65536];
	char table[32];	
	
	strncpy(table,export_filename,strlen(export_filename)-4);

	p = value;

	memset(buffer, 0, 65535);
	q = buffer;
	strcat(q,"INSERT INTO ");
	strcat(q,table);
	strcat(q," VALUES(\n");
	if((write(handle, buffer, strlen(buffer))) == -1) {
		printf("Cannot write data to SQL File - Aborting!\n"); exit(1);
	}
	for (i=1; i < (unsigned int)header_length; i++)
	{
		memset(buffer, 0, 65535);
		memset(NewString, 0, 65535);
		dbf = header[i];
		q = buffer;
		x = dbf->field_length;
		if(field_type[i] == IS_STRING)
			*q++ = '\'';
		while(x--){
			*q++ = *p++;
		}
		trim_spaces(NewString, buffer);
		if((field_type[i] == IS_NUMERIC) && (strlen(NewString) == 0))
			strcat(NewString,"0");
		if(field_type[i] == IS_STRING)
			strcat(NewString,"\'");
		if(i < header_length - 1)
			strcat(NewString,",");
 		if((write(handle, NewString, strlen(NewString))) == -1) {
			printf("Cannot write data to SQL File - Aborting!\n");
			exit(1);
		}
	}
	sprintf(buffer,");\n");
	if((write(handle, buffer, strlen(buffer))) == -1) {
		printf("Cannot write data to SQL File - Aborting!\n"); exit(1);
	}
	return 0;				
}
