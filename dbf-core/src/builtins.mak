#
# anubisnet Open Source Development Group
# This file is used by Borlands C-Compiler	
#

CC       = bcc32
RC       = brcc32
AS       = tasm32

INCDIR	= C:\Borland\BCC55\include
LIBDIR	= C:\Borland\BCC55\lib
OPTIONS = -G -N
BIN		= dbf

OBJ = dbf.obj an_string.obj codepages.obj congraph.obj csv.obj endian.obj iodbf.obj sql.obj statistic.obj

.c.obj:
      $(CC) -I$(INCDIR) -L$(LIBDIR) $(CFLAGS) /c $&.c
