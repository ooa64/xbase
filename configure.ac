# $Id: configure.in,v 1.20 2003/08/22 14:27:20 gkunkel Exp $
# Created by Denis Pershin <dyp@inetlab.com>
#
# 8/10/03  Recreated by Gary Kunkel <gkunkel@zhsac.com> 
#          for  Redhat 8.0, autoheader 2.53, autoconf  2.53, automake  1.63
#          simplified, removed unused and obsolete macros, etc
# 
#          All configuration parms moved to this file - if you make updates
#          please add meaningful comments

AC_INIT([xbase],[2.1.2],[xdb-devel@lists.sourceforge.net]) 
AC_CONFIG_MACRO_DIRS([m4])

# set variable XSUBVARS with a list of sub directories to process
# XSUBDIRS="xbase examples bin html libtest tv ybase"

XSUBDIRS="xbase examples bin html libtest tv"
AC_SUBST(XSUBDIRS)

# look for AIX and MINIX environments
AC_AIX
AC_MINIX


# build the following programs
AC_CONFIG_SRCDIR( examples/exfilter.cpp,
                  examples/sample1.cpp,
		  examples/sample2.cpp,
		  examples/sample3.cpp,
		  examples/sample4.cpp,
		  examples/sample5.cpp,
		  bin/checkndx.cpp,
		  bin/copydbf.cpp,
		  bin/dbfxtrct.cpp,
		  bin/dbfutil1.cpp,
		  bin/deletall.cpp,
		  bin/dumpdbt.cpp,
		  bin/dumphdr.cpp,
		  bin/dumprecs.cpp,
		  bin/packdbf.cpp,
		  bin/reindex.cpp,
		  bin/undelall.cpp,
		  bin/zap.cpp,
		  libtest/indextst.cpp,
		  libtest/exptest.cpp,
		  libtest/locktest.cpp,
		  libtest/testdate.cpp,
		  libtest/testhtml.cpp,
		  ybase/ybase.cpp
		 )
		  
# init automake
AM_INIT_AUTOMAKE

# create this header file
AM_CONFIG_HEADER(xbase/xbconfig.h)

#create a library
AM_PROG_LIBTOOL

# C++ Code
AC_PROG_CXX

# look for doxygen software
AC_CHECK_PROG(doxygen, doxygen, yes, no)
if test "$doxygen" = "yes"; then
  XSUBDIRS="$XSUBDIRS docs"
fi

# get RedHat release
if test -f /etc/redhat-release ; then
  RHREL=`cut -d " " -f 5 /etc/redhat-release`
  RHREL="rh${RHREL}"
else
  RHREL="X"
fi
AC_SUBST(RHREL)

# get G++ version
if test "$GXX" = "yes"; then
  GXXVER=`${CXX} -v 2>&1 | grep "gcc version" | cut -d " " -f 3 -`
  GXXVER="gcc${GXXVER}"
else
  GXXVER=""
fi
AC_SUBST(GXXVER)

RELEASE="${RHREL}-${GXXVER}-2.1"
AC_SUBST(RELEASE)


# setup topdir
topdir=`pwd`
AC_SUBST(topdir)


# Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADER(io.h)
AC_CHECK_HEADER(sys/locking.h)
AC_CHECK_HEADER(fcntl.h,
  [AC_DEFINE(HAVE_FCNTL_H,1,[Define to 1 if you have the <fcntl.h> header file.])])
AC_CHECK_HEADER(ctype.h)

# Checks for library functions. 
AC_CHECK_FUNCS(fcntl)
AC_CHECK_FUNCS(vsnprintf)
AC_CHECK_FUNCS(vsprintf)


# compile support for ndx index logic
AC_ARG_WITH(index-ndx, 
    [  --without-index-ndx     do not compile .ndx index support ], 
    enable_index_ndx="$withval", enable_index_ndx="yes")
if test "$enable_index_ndx" = "yes"; then
  AC_DEFINE(XB_INDEX_NDX, 1, XB_INDEX_NDX)
  AC_DEFINE(XB_EXPRESSIONS, 1, XB_EXPRESSIONS)
  AC_DEFINE(XB_INDEX_ANY, 1, XB_INDEX_ANY)
fi

# compile support for ntx index logic
AC_ARG_WITH(index-ntx, 
    [  --without-index-ntx     do not compile .ntx index support ], 
    enable_index_ntx="$withval", enable_index_ntx="yes")
if test "$enable_index_ntx" = "yes"; then
  AC_DEFINE(XB_INDEX_NTX, 1, XB_INDEX_NTX)
  AC_DEFINE(XB_EXPRESSIONS, 1, XB_EXPRESSIONS)
  AC_DEFINE(XB_INDEX_ANY, 1, XB_INDEX_ANY)
