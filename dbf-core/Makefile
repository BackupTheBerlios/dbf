# This Makefile was created by Bjoern Berg, clergyman@gmx.de
# introduced in dbf 0.3.2
# Makefile version 0.3, 17/02/2003, changed by: Bjoern Berg
# Makefile version 0.2, 17/01/2003, changed by: Bjoern Berg
# Makefile version 0.1, 02/11/2002, created by: Bjoern Berg

# Do not change the lines between START and END
# START - Configuration
# ROOTDIR has to be set to the current development version directory
CC = /usr/bin/gcc

SRC = dbf.c
HDR = iodbf.h tables.h an_string.h codepages.h statistic.h congraph.h dbfversion.h endian.h
DOC = CHANGELOG README CREDITS MANIFEST INSTALL FAQ
BUILD = Makefile BUGS 
ROOTDIR = dbf-core
BIN = dbf
VERS = 0.5


TAR = tar cfz
ZIP = zip
RM = /bin/rm -f
#END

# Change this line to install dbf anywhere you want
PREFIX = /usr/local/bin

dbf: $(SRC) $(HDR)
	@(echo "")
	@(echo "Compiling dbf:")
	$(CC) -Wall -o $(BIN) $(SRC)
	@(echo "")
	@(echo "Now install with 'make install'")
	@(echo "source will installed to: $(PREFIX)")
	@(echo "You can change PREFIX by editing the Makefile")
	@(echo "")
	@(echo ">> Type 'make install'")
	@(echo "")	

all: dbf

install:
	cp $(BIN) $(PREFIX)
	
uninstall: 
	@(echo "Uninstall dbf: "; rm -f $(PREFIX)/$(BIN))	

clean:
	@(echo "Remove built binaries: "; rm -f $(BIN))	

# Special Makefile scripts to build packages for SF and Freshmeat
# only UN*X based packages can be build
# This is only interesting for developers and contributors who want to build
# binaries for their prefered platform

dist: $(SRC) $(HDR) $(DOC) Makefile
	@ls $(SRC) $(HDR) $(DOC) $(BUILD) | sed s:^:$(BIN)-$(VERS)/: >MANIFEST
	@(echo "Using sources from $(ROOTDIR) to build package")
	@(cd ..; ln -s $(ROOTDIR) dbf-$(VERS))
	(cd ..;	$(TAR) $(ROOTDIR)/$(BIN)-$(VERS).src.tar.gz `cat $(ROOTDIR)/MANIFEST`)
	@(cd ..; rm dbf-$(VERS)) 

elf-dist: 
	$(CC) -o $(BIN) $(SRC)
	@ls $(BIN) $(DOC) | sed s:^:$(BIN)-$(VERS)/: >MANIFEST
	@(echo "Using sources from $(ROOTDIR) to build package")
	@(cd ..; ln -s $(ROOTDIR) dbf-$(VERS))
	(cd ..;	$(ZIP) $(ROOTDIR)/$(BIN)-$(VERS).bin.ELF.zip `cat $(ROOTDIR)/MANIFEST`)
	@(cd ..; rm dbf-$(VERS)) 
	@(rm -f $(BIN))

axp-dist: 
	$(CC) -o $(BIN) $(SRC)
	@ls $(BIN) $(DOC) | sed s:^:$(BIN)-$(VERS)/: >MANIFEST
	@(echo "Using sources from $(ROOTDIR) to build package")	
	@(cd ..; ln -s $(ROOTDIR) dbf-$(VERS))
	(cd ..;	$(ZIP) $(ROOTDIR)/$(BIN)-$(VERS).bin.AXP.zip `cat $(ROOTDIR)/MANIFEST`)
	@(cd ..; rm dbf-$(VERS)) 
	@(rm -f $(BIN))

ppc-dist: 
	$(CC) -o $(BIN) $(SRC)
	@ls $(BIN) $(DOC) | sed s:^:$(BIN)-$(VERS)/: >MANIFEST
	@(echo "Using sources from $(ROOTDIR) to build package")	
	@(cd ..; ln -s $(ROOTDIR) dbf-$(VERS))
	(cd ..;	$(ZIP) $(ROOTDIR)/$(BIN)-$(VERS).bin.PPC.zip `cat $(ROOTDIR)/MANIFEST`)
	@(cd ..; rm dbf-$(VERS)) 
	@(rm -f $(BIN))			
