/*****************************************************************************
 * sql.c
 *****************************************************************************
 * conversion of dbf files to sql
 * 
 * Author: 	Dr Georg Roesler, groesle@gwdg.de
 * 			Mikhail Teterin,
 *			Björn Berg, clergyman@gmx.de
 *
 * History:
 * $Log: sql.c,v $
 * Revision 1.13  2004/08/28 16:30:46  steinm
 * - many small improvements
 *
 * Revision 1.12  2004/08/27 09:18:55  steinm
 * - use gettext for outputing text
 *
 * Revision 1.11  2004/08/27 05:44:11  steinm
 * - used libdbf for reading the dbf file
 *
 * Revision 1.10  2004/03/16 21:01:46  rollinhand
 * New flag to prevent Drop/Create statements by user interaction
 *
 * Revision 1.9  2004/01/03 15:42:40  rollinhand
 * fixed problems with NULL values
 *
 ****************************************************************************/

#include "dbf.h"
#include "sql.h"

/* Whether to trim SQL strings from either side: */
static int	trimright = 0;
static int	trimleft = 0;
static unsigned int sql_drop_table = 1;
static unsigned int sql_create_table = 1;

/* setNoDrop() {{{
 * disable output of DROP TABLE statement
 */
int
setNoDrop(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	sql_drop_table = 0;
	return 0;
}
/* }}} */

/* setNoCreate() {{{
 * disable output of CREATE TABLE statement
 */
int
setNoCreate(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	sql_create_table = 0;
	return 0;
}
/* }}} */

int setSQLTrim(FILE *fp, P_DBF *p_dbf,
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
			fprintf(stderr, _("Invalid trim mode ``%s''. Expecting ``r'', ``l'', or ``b'' for both."),
			    mode);
			return 1;
	}
}

/* writeSQLHeader */
/* creates the SQL Header with the information provided by DB_FIELD */
int writeSQLHeader (FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename)
{
	fprintf(fp, "-- %s -- \n--\n"
	    "-- SQL code with the contents of dbf file %s\n\n", export_filename, filename);

	if ( sql_drop_table ) {
		fprintf(fp, "\nDROP TABLE %s;\n", tablename);
	}   
	if ( sql_create_table ) {
		int unsigned l1,l2;
		int i, columns;

		fprintf(fp, "\nCREATE TABLE %s (\n", tablename);

		columns = dbf_NumCols(p_dbf);
		for (i = 0; i < columns; i++) {
			char field_type;
			const char *field_name;
			int field_length, field_decimals;
			field_type = dbf_ColumnType(p_dbf, i);
			field_name = dbf_ColumnName(p_dbf, i);
			field_length = dbf_ColumnSize(p_dbf, i);
			field_decimals = dbf_ColumnDecimals(p_dbf, i);
			fprintf(fp, "  %-11s ", field_name);
			switch(field_type) {
				case 'C':
					/*
					 * SQL 2 requests "character varying" at this point,
					 * but oracle, informix, db2, MySQL and PGSQL
					 * support also "varchar". To be compatible to most
					 * SQL databases we should use varchar for the moment.
					 * - berg, 2003-09-08
					 */
					fprintf(fp, "varchar(%d)", field_type == 'M' ? 10 : field_length);
				break;
				case 'M':
					/*
					 * M stands for memo fields which are currently not
					 * supported by dbf.
					 * - berg, 2003-09-08
					 */
					fprintf(stderr, _("Invalid mode. Cannot convert this dBASE file. Memo fields are not supported."));
					return 1;
				break;
				case 'I':
					fputs("int", fp);
				break;
				case 'N':
					l1 = field_length;
					l2 = field_decimals;
					if((l1 < 10) && (l2 == 0))
						fputs("int", fp);
					else
						fprintf(fp, "numeric(%d,%d)",
								l1, l2);
				break;
				case 'F':
					l1 = field_length;
					l2 = field_decimals;
					fprintf(fp, "numeric(%d, %d)", l1, l2);
				break;
				case 'B': {
					/*
					 * In VisualFoxPro 'B' stands for double so it is an int value
					 */
					int dbversion = dbf_GetVersion(p_dbf);
					if ( dbversion == VisualFoxPro ) {
						l1 = field_length;
						l2 = field_decimals;
						fprintf(fp, "numeric(%d, %d)", l1, l2);
					} else if ( dbversion == dBase3 ) {
							fprintf(stderr, _("Invalid mode. Cannot convert this dBASE file. Memo fields are not supported."));
						return 1;
					}

				break;
				}
				case 'D':
					fputs("date", fp);
				break;
				case 'L':
					/*
					 * Type logical is not supported in SQL, you have to use number
					 * resp. numeric to keep to the standard
					 */
					 fprintf(fp, "boolean");
				break;
				default:
					fprintf(fp, "/* unsupported type ``%c'' */", field_type);
			}
			if (i < columns-1)
				fputc(',', fp);
			fputs("\n", fp);
		}
		fputs(");\n\n", fp);
	}

	return 0;
}

