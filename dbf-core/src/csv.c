/****************************************************************************
 * csv.c
 ****************************************************************************
 * dbf Reader and Converter for dBASE files
 * Implementation
 *
 * Author: Bjoern Berg <clergyman@gmx.de>
 * Modifications: Uwe Steinmann <uwe@steinmann.cx>
 *
 ****************************************************************************
 * Functions to write CSV files
 ****************************************************************************
 * History:
 * $Log: csv.c,v $
 * Revision 1.16  2004/08/27 09:18:55  steinm
 * - use gettext for outputing text
 *
 * Revision 1.15  2004/08/27 05:44:11  steinm
 * - used libdbf for reading the dbf file
 *
 * Revision 1.14  2004/03/16 20:57:36  rollinhand
 * Code Cleanup
 *
 * Revision 1.13  2003/12/04 12:10:48  steinm
 * - skip 0 at end of field value
 *
 * Revision 1.12  2003/11/12 08:50:32  steinm
 * - simplyfied the masking of the separator char within a value of a field
 * - fixed the masking. The separator char is masked with another separator char
 *
 * Revision 1.11  2003/11/12 07:51:02  steinm
 * - put variable declaration at start of function because some older
 *   gcc can handle it otherwise
 ***************************************************************************/

#include <libdbf/libdbf.h>
#include "csv.h"

/* CSVFileType describes the type the converted file is of: (C)SV or (T)SV */
static char CSVFileType = 'C';
static char CSVSeparator = ',';
static char CSVEnclosure = '"';
static int CSVTableStructure = 1;

/* setCSVSep() {{{
 * allows to change the separator used for CSV output
 */
int
setCSVSep(FILE *fp, P_DBF *p_dbf,
    const char *in /* __unused */, const char *separator)
{
	if ( separator[1] && separator[0] != 't' ) {
		fprintf(stderr, _("Separator / Escape char ``%s'' is too long -- must be a single character."),
		    separator);
		fprintf(stderr, "\n");
		return 1;
	} else if ( separator[0] == 't' ) {
		CSVSeparator = '\t';
		CSVFileType = 'T';
	} else {
		CSVSeparator = separator[0];
	}
	return 0;
}
/* }}} */

/* writeCSVHeader() {{{
 * creates the CSV Header with the information provided by DB_FIELD
 */
int
writeCSVHeader (FILE *fp, P_DBF *p_dbf,
    const char *in /* __unused */, const char *out /* __unused */)
{
	int i, columns;

	columns = dbf_NumCols(p_dbf);
	for (i = 0; i < columns; i++) {
		char field_type;
		const char *field_name;
		int field_length, field_decimals;
		field_type = dbf_ColumnType(p_dbf, i);
		field_name = dbf_ColumnName(p_dbf, i);
		field_length = dbf_ColumnSize(p_dbf, i);
		field_decimals = dbf_ColumnDecimals(p_dbf, i);
		if(CSVTableStructure && CSVSeparator == ',')
			fputs("\"", fp);
		fprintf(fp, "%s", field_name);
		if(CSVTableStructure) {
			fprintf(fp, ",%c", field_type);
			switch(field_type) {
				case 'C':
					fprintf(fp, ",%d", field_length);
					break;
				case 'N':
					fprintf(fp, ",%d,%d", field_length, field_decimals);
					break;
			}
		}
		if(CSVTableStructure && CSVSeparator == ',')
			fputs("\"", fp);
		if(i > 0)
			putc(CSVSeparator, fp);
	}
	fputs("\n", fp);

	return 0;
}
/* }}} */

/* writeCSVLine {{{
 * creates a line in the CSV document for each data set
 */
int
writeCSVLine(FILE *fp, P_DBF *p_dbf,
    const unsigned char *value, int header_length,
    const char *in /* unused */, const char *out /* unused */)
{
	int i, columns;

	columns = dbf_NumCols(p_dbf);

	for (i = 0; i < columns; i++) {
		const unsigned char *end, *begin;
		int isstring, isfloat;
		char field_type;
		const char *field_name;
		int field_length, field_decimals;
		int dbversion = dbf_GetVersion(p_dbf);
		field_type = dbf_ColumnType(p_dbf, i);
		field_name = dbf_ColumnName(p_dbf, i);
		field_length = dbf_ColumnSize(p_dbf, i);
		field_decimals = dbf_ColumnDecimals(p_dbf, i);

		isstring = field_type == 'M' || field_type == 'C';
		isfloat = field_type == 'F' || (field_type == 'B' && dbversion == VisualFoxPro) ? 1 : 0;

		begin = value;
		value += field_length;
		end = value - 1;

		/* Remove NULL chars at end of field */
		while(end != begin && *end == '\0')
			end--;

		/*
		 * addded to keep to CSV standard:
		 * Text fields must be enclosed by quotation marks
		 * - berg, 2003-09-08
		 */
		if ( isstring && CSVFileType == 'C' )
			putc(CSVEnclosure, fp);

		while (*begin == ' ' && begin != end)
			begin++;

		if (*begin != ' ') {

			while (*end == ' ')
				end--;

			/* This routine must be verified in several tests */
			if (isfloat) {
				char *fmt = malloc(20);
				sprintf(fmt, "%%%d.%df", field_length, field_decimals);
				fprintf(fp, fmt, *(double *)begin);
				begin += field_length;
			} else {
				do {
					/* mask enclosure char */
					if(*begin == CSVEnclosure) {
							putc(CSVEnclosure, fp);
					}
					putc(*begin, fp);
				} while (begin++ != end);
			}
		}

		if ( isstring && CSVFileType == 'C')
			putc(CSVEnclosure, fp);

		if(i < columns-1)
			putc(CSVSeparator, fp);
	}
	fputs("\n", fp);

	return 0;
}
/* }}} */
