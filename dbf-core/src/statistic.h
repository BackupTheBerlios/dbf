/***********************************************************************************
 * statistic.h
 * inherits the statistic functions for dBASE files
 * Author: Bjoern Berg, September 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBase III, IV, 5.0
 * Version 0.3.1
 *
 * History:
 * - Version 0.3.1 - 2003-02-16
 *   berg: Fixing declaration of columns, has shown one line more than exists
 *	 jones: removing compiler errors
 *   todo: checking if this influences conversion
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

/* How can we resolve the dependencies below ? */

#ifndef _DBF_STATS_
#define _DBF_STATS_
#include "tables.h"
#include "congraph.h"
#include "dbfversion.h"
#include "endian.h"

#define DBF_VERSION 0x07
#define DBF_MEMO3 0x80
#define DBF_MEMO4 0x08

void dbf_file_info (const struct DB_HEADER *db);
void dbf_field_stat (const struct DB_FIELD *header, int header_length);

#endif
