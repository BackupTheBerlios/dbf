/***********************************************************************************
 * tables.h
 ***********************************************************************************
 * describes table file header and table field descriptor array
 * for dBASE III PLUS, IV 2.0 and 5.0 for DOS/Windows
 *
 * Author: Bjoern Berg, clergyman@gmx.de 
 *
 * History:
 * $Log: tables.h,v $
 * Revision 1.6  2004/04/14 07:05:38  rollinhand
 * Added DB_FSIZE
 *
 * Revision 1.5  2004/03/16 20:57:36  rollinhand
 * Code Cleanup
 *
 * 2003-08-27	teterin,berg	changed unsigned integer values to u_int
 * 2003-01-21	berg			added support for Alpha RISC systems (Cancelling
 *								datatype long, because AXP uses 8 Byte for long,
 *								i386 4 Byte -> incompatible)
 * 2002-09-22	berg			added additional dBASE Header
 *								support for dBASE IV 2.0, 5.0 DOS/Windows introduced
 * 2002-09-20	berg			first implementation, split off dbf.c
 *
 ************************************************************************************/

#ifndef _DBF_TABLES_
#define _DBF_TABLES_

#ifdef __unix__
   #include <sys/types.h>
   #ifndef __ANUBISNET_TYPES__
   #define __ANUBISNET_TYPES__
     typedef u_int16_t uint16_t;
     typedef u_int32_t uint32_t;
   #endif
/*
 * Windows does not know UINT16 types, therefore we have to make an improvement
 * for 32 Bit systems. unsigned short is only verified to work properly on 32 Bit
 * systems.
 */
#elif _WIN32
	#include <windows.h>
    #ifndef __ANUBISNET_TYPES__
    #define __ANUBISNET_TYPES__
      typedef UINT32 u_int32_t;
      typedef unsigned short u_int16_t;
    #endif
#else
   #include <sys/types.h>
#endif


/*
 * These defines are used to distinguish between types in the
 * dbf fields.
 */
#define IS_STRING 1
#define IS_NUMERIC 2

/*
 * table file header
 * Standard dBASE Header
 * Additional dBASE Header FLAGS
 * Since version IV 2.0 dBASE has introduced new flags, like:
 * - incomplete transmission
 * - encryption
 * - mdx
 * - language
 * Offsets of this header are the same in all versions of dBASE
 */
struct DB_HEADER {
	unsigned char version;			/* Byte: 0; dBase version */
	unsigned char last_update[3];	/* Byte: 1-3; date of last update */
	//unsigned int records;			/* Byte: 4-7; number of records in table */
	u_int32_t records;				/* Byte: 4-7; number of records in table */
	u_int16_t header_length;		/* Byte: 8-9; number of bytes in the header */
	u_int16_t record_length;		/* Byte: 10-11; number of bytes in the record */
	unsigned char reserved01[2];	/* Byte: 12-13; reserved, see specification of dBase databases */
	unsigned char transaction;		/* Byte: 14; Flag indicating incomplete transaction */
	unsigned char encryption;		/* Byte: 15; Encryption Flag */
	unsigned char reserved02[12];	/* Byte: 16-27; reserved for dBASE in a multiuser environment*/
	unsigned char mdx;				/* Byte: 28; Production MDX file flag */
	unsigned char language;			/* Byte: 29; Language driver ID, for Visual FoxPro */
	unsigned char reserved03[2];	/* Byte: 30-31; reserved, filled with zero */
};

/* The field descriptor array */
/* Offsets of this header are the same in all versions of dBASE */
struct DB_FIELD {
	unsigned char field_name[11];	/* Byte: 0-10; fieldname in ASCII */
	unsigned char field_type;		/* Byte: 11; field type in ASCII (C, D, L, M or N) */
	//unsigned long field_adress;	/* Byte: 12-15; field data adress */
	u_int32_t field_adress;			/* Byte: 12-15; field data adress */
	unsigned char field_length;		/* Byte: 16; field length in binary */
	unsigned char field_decimals;	/* Byte: 17; field decimal count in binary */
	unsigned char reserved[13];		/* Byte: 18-30; reserved */
	unsigned char mdx;				/* Byte: 31; Production MDX field flag */
};

/* Memo File Structure (.FPT) */
/* Memo files contain one header record and any number of block structures. The header record contains  */
/* a pointer to the next free block and the size of the block in bytes. The size is determined by the  */
/* SET BLOCKSIZE command when the file is created. The header record starts at file position zero and  */
/* occupies 512 bytes. The SET BLOCKSIZE TO 0 command sets the block size width to 1. */

/* Following the header record are the blocks that contain a block header and the text of the memo.  */
/* The table file contains block numbers that are used to reference the memo blocks. The position of  */
/* the block in the memo file is determined by multiplying the block number by the block size (found  */
/* in the memo file header record). All memo blocks start at even block boundary addresses. A memo  */
/* block can occupy more than one consecutive block. */

/* Memo Header Record */
											/* Byte offset	Description */
struct DB_MEMO_HEADER {
	unsigned int block_adress;				/* 00  03			Location of next free block [1] */
	unsigned char reserved[2];				/* 04  05			Unused */
	unsigned short block_size;				/* 06  07			Block size (bytes per block) [1] */
	unsigned char reserved2[504];			/* 08  511			Unused */
 };

 /* [1] Integers stored with the most significant byte first. See: endian.h   */

/* Memo Block Header and Memo Text */
								/* Byte offset	Description */
struct DB_MEMO_BLOCK_TOP {
	unsigned int signature;				/* 00  03			Block signature [1]  */
								/*		(indicates the type of data in the block)
								 *		0  picture (picture field type)
								 *		1  text (memo field type) */
	unsigned int block_length;		/* 04  07			Length [1] of memo (in bytes) */
								/* 08 -  n	Memo text (n = length) */
};

/* [1] Integers stored with the most significant byte first.    */

struct DB_FSIZE {
	u_int32_t real_filesize;		/* the pysical size of the file, as stated
									   from filesystem */
	u_int32_t calc_filesize;		/* the calculated filesize */
	unsigned char integrity[6];		/* integrity could be: OK, not OK */
};


#endif
