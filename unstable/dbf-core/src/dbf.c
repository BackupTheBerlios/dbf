/***********************************************************************************
 * dbf.c
 * Author: Bjoern Berg, June 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBASE files
 * Version 0.7
 *
 * History:
 * - Version 0.7 - September 2003
 *   completely rewritten version by Mikhail Teterin
 *   for a complete list of nower and former changes please have a look at CHANGELOG
 * [...]
 * - Version 0.1 - June 2002
 *	 Output for dBase3 databases, based on a version by Joachim Astel, 1989
 ************************************************************************************/

#include <assert.h>
#include "dbf.h"

static struct DB_HEADER db_buf, *db = &db_buf;
static struct DB_FIELD *header;
static int convert = 1;

int	isbigendian;
unsigned int verbosity = 1;

static int
dbf_read_header(int fh, const char *file)
{
	if ((read( fh, (char *)db, sizeof(struct DB_HEADER))) == -1 ) {
		perror(file);
		exit(1);
	}

	return 1;
}

/* getHeaderValues */
/* fills the struct DB_FIELD with field names and other values*/
static int
getHeaderValues(int fh, const char *file, int header_length)
{
	struct DB_FIELD *h;
	header = malloc(sizeof(struct DB_FIELD) * header_length);
	if (header == NULL) {
		perror("malloc");
		exit(1);
	}

	for (h = header + 1; --header_length; h++)
	{
		if((read(fh, h, sizeof(struct DB_FIELD))) == -1) {
			perror(file);
			exit(1);
		}
	}
	return 0;
}

static int
setNoConv(FILE *output, const struct DB_FIELD * header,
    int header_length,
    const char *filename, const char *level)
{
	convert = 0;
	return 0;
}

static int
setVerbosity(FILE *output, const struct DB_FIELD * header,
    int header_length,
    const char *filename, const char *level)
{
	if (level[1] != '\0' || level[0] < '0' || level[0] > '9') {
		fprintf(stderr, "Invalid debug level ``%s''. Must be from 0 to 9\n", level);
		return 1;
	}
	verbosity = level[0] - '0';
	return 0;
}

static int
writeINFOHdr(FILE *output, const struct DB_FIELD * header,
    int header_length,
    const char *filename, const char *export_filename)
{
	dbf_file_info(db);
	dbf_field_stat(header,header_length);
	return 0;
}

static int
printDBF(FILE *output, const struct DB_FIELD * header,
    const unsigned char *value, int header_length,
    const char *filename, const char *export_filename)
{
	const struct DB_FIELD *dbf;
	for (dbf = header + 1; --header_length; dbf++) {
		printf("%11.11s: %.*s\n", dbf->field_name,
		    dbf->field_length, value);
		value += dbf->field_length;
	}
	return 0;
}

struct options {
	const char	*id;
	headerMethod	 writeHeader;
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
		"sql",	writeSQLHeader,	writeSQLLine,	ARG_OUTPUT,
		"{filename} -- writes the table-creating SQL code into the file",
		"to write to stdout in human-readable form"
	},
	{
		"trim",	setSQLTrim,	writeSQLLine,	ARG_OPTION,
		"{r|l|b} -- whether to trim the string fields in the SQL output\n"
		"\t\tfrom right, left, or both",
		"not to trim"
	},
	{
		"csv",	writeCSVHeader,	writeCSVLine,	ARG_OUTPUT,
		"{filename} -- outputs the \"Comma Separated Values\" represenation\n"
		"\t\tof the dBASE table into the specified file",
		"to write to stdout in human-readable form"
	},
	{
		"separator",	setCSVSep,	NULL,		ARG_OPTION,
		"{c} -- sets the separator character for the CSV format",
		"to use ``,''"
	},
	{
		"view-info",	writeINFOHdr,	NULL,		ARG_NONE,
		"write the dBASE file's headers and stats to stdout",
		"not to output the stats"
	},
	{
		"noconv",	setNoConv,	NULL,		ARG_BOOLEAN,
		"do not run each each record through charset converters",
		"to use the experimental converters"
	},
	{
		"debug",	setVerbosity,	NULL,		ARG_OPTION,
		"{0-9} -- set the debug level. 0 is the quietest",
		"1"
	},
	{	NULL	}
};

static void
banner()
{
	fputs("dBase Reader and Converter V. 0.7, (c) 2002 - 2003 by Bjoern Berg\n"
	    /*"$Id: dbf.c,v 1.4 2003/09/08 17:02:10 rollin_hand Exp $\n" */, stderr);
/*#	if defined(__DATE__) && defined(__TIME__)
	fputs("Built on "__DATE__" at "__TIME__"\n", stderr);
#	endif */
}

