/*  $Id: options.h,v 1.4 2000/10/31 00:59:48 dbryson Exp $
    
    Xbase project source code

    Copyright (C) 1997  StarTech, Gary A. Kunkel

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact:

      Mail:

        Technology Associates, Inc.
        XBase Project
        1455 Deming Way #11
        Sparks, NV 89434
        USA

      Email:

        xbase@techass.com

      See our website at:

        xdb.sourceforge.net


    V 1.0   10/10/97   - Initial release of software
    V 1.5   1/2/98     - Added Dbase IV memo field support
    V 1.6a  4/1/98     - Added expression support
    V 1.8   12/3/98    - Version 1.8 upgrade
*/

#ifndef __XB_OPTIONS_H__
#define __XB_OPTIONS_H__

/************************************************************************/
/* Xbase options
   these are the library compile options for a non-autoconf environment
   
   Please note that this file is not used under Unix, as all configurable
   options are controlled by 'configure' script.
*/

/* Define if you need to turn on XBase specific debug */
/* #undef XBASE_DEBUG 1 */

/* Define if you need to have .ndx indexes */
#define XB_INDEX_NDX 1

/* Define if you need to have .ntx indexes */
#define XB_INDEX_NTX 1

/* Multi-user record/file locking is an experimental alpha level feature
   under DOS/Windows */
/* Define if you need locking support */
/* #define XB_LOCKING_ON 1 */

/* Define if you need html support */
#define XB_HTML 1

/* Define if you need to support memo fields */
#define XB_MEMO_FIELDS 1

/* Define if you need expressions */
#define XB_EXPRESSIONS 1

/* Define if you want Castellano (Spanish) Dates */
/* #undef XB_CASTELLANO */

#define XB_DBT_BLOCK_SIZE  512      /* default memo block size */
               /* must be increment of 512   */

#endif      // __XB_OPTIONS_H__
