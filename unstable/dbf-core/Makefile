# Makefile for dbf
# Version: 0.6

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
	@(for i in $(DIRS); do cd $(DIRS); rm *.o; rm $(BIN); done);
