# Makefile for dbf, src directory
# Version: 0.6

include ../Makefile.rules


OBJ = an_string.o codepages.o congraph.o csv.o dbf.o dbfversion.o endian.o iodbf.o sql.o statistic.o 


dbf: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)	

all: dbf

clean:
	rm *.o
