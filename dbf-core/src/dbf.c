/******************************************************************************
 * dbf.c
 ******************************************************************************
 * Author: Bjoern Berg <clergyman@gmx.de>
 * dbf Reader and Converter for dBASE files
 * Version 0.9
 *
 ******************************************************************************
 * $Id: dbf.c,v 1.28 2004/08/30 12:03:00 steinm Exp $
 *****************************************************************************/

 /** TODO **/
 /* Currently we do not know how to handle field subrecords in FoxPro and dBASE 4
  * backlink_exists() works only if no field subrecords are in the table definition (*.dbf)
  */

#include <assert.h>
#include "dbf.h"

static struct DB_FSIZE *fsz;

static int convert = 1;
static int keep_deleted = 0;
static int dbc = 0;   /* 0 = no backlink, 1 = backlink */

unsigned int verbosity = 1;
char *tablename = NULL;


/* banner() {{{
 */
static void
banner()
{
	fprintf(stderr, PACKAGE_NAME " " VERSION);
	fprintf(stderr, "\n");
	fprintf(stderr, _("Copyright 2002-2004 Bjoern Berg"));
	fprintf(stderr, "\n");
}
/* }}} */

/* version() {{{
 */
static void
version()
{
	fprintf(stderr, VERSION);
	fprintf(stderr, "\n");
}
/* }}} */

/* export_open() {{{
 * open the export file for writing */
FILE *
export_open(const char *file)
{
	FILE *result;
	if (file == NULL || (file[0] == '-' && file[1] == '\0'))
		return stdout;
	if ((result = fopen(file, "w")) == NULL) {
		perror(file);
		exit(1);
	}
	return result;
}
/* }}} */

/* export_close() {{{
 * closes the opened file and stops the write-process */
int
export_close(FILE *fp, const char *file)
{
	if (fp == stdout)
		return 0;
	if (fclose(fp)) {
		fprintf(stderr, _("Cannot close output file '%s'."), file);
		fprintf(stderr, "\n");
		perror("Error");
		return 1;
	}
	if (verbosity > 2) {
		fprintf(stderr, _("Output file '%s' was closed successfully."), file);
		fprintf(stderr, "\n");
	}
	return 0;
}
/* }}} */

/* dbf_backlink_exists() {{{
 * checks if a backlink is at the end of the field definition, this backlink is
 * about 263 byte
 */
#ifdef kkk
static int dbf_backlink_exists(int fh, const char *file)
{
	int number_of_fields;
	long pos;
	char terminator[1], *pt=terminator;

	/* As stated in Microsofts TechNet Article about Visual FoxPro, we have to
	 * proof if the Field Terminator is set, because dbf puts the backlink to
	 * the normal header. The sign 0x0Dh is invalid in the backlink so we can
	 * easily prove for it and verify Microsofts formula.
	 * -- berg, 2003-12-08
	 */
	pos = rotate4b( lseek(fh, 0L, SEEK_CUR) );	
	lseek(fh, db->header_length-1, SEEK_SET);

	if ((read( fh, (char *)pt, 1)) == -1 ) {
		perror(file);
		exit(1);
	}

	lseek(fh, pos, SEEK_SET);

	number_of_fields = (db->header_length - 296) / 32;

	if ( (number_of_fields != db->records) && *pt != 0x0D) {
		fprintf(stderr, "Database backlink found!");
		fprintf(stderr, "\n");
		return 1;
	}

	return 0;
}
#endif
/* }}} */

/* dbf_check {{{
 * checks if dbf file is valid
 * File size reported by the operating system must match the logical file size.
 * Logical file size = ( Length of header + ( Number of records * Length of each record ) )
 * Exception: Clipper and Visual FoxPro
 */
#ifdef kkkk
static int
dbf_check(int fh, const char *file)
{
	//u_int32_t filesize, pos, calc_filesize;
	u_int32_t pos;

	pos = rotate4b( lseek(fh, 0L, SEEK_CUR) );
	fsz->real_filesize = rotate4b( lseek(fh, 0L, SEEK_END) );
	lseek(fh, pos, SEEK_SET);

	fsz->calc_filesize = (db->header_length + (db->records * db->record_length) + 1) ;

	if ( fsz->calc_filesize != fsz->real_filesize ) {
		strcpy(fsz->integrity,"not OK");
		return 0;
	}

	strcpy(fsz->integrity,"OK");
	return 1;
}
#endif
/* }}} */

