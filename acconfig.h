/* $Id: acconfig.h,v 1.2 2001/06/26 19:38:28 dbryson Exp $ */

/* Define if the C++ compiler supports BOOL */
#undef HAVE_BOOL

/* defines if having libgif (always 1) */
#undef HAVE_LIBGIF

/* defines if having libjpeg (always 1) */
#undef HAVE_LIBJPEG

/* defines which to take for ksize_t */
#undef ksize_t

/* define if you have setenv */
#undef HAVE_FUNC_SETENV

/* Define to 1 if NLS is requested.  */
#undef ENABLE_NLS

/* Define as 1 if you have the stpcpy function.  */
#undef HAVE_STPCPY

/* Define if your locale.h file contains LC_MESSAGES.  */
#undef HAVE_LC_MESSAGES    

/* Define if you need the GNU extensions to compile */
#undef _GNU_SOURCE

/* Define if you need to have .ndx indexes */
#undef XB_INDEX_NDX

/* Define if you need to have .ntx indexes */
#undef XB_INDEX_NTX

/* Define if you need to support memo fields */
#undef XB_MEMO_FIELDS

/* Define if you need expressions */
#undef XB_EXPRESSIONS

/* Define if you need html support */
#undef XB_HTML

/* Define if you need locking support */
#undef XB_LOCKING_ON

/* Define if you need to turn on XBase specific debug */
#undef XBASE_DEBUG

/* Define if your compiler support exceptions */
#undef HAVE_EXCEPTIONS

/* Define if you want Castellano (Spanish) Dates */
#undef XB_CASTELLANO

/* Define if using real deletes */
#undef XB_REAL_DELETE

/* Define if need filters */
#undef XB_FILTERS


@BOTTOM@

/* Should we include generic index support? */
#if defined(XB_INDEX_NDX) || defined(XB_INDEX_NTX)
#define  XB_INDEX_ANY 1
#endif

/* expressions required for indexes */
#if defined(XB_INDEX_ANY) && !defined(XB_EXPRESSIONS)
#define XB_EXPRESSIONS 1
#endif

/* default memo block size */
#define XB_DBT_BLOCK_SIZE  512

/* filename path separator */
#define PATH_SEPARATOR '/'

#ifndef HAVE_BOOL
#define HAVE_BOOL 1
typedef int bool;
const bool false = 0;
const bool true = 1;
#endif
