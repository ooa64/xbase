
INCLUDES= -I$(topdir) $(all_includes)
LDADD	= -L$(topdir)/xbase/.libs -lxbase

INSTALL_PROGRAM = @INSTALL@

noinst_PROGRAMS = indextst exptest locktest

indextst_SOURCES  = indextst.cpp
exptest_SOURCES   = exptest.cpp
locktest_SOURCES  = locktest.cpp

noinst_HEADERS =

MAINTAINERCLEANFILES = Makefile.in
