/*****************************************************************************
 * dbf.h
 *****************************************************************************
 * Author: Bjoern Berg, June 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBASE files
 * 
 * History:
 * $Log: dbf.h,v $
 * Revision 1.12  2004/08/30 10:24:25  steinm
 * - removed include file an_string.h
 *
 * Revision 1.11  2004/08/28 16:30:50  steinm
 * - remove some global variables, add tablename as new one
 *
 * Revision 1.10  2004/08/27 06:43:35  steinm
 * - started translation of strings
 * - removed a lot of old code
 *
 * Revision 1.9  2004/08/27 05:44:11  steinm
 * - used libdbf for reading the dbf file
 *
 * Revision 1.8  2004/04/25 16:00:52  rollinhand
 * added dbf_open / dbf_close
 *
 * Revision 1.7  2004/04/25 15:20:02  rollinhand
 * several changes - not yet verified
 *
 * Revision 1.6  2004/03/16 20:57:36  rollinhand
 * Code Cleanup
 *
 *****************************************************************************/

#ifndef __DBF_H__
#define __DBF_H__

#include "config.h"
#include <libdbf/libdbf.h>

#ifdef ENABLE_NLS
#include <libintl.h>
#define _(a) dgettext(GETTEXT_PACKAGE, a)
#else
#define _(a) a
#endif

/*
 * special anubisnet and dbf includes
 */
#include "codepages.h"
#include "statistic.h"
#include "endian.h"
#include "csv.h"
#include "sql.h"

#define DBF_FILE_CHECK 1

/*
 * These defines are used to distinguish between types in the
 * dbf fields.
 */
#define IS_STRING 1
#define IS_NUMERIC 2

#ifndef O_BINARY
#define O_BINARY 0
#endif


/*
 * V A R I A B L E S
 */

extern unsigned int verbosity;
extern char *tablename;

/*
 *	FUNCTIONS
 */

typedef int	(*headerMethod)(FILE *output, P_DBF * p_dbf,
    const char *filename, const char *export_filename);

typedef int	(*lineMethod)(FILE *output, P_DBF * p_dbf,
    const unsigned char *value, int header_length,
    const char *filename, const char *export_filename);

#endif