/* Help */
/* Displays a well known UNIX style command line options overview */
static void
usage(const char *pname)
{
	struct options *option;
	banner();
	fprintf(stderr, "Usage:\n\t%s [options] dbf-file\n"
	    "\tOutput the contents of the dBASE table file (.dbf).\n"
	    "Available options:\n", pname);

	for (option = options; option->id; option++)
		fprintf(stderr, "%c\t--%s\t%s\n\t\t(default is %s)\n",
		    option->argument == ARG_OUTPUT || option->argument == ARG_NONE ? '*' : ' ',
		    option->id, option->help, option->def_behavior);
	fputs("*-marked options are currently mutually exclusive.\n"
	    "The last specified takes precedence.\n", stderr);
	fputs("A single dash (``-'') as a filename specifies stdin or stdout\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	int		 dbfhandle;
	FILE		*output = NULL;
	int		 header_length, record_length, i;
	const char	*filename, *export_filename = NULL;
	headerMethod	 writeHeader = NULL;
	lineMethod	 writeLine = printDBF;
	unsigned char	*record;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s [option][argument] dbf-file, -h for help\n", *argv);
		exit(1);
	}

	/* Check if someone needs help */
	for(i=1; i < argc; i++)
		if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0 || strcmp(argv[i],"/?")==0)
			usage(*argv);	/* Does not return */

	/* check architecture: little-endian/big-endian XXX Should be done at compile time! */
	isbigendian = IsBigEndian();

	/* fill filename with last argument */
	/* Test if last argument is an option or a possible valid filename */
	filename = argv[--argc];
	if (filename[0] == '-' && filename[1] != '\0') {
		fprintf(stderr, "\nERROR: Found no file for input\n"
		    "Please make sure that the last argument is a valid dBASE file\n");
		exit(1);
	}

	dbfhandle = dbf_open(filename);
	dbf_read_header(dbfhandle, filename);
	header_length = rotate2b(db->header_length) / 32;

	/*
	 * Scan through arguments looking for options
	 */
	for(i=1; i < argc; i++) {
		struct options *option = options;
		if (argv[i][0] != '-' && argv[i][1] != '-')
			goto badarg;
		while (option->id && strcmp(argv[i]+2, option->id))
			option++;
		if (option->id == NULL) {
		badarg:
			fprintf(stderr, "Unrecognized option ``%s''. "
			    "Try ``--help'' for usage\n", argv[i]);
			exit(1);
		}
		switch (option->argument) {
		case ARG_OUTPUT:
			/* There can only be one: */
			if (export_filename) {
				fprintf(stderr,
				    "Output file name was already specified as ``%s''.\n"
				    "Try the --help option\n",
				    export_filename);
				exit(1);
			}
			export_filename = argv[++i];
			/* FALLTHROUGH */
		case ARG_NONE:
			writeHeader = option->writeHeader;
			writeLine = option->writeLine;
			break;
		case ARG_OPTION:
			i++;
			/* FALLTHROUGH */
		case ARG_BOOLEAN:
			/* There can be many -- call them all: */
			if (option->writeHeader &&
			    option->writeHeader(NULL, header,
			    header_length, filename, argv[i]))
				exit (1);
			break;
		default:
			assert(!"Unknown type of option argument");
		}
	}

	if (verbosity > 0)
		banner();

	output = export_open(export_filename);
	header_length = rotate2b(db->header_length) / 32;
	record_length = rotate2b(db->record_length);
	getHeaderValues(dbfhandle,filename,header_length);

	/*
	 * Call the main header-method, which we skipped during the option parsing
	 */
	if (writeHeader && writeHeader(output, header, header_length,
	    filename, export_filename))
		exit(1);

	if (writeLine) {
		if ((record = malloc(record_length + 1)) == NULL)	{
			perror("malloc"); exit(1);
		}
		record[record_length] = '\0'; /* So the converters know, where to stop */

		if (verbosity > 0)
			fprintf(stderr, "Export from %s to %s\n",filename,
			    output == stdout ? "stdout" : export_filename);

		lseek(dbfhandle, rotate2b(db->header_length) + 1, SEEK_SET);
		while ((i = read(dbfhandle, record, record_length)))
		{
			if (i == -1) {
				perror("reading the next block");
				exit(1);
			}
			/* automaticly convert options */
		  	if (convert)
				cp850andASCIIconvert(record);
			writeLine(output, header, record, header_length,
			    filename, export_filename);
		}
		free(record);
	}
	dbf_close(dbfhandle, filename);
	export_close(output, export_filename);

	return 0;
}
