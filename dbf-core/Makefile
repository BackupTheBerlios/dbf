# Makefile for dbf
# Makefile Version: 0.6

include Makefile.rules

all: 
	@(echo "");
	@(echo -en "Compiling dbf $(VERSION)... ");
	@(for i in $(DIRS); do make -C $$i > $(LOG) 2>&1; done);		
	@(echo -e "$(GREEN)   done$(NORMAL)");
	@(echo "");
	@(echo "Check make.log in the main-directory of dbf if dbf compiled fine.");
	@(echo "");
	@(echo -e "Now execute 'make install'")
	@(echo -e "Binary will be installed to $(PREFIX)")
	@(echo "");
	
install:
	@(echo "");
	@(echo -en "Copying dbf to $(PREFIX)... ");
	cp $(DIRS)/$(BIN) $(PREFIX)
	
uninstall:
	@(echo "");
	@(echo -e "$(RED)Uninstalling dbf...$(NORMAL)");
	rm $(PREFIX)/$(BIN)		
	
clean:	
	@(echo "");
	@(echo -e "$(RED)Cleaning directories...$(NORMAL)");
	@(for i in $(DIRS); do cd $(DIRS); rm -f *.o; rm -f $(BIN); done);
	
src-dist:	
	# Add sources
	@( ls src/*.h src/*.c | sed s:^:$(BIN)-$(VERSION)/: > MANIFEST )
	# Add build instructions and Makefiles
	@( ls src/Makefile src/dbf.mak Makefile Makefile.rules | sed s:^:$(BIN)-$(VERSION)/: >> MANIFEST )
	@( ls $(DOC) | sed s:^:$(BIN)-$(VERSION)/: >> MANIFEST )
	# Create virtual packet building environment
	@( cd ..; ln -s $(PWD) $(BIN)-$(VERSION) )
	@( cd ..; tar cfz $(BIN)-$(VERSION)/$(BIN)-$(VERSION).src.tar.gz `cat $(BIN)-$(VERSION)/MANIFEST` )
	# Delete virtual packet building environment
	@( cd ..; rm $(BIN)-$(VERSION) )
	
elf-dist:
	# Compile
	@(echo -en "Compiling dbf $(VERSION)... ");
	@(for i in $(DIRS); do make -C $$i > $(LOG) 2>&1; done);		
	@(echo -e "$(GREEN)   done$(NORMAL)");	
	# Add build instructions and binary	
	@( ls src/$(BIN) src/Makefile Makefile Makefile.rules | sed s:^:$(BIN)-$(VERSION)/: > MANIFEST )
	@( ls $(DOC) | sed s:^:$(BIN)-$(VERSION)/: >> MANIFEST )
	# Create virtual packet building environment
	@( cd ..; ln -s $(PWD) $(BIN)-$(VERSION) )
	@( cd ..; zip $(BIN)-$(VERSION)/$(BIN)-$(VERSION).bin.x86.zip `cat $(BIN)-$(VERSION)/MANIFEST` )
	# Delete virtual packet building environment
	@( cd ..; rm $(BIN)-$(VERSION) )
	@( rm -f src/$(BIN) )	
	
ppc-dist:
	# Compile
	@(echo -en "Compiling dbf $(VERSION)... ");
	@(for i in $(DIRS); do make -C $$i > $(LOG) 2>&1; done);		
	@(echo -e "$(GREEN)   done$(NORMAL)");	
	# Add build instructions and binary	
	@( ls src/$(BIN) src/Makefile Makefile Makefile.rules | sed s:^:$(BIN)-$(VERSION)/: > MANIFEST )
	@( ls $(DOC) | sed s:^:$(BIN)-$(VERSION)/: >> MANIFEST )
	# Create virtual packet building environment
	@( cd ..; ln -s $(PWD) $(BIN)-$(VERSION) )
	@( cd ..; zip $(BIN)-$(VERSION)/$(BIN)-$(VERSION).bin.PPC.zip `cat $(BIN)-$(VERSION)/MANIFEST` )
	# Delete virtual packet building environment
	@( cd ..; rm $(BIN)-$(VERSION) )
	@( rm -f src/$(BIN) )	
	
axp-dist:
	# Compile
	@(echo -en "Compiling dbf $(VERSION)... ");
	@(for i in $(DIRS); do make -C $$i > $(LOG) 2>&1; done);		
	@(echo -e "$(GREEN)   done$(NORMAL)");	
	# Add build instructions and binary	
	@( ls src/$(BIN) src/Makefile Makefile Makefile.rules | sed s:^:$(BIN)-$(VERSION)/: > MANIFEST )
	@( ls $(DOC) | sed s:^:$(BIN)-$(VERSION)/: >> MANIFEST )
	# Create virtual packet building environment
	@( cd ..; ln -s $(PWD) $(BIN)-$(VERSION) )
	@( cd ..; zip $(BIN)-$(VERSION)/$(BIN)-$(VERSION).bin.AXP.zip `cat $(BIN)-$(VERSION)/MANIFEST` )
	# Delete virtual packet building environment
	@( cd ..; rm $(BIN)-$(VERSION) )
	@( rm -f src/$(BIN) )
	
solaris-dist:
	# Compile
	@(echo -en "Compiling dbf $(VERSION)... ");
	@(for i in $(DIRS); do make -C $$i > $(LOG) 2>&1; done);		
	@(echo -e "$(GREEN)   done$(NORMAL)");	
	# Add build instructions and binary	
	@( ls src/$(BIN) src/Makefile Makefile Makefile.rules | sed s:^:$(BIN)-$(VERSION)/: > MANIFEST )
	@( ls $(DOC) | sed s:^:$(BIN)-$(VERSION)/: >> MANIFEST )
	# Create virtual packet building environment
	@( cd ..; ln -s $(PWD) $(BIN)-$(VERSION) )
	@( cd ..; zip $(BIN)-$(VERSION)/$(BIN)-$(VERSION).bin.solaris.zip `cat $(BIN)-$(VERSION)/MANIFEST` )
	# Delete virtual packet building environment
	@( cd ..; rm $(BIN)-$(VERSION) )
	@( rm -f src/$(BIN) )						
	
