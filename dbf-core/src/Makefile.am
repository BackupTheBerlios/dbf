## Process this file with automake to produce Makefile.in

INCLUDES = $(DBFLIB_INCLUDEDIR) -DPACKAGE_LOCALE_DIR=\""$(datadir)/locale"\"

bin_PROGRAMS = dbf

dbf_SOURCES = dbf.c csv.c sql.c codepages.c statistic.c congraph.c

dbf_LDADD = $(DBFLIB_LIBDIR) $(DBFLIB_LIBS)

