
INCLUDES= -I$(topdir) $(all_includes)
LDADD	= -L$(topdir)/xbase/.libs -lxbase

INSTALL_PROGRAM = @INSTALL@

noinst_PROGRAMS = indextst exptest locktest testdate testhtml.cgi

indextst_SOURCES  = indextst.cpp
exptest_SOURCES   = exptest.cpp
locktest_SOURCES  = locktest.cpp
testdate_SOURCES = testdate.cpp
testhtml_cgi_SOURCES = testhtml.cpp

noinst_HEADERS =

EXTRA_DIST = \
	makefile.g95 \
	libtest.ide \
	testhtml.html \
	README

MAINTAINERCLEANFILES = Makefile.in
