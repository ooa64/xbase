
INCLUDES= -I$(topdir) $(all_includes)
LDADD	= -L$(topdir)/xbase/.libs -lxbase

# for shadow passwords override the value
INSTALL_PROGRAM = @INSTALL@

noinst_PROGRAMS = sample1 sample2 sample3 sample4 sample5 \
               exfilter
	       
exfilter_SOURCES = exfilter.cpp
sample1_SOURCES  = sample1.cpp
sample2_SOURCES  = sample2.cpp
sample3_SOURCES  = sample3.cpp
sample4_SOURCES  = sample4.cpp
sample5_SOURCES  = sample5.cpp

noinst_HEADERS =

EXTRA_DIST = \
	makefile.g95 \
	examples.ide

MAINTAINERCLEANFILES = Makefile.in