/* setTablename() {{{
 * set the name of the table for sql output
 */
static int
setTablename(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *_tablename)
{
	tablename = strdup(_tablename);
	return 0;
}
/* }}} */

/* setNoConv() {{{
 * defines if charset converter should be used
 */
static int
setNoConv(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	convert = 0;
	return 0;
}
/* }}} */

/* setKeepDel() {{{
 */
static int
setKeepDel (FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	keep_deleted = 1;
	return 0;
}
/* }}} */

/* setVerbosity() {{{
 * sets debug level
 */
static int
setVerbosity(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	if (level[1] != '\0' || level[0] < '0' || level[0] > '9') {
		fprintf(stderr, _("Invalid debug level ``%s''. Must be from 0 to 9"), level);
		fprintf(stderr, "\n");
		return 1;
	}
	verbosity = level[0] - '0';
	return 0;
}
/* }}} */

/* writeINFOHdr() {{{
 */
static int
writeINFOHdr(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *export_filename)
{
	dbf_file_info(p_dbf);
	dbf_field_stat(p_dbf);
	return 0;
}
/* }}} */

/* printDBF() {{{
 * printDBF is the real function that is hidden behind writeLine
 */
static int
printDBF(FILE *output, P_DBF *p_dbf,
    const unsigned char *record, int header_length,
    const char *filename, const char *export_filename)
{
	int i, columns;
	const char *value;
	columns = dbf_NumCols(p_dbf);
	value = record;

	for (i = 0; i < columns; i++) {
		char field_type;
		const char *field_name;
		int field_length, field_decimals;
		field_type = dbf_ColumnType(p_dbf, i);
		field_name = dbf_ColumnName(p_dbf, i);
		field_length = dbf_ColumnSize(p_dbf, i);
		field_decimals = dbf_ColumnDecimals(p_dbf, i);
		printf("%11.11s: %.*s\n", field_name, field_length, value);
		value += field_length;
	}
	return 0;
}

/*}}} */

/* Options {{{
 * Added the hyphes to the id so that ids with a single hyphe are also possible.
 * -- Bjoern Berg, 2003-10-06
 */
struct options {
	const char	*id;
	headerMethod	 writeHeader;
	footerMethod	 writeFooter;
	lineMethod	 writeLine;
	enum argument {
		ARG_NONE,	/* Method without output file */
		ARG_OPTION,	/* Option with argument	*/
		ARG_BOOLEAN,	/* Option without argument */
		ARG_OUTPUT	/* Method with output file */
	} argument;
	const char	*help, *def_behavior;
} options[] = {
	{
		"--sql", writeSQLHeader,	writeSQLFooter, writeSQLLine,	ARG_OUTPUT,
		"{filename} -- convert file into sql statements",
		NULL
	},
	{
		"--trim", setSQLTrim,	NULL, NULL,	ARG_OPTION,
		"{r|l|b} -- trim char fields in sql output (right, left, both)",
		"not to trim"
	},
	{
		"--csv", writeCSVHeader,	NULL, writeCSVLine,	ARG_OUTPUT,
		"{filename} -- convert file into \"comma separated values\"",
		NULL
	},
	{
		"--separator", setCSVSep, NULL, NULL, ARG_OPTION,
		"{c} -- set field separator for csv format",
		"to use ``,''"
	},	
	{
		"--tablename", setTablename, NULL, NULL, ARG_OPTION,
		"{name} -- set name of the table for sql output",
		"the name of the export file"
	},	
	{
		"--view-info", writeINFOHdr, NULL, NULL, ARG_NONE,
		"write various information and table structure to stdout",
		NULL
	},
	{
		"--noconv",	setNoConv, NULL, NULL, ARG_BOOLEAN,
		"do not run each record through charset converters",
		"to use the experimental converters"
	},
	{
		"--nodrop",	setNoDrop, NULL, NULL, ARG_BOOLEAN,
		"disable DROP TABLE statement in sql output",
		NULL
	},
	{
		"--nocreate", setNoCreate, NULL, NULL, ARG_BOOLEAN,
		"disable CREATE TABLE statement in sql output",
		NULL
	},
	{
		"--usecopy", setSQLUsecopy, NULL, NULL, ARG_BOOLEAN,
		"use COPY instead of INSERT for populating table",
		NULL
	},
	{
		"--empty-str-is-null", setSQLEmptyStrIsNULL, NULL, NULL, ARG_BOOLEAN,
		"ouput NULL for empty strings in sql output",
		NULL
	},
	{
		"--keepdel", setKeepDel, NULL, NULL, ARG_NONE,
		"output also deleted records",
		"to skip deleted records"
	},
	{
		"--debug", setVerbosity, NULL, NULL, ARG_OPTION,
		"{0-9} -- set the debug level. 0 is the quietest",
		"0"
	},
	{
		"--version", NULL, NULL, NULL, ARG_NONE,
		"shows the current release number",
		NULL
	},
	{	NULL	}
};
/* }}} */