fi

# compile support for memo fields
AC_ARG_WITH(memo-fields, 
    [  --without-memo-fields   do not compile memo fields support ], 
    enable_memo_fields="$withval", enable_memo_fields="yes")
if test "$enable_memo_fields" = "yes"; then
  AC_DEFINE(XB_MEMO_FIELDS, 1, XB_MEMO_FIELDS)
fi

# compile support for expressions
# index logic requires this be enabled
AC_ARG_WITH(expressions, 
    [  --without-expressions   do not compile expressions support ], 
    enable_expressions="$withval", enable_expressions="yes")
if test "$enable_expressions" = "yes"; then
  AC_DEFINE(XB_EXPRESSIONS, 1, XB_EXPRESSIONS)
fi
    
# compile support for html generation
AC_ARG_WITH(ui-html, 
    [  --without-ui-html       do not compile HTML support ], 
    enable_ui_html="$withval", enable_ui_html="yes")
if test "$enable_ui_html" = "yes"; then
  AC_DEFINE(XB_HTML, 1, XB_HTML)
fi

# compile support for record locking
AC_ARG_WITH(xbase-locking, 
    [  --without-xbase-locking turn off XBase file locking ], 
    enable_xbase_locking="$withval", enable_xbase_locking="yes")
if test "$enable_xbase_locking" = "yes"; then
  AC_DEFINE(XB_LOCKING_ON, 1, XB_LOCKING_ON)
fi
    
# compile support for true record deletion
AC_ARG_WITH(realdelete, 
    [  --without-realdelete     turn off XBase record deletion ], 
    enable_realdelete="$withval", enable_realdelete="yes")
if test "$enable_realdelete" = "yes"; then
  AC_DEFINE(XB_REAL_DELETE, 1, XB_REAL_DELETE)
fi
    
# compile support for filters
AC_ARG_WITH(xbase-filters, 
    [  --without-xbase-filters turn off XBase filter logic ],
    enable_xbase_filters="$withval", enable_xbase_filters="yes")
if test "$enable_xbase_filters" = "yes"; then
  AC_DEFINE(XB_FILTERS, 1, XB_FILTERS)
fi

# compile in debug logic
AC_ARG_WITH(xbase-debug, 
    [  --without-xbase-debug   turn off XBase specific debug ], 
    enable_xbase_debug="$withval", enable_xbase_debug="yes")
if test "$enable_xbase_debug" = "yes"; then
  AC_DEFINE(XBASE_DEBUG, 1, XB_DEBUG)
fi

# compile in support for exception error processing
AC_ARG_WITH(exceptions, 
    [  --with-exceptions       turn on exceptions ], 
    enable_exceptions="$withval", enable_exceptions="no")
if test "$enable_exceptions" = "yes"; then
  AC_DEFINE(HAVE_EXCEPTIONS, 1, HAVE_EXCEPTIONS)
fi
    
# compile in support for castellano dates
AC_ARG_WITH(castellano, 
    [  --with-castellano       turn on spanish date logic ], 
    enable_castellano="$withval", enable_castellano="no")
if test "$enable_castellano" = "yes"; then
  AC_DEFINE(XB_CASTELLANO, 1, XB_CASTELLANO)
fi

# turbo vision support
if test "$ac_cv_header_tvision_tv_h" = "yes"; then
    TVPROGS="tvxbase"
else
    TVPROGS=
fi
AC_SUBST(TVPROGS)


# path separator
AH_VERBATIM([PATH_SEPARATOR],[#define PATH_SEPARATOR '/'])

# default memo file block size
AH_VERBATIM([XB_DBT_BLOCK_SIZE],[#define XB_DBT_BLOCK_SIZE 512])

# build the following Makefiles 
AC_OUTPUT(
	    Makefile
	    xbase/Makefile
	    xbase/xbase.h
	    examples/Makefile
    	    bin/Makefile
            html/Makefile
	    tv/Makefile
    	    libtest/Makefile
dnl	    ybase/Makefile
	    xbase-config
	    xbase.spec
	    docs/doxygen.cfg
	    docs/Makefile
)

# 8/10/03 - gkunkel
# The following may or may not be needed. There was evidence this didn't work
# in the older configure.in. It definitely does not work in this version.
# If this is needed for some configuration, this will need to be set up.

# AC_CHECK_BOOL
# #ifndef HAVE_BOOL
# #define HAVE_BOOL
# typedef int bool;
# const bool false;
# const bool true 1;
# #endif

