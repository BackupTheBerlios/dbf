/***********************************************************************************
 * csv.c
 ***********************************************************************************
 * dbf Reader and Converter for dBASE files
 * Implementation
 *
 * Author: Bjoern Berg, clergyman@gmx.de
 * Version 0.2, September 2003
 *
 ***********************************************************************************
 * This includes enable dbf to write CSV files
 ***********************************************************************************
 * History:
 * 2003-09-08	teterin, berg	complete rewrite of functions
 *								code cleanup
 * 2003-03-17	berg			first implementation
 *								copied CSV-specific functions to separate file
 ***********************************************************************************/

#include "csv.h"

static char CSVSeparator = ',';

/* allows to change the separator used for CSV output */
int
setCSVSep(FILE *fp, const struct DB_FIELD * /*const*/ header,
    int header_length, const char *in /* __unused */, const char *separator)
{
	if (separator[1]) {
		fprintf(stderr, "Separator ``%s'' is too long -- must be a single character\n",
		    separator);
		return 1;
	}
	CSVSeparator = separator[0];
	return 0;
}

/* writeCSVHeader */
/* creates the CSV Header with the information provided by DB_FIELD */
int
writeCSVHeader (FILE *fp, const struct DB_FIELD * header,
    int header_length,
    const char *in /* __unused */, const char *out /* __unused */)
{
	while (--header_length)
		fprintf(fp, "%s%c", (++header)->field_name, CSVSeparator);
	fputs("\n", fp);

	return 0;
}

/* writeCSVLine */
/* creates a line in the CSV document for each data set */
int
writeCSVLine(FILE *fp, const struct DB_FIELD * header,
    const unsigned char *value, int header_length,
    const char *in /* unused */, const char *out /* unused */)
{

	while (--header_length)
	{
		const unsigned char *begin, *end;
		int isstring = (header+1)->field_type == 'M' || (header+1)->field_type == 'C';
		
		begin = value;
		value += (++header)->field_length;
		end = value - 1;

		/*
		 * addded to keep to CSV standard:
		 * Text fields must be enclosed by quotation marks
		 * - berg, 2003-09-08
		 */
		if (isstring)
			putc('\"', fp);
		
		while (*begin == ' ' && begin != end)
			begin++;
		if (*begin != ' ') {
			while (*end == ' ')
				end--;
			do {
				putc(*begin, fp);
			} while (begin++ != end);
		}

		if (isstring)
			putc('\"', fp);

		putc(CSVSeparator, fp);
	}
	fputs("\n", fp);

	return 0;
}