/* usage() {{{
 * Displays a well known UNIX style command line options overview
 */
static void
usage(const char *pname)
{
	struct options *option;
	banner();
	fprintf(stderr, _("Usage: %s [options] dbf-file"), pname);
	fprintf(stderr, "\n");
	fprintf(stderr, _("Output the contents of a dBASE table file (.dbf)."));
	fprintf(stderr, "\n\n");
	fprintf(stderr, _("Available options:"));
	fprintf(stderr, "\n");

	for (option = options; option->id; option++) {
		fprintf(stderr, "  %-11s %s\n",
	    	option->id, option->help);
		if (option->def_behavior != NULL)
			fprintf(stderr, "  %-11s (default is %s)\n", "", option->def_behavior);
	}
	fprintf(stderr, "\n");
	fprintf(stderr, _("The options --sql, --csv, --view-info, --version are mutually exclusive."));
	fprintf(stderr, "\n");
	fprintf(stderr, _("The last option specified takes precedence."));
	fprintf(stderr, "\n");
	fprintf(stderr, _("A single dash (``-'') as a filename specifies stdin or stdout"));
	fprintf(stderr, "\n");
	exit(1);
}
/* }}} */

/* main() {{{
 */
int
main(int argc, char *argv[])
{
	P_DBF *p_dbf;
	FILE		*output = NULL;
	int		 header_length, record_length, i;
	const char	*filename, *export_filename = NULL;
	headerMethod	 writeHeader = NULL;
	footerMethod	 writeFooter = NULL;
	lineMethod	 writeLine = printDBF;
	unsigned char	*record;
	unsigned int dataset_deleted;

#ifdef ENABLE_NLS
	setlocale (LC_ALL, "");
	setlocale (LC_NUMERIC, "C");
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	textdomain (GETTEXT_PACKAGE);
#endif

	if (argc < 2) {
		usage(PACKAGE_NAME);	/* Does not return */
		exit(1);
	}

	/* Check if someone needs help */
	for(i=1; i < argc; i++)
		if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0 || strcmp(argv[i],"/?")==0)
			usage(PACKAGE_NAME);	/* Does not return */

	/* Check if someone wants only version output */
	for(i=1; i < argc; i++) {
		if( strcmp(argv[i],"-v")==0 || strcmp(argv[i],"--version")==0 ) {
			banner();
			exit(1);
		}
	}

	/* fill filename with last argument
	 * Test if last argument is an option or a possible valid filename
	 */
	filename = argv[--argc];
	if (filename[0] == '-' && filename[1] != '\0') {
		fprintf(stderr, _("No input file specified. Please make sure that the last argument is a valid dBASE file."));
		fprintf(stderr, "\n");
		exit(1);
	}

	/* Open the input dBASE file */
	if(NULL == (p_dbf = dbf_Open(filename))) {
		fprintf(stderr, _("Could not open dBASE file '%s'."), filename);
		fprintf(stderr, "\n");
		exit(1);
	}

	/* Scan through arguments looking for options
	 */
	for(i=1; i < argc; i++) {
		struct options *option = options;
		if (argv[i][0] != '-' && argv[i][1] != '-')
			goto badarg;
		while (option->id && strcmp(argv[i], option->id))
			option++;
		if (option->id == NULL) {
		badarg:
			fprintf(stderr, _("Unrecognized option ``%s''. Try ``--help'' for a list of options."), argv[i]);
			fprintf(stderr, "\n");
			exit(1);
		}
		switch (option->argument) {
			case ARG_OUTPUT:
				if (export_filename) {
					fprintf(stderr,
						_("Output file name was already specified as ``%s''. Try the --help for a list of options."), export_filename);
					fprintf(stderr, "\n");
					exit(1);
				}
				export_filename = argv[++i];
				/* Fail safe routine to keep sure that the original file can
				 * never be overwritten
				 */
				if ( strcmp(export_filename, filename) == 0 ) {
					fprintf(stderr, _("Input file name is equal to output file name. Please choose a different output file name."));
					fprintf(stderr, "\n");
					exit(1);
				}
				/* FALLTHROUGH */
			case ARG_NONE:
				writeHeader = option->writeHeader;
				writeFooter = option->writeFooter;
				writeLine = option->writeLine;
				break;
			case ARG_OPTION:
				i++;
				/* FALLTHROUGH */
			case ARG_BOOLEAN:
				/* There can be many -- call them all: */
				if (option->writeHeader &&
					option->writeHeader(NULL, p_dbf,
					filename, argv[i]))
					exit (1);
				break;
			default:
				assert(!"Unknown type of option argument");
		}
	}

	if (verbosity > 0)
		banner();

	if(!export_filename || 0 == strcmp(export_filename, "-"))
		output = stdout;
	else
		output = export_open(export_filename);

	/* If the tablename was not set explicitly, use the export file name */
	if(!tablename && export_filename && 0 != strcmp(export_filename, "-")) {
		char *ptr;
		tablename = basename(export_filename);
		if(NULL != (ptr = strrchr(tablename, '.')))
			*ptr = '\0';
	}
		

	if(!tablename && writeHeader == writeSQLHeader) {
		fprintf(stderr, _("SQL mode requires a tablename to be set, if the output goes to stdout."));
		fprintf(stderr, "\n");
		exit(1);
	}

	/*
	 * Call the main header-method, which we skipped during the option parsing
	 */
	if (writeHeader && writeHeader(output, p_dbf, filename, export_filename))
		exit(1);

	record_length = dbf_RecordLength(p_dbf);
	if (writeLine) {
		if ((record = malloc(record_length + 1)) == NULL)	{
			perror("malloc"); exit(1);
		}
		record[record_length] = '\0'; /* So the converters know, where to stop */

		if (verbosity > 0) {
			fprintf(stderr, _("Export from %s to %s"),filename,
			    output == stdout ? "stdout" : export_filename);
			fprintf(stderr, "\n");
		}

		while (0 <= (i = dbf_ReadRecord(p_dbf, record, record_length)))
		{
			dataset_deleted = 0;
			if (record[0] == '*' ) {
				dataset_deleted = 1;
			}

			/* Look if the dataset is deleted or the end of the dBASE file was
			 * reached without notification by read. The end of each dBASE file is
			 * marked with a dot.
			 */
			if ( (!dataset_deleted || keep_deleted == 1) && record[0] != 0x1A) {
				/* automatically convert options */
				if (convert)
					cp850andASCIIconvert(record);
				writeLine(output, p_dbf, record+1, header_length,
			    	filename, export_filename);
			} else if ( verbosity >=1 && record[0] != 0x1A) {
				fprintf(stderr, _("The row %i is set to 'deleted'."), i);
				fprintf(stderr, "\n");
			}
		}
		free(record);
	}
	/*
	 * Call the main footer-method, which we skipped during the option parsing
	 */
	if (writeFooter && writeFooter(output, p_dbf, filename, export_filename))
		exit(1);

	dbf_Close(p_dbf);
	export_close(output, export_filename);

	return 0;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
