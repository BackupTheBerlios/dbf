MANS=dbf.1

all: ${MANS}

%.1: %.sgml
	docbook-to-man $< > $@

distclean: clean

clean:
	rm -f ${MANS}

