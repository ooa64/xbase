
INCLUDES = -I$(topdir) $(all_includes)

bin_PROGRAMS = ybase
INSTALL_PROGRAM = @INSTALL@

ybase_LDADD = -L$(topdir)/xbase/.libs -lxbase

ybase_SOURCES = funct.cpp funct.h lang.cpp lang.h lexic.cpp parser.c \
	parser.h table.cpp table.h trecom.cpp trecom.h treexp.cpp treexp.h \
	types.cpp types.h ybase.cpp

parser.o: parser.c
	$(CXXCOMPILE) -x c++ -c parser.c

MAINTAINERCLEANFILES = Makefile.in
