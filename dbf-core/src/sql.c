/***********************************************************************************
 * sql.c
 ***********************************************************************************
 * conversion of dbf files to sql
 * 
 * Version 0.2, 2003-09-08
 * Author: Dr Georg Roesler, groesle@gwdg.de
 *
 * History:
 * 2003-09-08	teterin,berg	Fixing some errors in the produced SQL statements
 *								Support for MySQL and PostGres
 * 2003-02-24	jones			some minor changes
 * - Version 0.1 - February 2003
 *	 first implementation in dbf.c
 ************************************************************************************/

#include "sql.h"

static size_t	tablelen;
/* Whether to trim SQL strings from either side: */
static int	trimright = 0;
static int	trimleft = 0;

int setSQLTrim(FILE *fp, const struct DB_FIELD * header,
    int header_length,
    const char *filename, const char *mode)
{
	if (mode[1] != '\0')
		goto invalid;
	switch (mode[0]) {
		case 'R':
		case 'r':
			trimright = 1;
			return 0;
		case 'L':
		case 'l':
			trimleft = 1;
			return 0;
		case 'B':
		case 'b':
			trimleft = trimright = 1;
			return 0;
		invalid:
		default:
			fprintf(stderr, "Invalid trim mode ``%s''. "
			    "Expecting ``r'', ``l'', or ``b'' for both",
			    mode);
			return 1;
	}
}

/* writeSQLHeader */
/* creates the SQL Header with the information provided by DB_FIELD */
int writeSQLHeader (FILE *fp, const struct DB_FIELD * header,
    int header_length,
    const char *filename, const char *export_filename)
{
	int unsigned l1,l2;
	const struct DB_FIELD *dbf;

	tablelen = strlen(export_filename) - 4; /* Also used by the line-method */

	fprintf(fp, "-- %s -- \n--\n"
	    "-- SQL code with the contents of dbf file %s\n\n"
	    "\ndrop table %.*s;\n"
	    "\nCREATE TABLE %.*s(\n",
	    export_filename, filename,
	    tablelen, export_filename,
	    tablelen, export_filename);
	for (dbf = header + 1; --header_length; dbf++) {
		fprintf(fp, "%s\t", dbf->field_name);
		switch(dbf->field_type) {
			case 'C':
				/*
				 * SQL 2 requests "character varying" at this point,
				 * but oracle, informix, dab2, MySQL and PGSQL
				 * support also "varchar". To be compatible to most
				 * SQL databases we should use varchar for the moment.
				 * - berg, 2003-09-08
				 */
				fprintf(fp, "varchar(%d)",
				    dbf->field_type == 'M' ? 10 :
					dbf->field_length);
			break;
			case 'M':
				/*
				 * M stands for memo fields which are currently not
				 * supported by dbf.
				 * - berg, 2003-09-08
				 */
				fprintf(stderr, "Invalid mode. "
			    "dbf cannot convert this dBASE file. Memo fields are not supported.");
				return 1;
			break;
			case 'N':
				l1 = dbf->field_length;
				l2 = dbf->field_decimals;
				if((l1 < 10) && (l2 == 0))
					fputs("int", fp);
				else
					fprintf(fp, "numeric(%d, %d)",
					    l1, l2);
			break;
			case 'D':
				fputs("date", fp);
				break;
			default:
				fprintf(fp, "/* unsupported type ``%c'' */",
				    dbf->field_type);
		}
		if (header_length != 1)
			fputc(',', fp);
		fputs("\n", fp);
	}
	fputs(");\n", fp);

	return 0;
}

/* writeSQLLine */
/* fills the SQL table */
int
writeSQLLine (FILE *fp, const struct DB_FIELD * header,
    const unsigned char *value, int header_length,
    const char *filename, const char *export_filename)
{
	const struct DB_FIELD *dbf;

	fprintf(fp, "INSERT INTO %.*s VALUES(\n",
	    tablelen, export_filename);

	for (dbf = header + 1; --header_length; dbf++) {
		const unsigned char *end, *begin;
		int isstring = dbf->field_type == 'M' || dbf->field_type == 'C';

		/*
		 * A string is only trimmed if trimright and/or trimleft is set
		 * Other datatypes are always "trimmed" to determine, if they
		 * are empty, in which case they are printed out as NULL -- to
		 * keep the SQL correctness.	-mi	Aug, 2003
		 */
		begin = value;
		value += dbf->field_length; /* The next field */
		end = value;

		if (isstring) {
			putc('\'', fp);
			/*
			 * Non-string data-fields are right justified
			 * and don't need right-trimming
			 */
			if (trimright) {
				while (--end != begin && *end == ' ')
					/* Nothing */;
					if (end == begin) { 
						goto endstring;	
					}
				end++;
			}			
		}

		if (trimleft || !isstring) {
			while (begin != end && *begin == ' ')
				begin++;
		}

		if (begin == end) {
			if (isstring) {				
				goto endstring;
			}
				
			fputs("NULL", fp);
			goto endfield;

		}

		do	/* Output the non-empty string:*/
			putc(*begin++, fp);
		while (begin != end);

		if (isstring)
		endstring:			
			putc('\'', fp);		

		endfield:			
			if (header_length != 1) {
			/* Not the last field */												
				putc(',', fp);
			} 
		
	}	
	/* Terminate INSERT INTO with ) and ; */ 
	fputs(");\n", fp);

	return 0;
}
