
AM_CPPFLAGS = -I$(topdir)
LDADD	= -L$(topdir)/xbase -lxbase

INSTALL_PROGRAM = @INSTALL@

noinst_PROGRAMS = indextst exptest locktest testdate sizeof testhtml.cgi

indextst_SOURCES  = indextst.cpp
exptest_SOURCES   = exptest.cpp
locktest_SOURCES  = locktest.cpp
testdate_SOURCES = testdate.cpp
sizeof_SOURCES = sizeof.cpp
testhtml_cgi_SOURCES = testhtml.cpp

noinst_HEADERS =

EXTRA_DIST = \
	makefile.g95 \
	libtest.ide \
	testhtml.html \
	README

MAINTAINERCLEANFILES = Makefile.in