/* writeSQLLine */
/* fills the SQL table */
int
writeSQLLine (FILE *fp, P_DBF *p_dbf, 
    const unsigned char *value, int header_length,
    const char *filename, const char *export_filename)
{
	int i, columns;

	columns = dbf_NumCols(p_dbf);

	fprintf(fp, "INSERT INTO %s VALUES (", tablename);

	for (i = 0; i < columns; i++) {
		const unsigned char *end, *begin;
		char field_type;
		field_type = dbf_ColumnType(p_dbf, i);
		int isstring = (field_type == 'M' || field_type == 'C');
		int isdate = (field_type == 'D');
		int isbool = (field_type == 'L');

		/*
		 * A string is only trimmed if trimright and/or trimleft is set
		 * Other datatypes are always "trimmed" to determine, if they
		 * are empty, in which case they are printed out as NULL -- to
		 * keep the SQL correctness.	-mi	Aug, 2003
		 */
		begin = value;
		value += dbf_ColumnSize(p_dbf, i); /* The next field */
		end = value;

		/* Remove NULL chars at end of field */
		while(--end != begin && *end == '\0')
			;
		end++;

		if (isdate && begin != end) {
			/*
			 * SQL syntax requires quotes around date strings
			 * t2r@wasalab.com, Oct 2003
			 */
			putc('\'', fp);
		}

		if (isstring && begin != end) {
			putc('\'', fp);
			/*
			 * Non-string data-fields are right justified
			 * and don't need right-trimming
			 */
			if (trimright) {
				while (--end != begin && *end == ' ')
					/* Nothing */;
					if (end == begin && *end == ' ') {
						goto endstring;
					}
				end++;
			}
		}

		if (trimleft || !isstring) {
			while (begin != end && *begin == ' ')
				begin++;
		}

		/*
		 * If date field value was missing, "valid" data should have been
		 * written. [...] In my application I can live with date like 1970-01-01.
		 * - Tommi Rintala, by email, Oct 2003
		 */
		/*if (isdate) {
			fputs("19700101", fp);
			goto endstring;
		}*/

		if (begin == end) {
			if (isstring) {
				goto endstring;
			}

			fputs("NULL", fp);
			goto endfield;
		}

		if (isbool) {
			char sign = *begin++;
			if ( sign == 't' || sign == 'y' || sign == 'T' || sign == 'Y') {
				fprintf(fp, "true");
			} else {
				fprintf(fp, "false");
			}

		} else if (field_type == 'B' || field_type == 'F') {

			char *fmt = malloc(20);
			sprintf(fmt, "%%%d.%df", dbf_ColumnSize(p_dbf, i), dbf_ColumnDecimals(p_dbf, i));
			fprintf(fp, fmt, *(double *)begin);
			begin += dbf_ColumnSize(p_dbf, i);

		} else {

			do	{ /* Output the non-empty string:*/

				char sign = *begin++;	/* cast operations */
				switch (sign) {
					case '\'':
						putc('\\', fp);
						putc('\'', fp);
						break;
					case '\"':
						putc('\\', fp);
						putc('\"', fp);
						break;
					default:
						putc(sign, fp);
				}
			} while (begin != end);

		}

		if (isstring || isdate)
		endstring:
			putc('\'', fp);

		endfield:
			if (i < columns-1) {
			/* Not the last field */
				putc(',', fp);
			}

	}
	/* Terminate INSERT INTO with ) and ; */
	fputs(");\n", fp);

	return 0;
}
