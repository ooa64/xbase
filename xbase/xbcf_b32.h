/*  $Id: xbcf_b32.h,v 1.1 2000/09/20 19:45:31 dbryson Exp $

    Xbase project source code

    This file contains settings for Borland C++ 4.5

    Copyright (C) 1999	Vitaly Fedrushkov <fedrushkov@acm.org>
    www   - http://www.startech.keller.tx.us/xbase.html

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; see the file COPYING.LIB.  If
    not, write to the Free Software Foundation, Inc., 59 Temple Place
    - Suite 330, Boston, MA 02111-1307, USA.

*/

/* Xbase platform and compiler specific configuration options.

   Please note that user-configurable options are held in <options.h>.
   Do not touch settings here unless you are porting to another compiler.

   Settings for:

   Borland C++ 4.5	Win32 GUI	<fedrushkov@acm.org>
*/

/* Safety net to prevent misconfiguration */
#if !defined(__BORLANDC__) || (__BORLANDC__ != 0x460)
#error	wrong <xbase/xbconfig.h> for this compiler and/or version
#endif

#include <xbase/options.h>

#define VERSION "1.8.0b"

#define PACKAGE "xbase"

/* Define if the C++ compiler supports BOOL */
/* #undef HAVE_BOOL */

/* FIXME:	Borland C++ 4.5 can do exceptions, but has no 'class
   exception' to inherit from.  Either we provide a substitute or
   define class xbException from scratch.  -- willy */
/* Define if your compiler support exceptions */
/* #undef HAVE_EXCEPTIONS */

/* In fact there isn't one -- see at bottom */
/* Define if you have the strcasecmp function.  */
/* #undef HAVE_STRCASECMP */

/* Define if you have the vsnprintf function.  */
/* #undef HAVE_VSNPRINTF */

/* Define if you have the vsprintf function.  */
#define HAVE_VSPRINTF 1

/* Define if you have the fcntl function.  */
/* #undef HAVE_FCNTL */

/* Define if you have the <ctype.h> header file.  */
#define HAVE_CTYPE_H 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <exception> header file.  */
/* #undef HAVE_EXCEPTION */

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <g++/exception.h> header file.  */
/* #undef HAVE_G___EXCEPTION_H */

/* Define if you have the <io.h> header file.  */
#define HAVE_IO_H 1

/* Define if you have the <stdarg.h> header file.  */
#define HAVE_STDARG_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <strings.h> header file.  */
/* #undef HAVE_STRINGS_H */

/* Define if you have the <unistd.h> header file.  */
/* #undef HAVE_UNISTD_H */

/* Define if you have the <sys/locking.h> header file.  */
#define HAVE_SYS_LOCKING_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the <tvision/tv.h> header file.  */
#define HAVE_TVISION_TV_H 1

/* Should we include generic index support? */
#if defined(XB_INDEX_NDX) || defined(XB_INDEX_NTX)
#define  XB_INDEX_ANY 1
#endif

/* expressions required for indexes */
#if defined(XB_INDEX_ANY) && !defined(XB_EXPRESSIONS)
#define XB_EXPRESSIONS 1
#endif

#define PATH_SEPARATOR '\\'

/* bool stub */
#ifndef HAVE_BOOL
#define HAVE_BOOL 1
typedef int bool;
#define false 0
#define true (!false)
#endif

/* strcasecmp() substitute */
#ifndef HAVE_STRCASECMP
#define HAVE_STRCASECMP 1
#define strcasecmp(s1, s2) stricmp(s1, s2)
#endif

