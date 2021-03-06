/***********************************************************************************
 * dbf.h
 ***********************************************************************************
 * Author: Bjoern Berg, June 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBASE files
 * Version 0.6
 ***********************************************************************************/

#ifndef __DBF_CORE__
#define __DBF_CORE__

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* 
 * special anubisnet and dbf includes 
 */
#include "codepages.h"
#include "iodbf.h"
#include "an_string.h"
#include "statistic.h"
#include "endian.h"
#include "csv.h"
#include "sql.h"

/* 
 * V A R I A B L E S 
 */

extern	unsigned int verbosity;

typedef int	(*headerMethod)(FILE *output, const struct DB_FIELD * header,
    int header_length,
    const char *filename, const char *export_filename);

typedef int	(*lineMethod)(FILE *output, const struct DB_FIELD * header,
    const unsigned char *value, int header_length,
    const char *filename, const char *export_filename);

#endif
