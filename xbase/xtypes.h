/*  $Id: xtypes.h,v 1.5 2003/08/16 19:59:39 gkunkel Exp $

    Xbase project source code

    Copyright (C) 1997  Gary A. Kunkel   

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
	xdb-devel@lists.sourceforge.net
	xdb-users@lists.sourceforge.net

      See our website at:

        xdb.sourceforge.net

*/

#ifndef __XB_XTYPES_H__
#define __XB_XTYPES_H__

#include <stdio.h>
#ifdef _MSC_VER 
typedef __int8  int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t; 
typedef unsigned __int32 uint32_t; 
typedef __int64 int64_t; 
typedef unsigned __int64 uint64_t; 
#else 
#include <stdint.h> 
#endif 

/*! \file xtypes.h
*/

//! xbULong type
/*!
*/
//typedef unsigned long  int xbULong;
typedef uint32_t xbULong;
//! xbUShort type
/*!
*/
//typedef unsigned short int xbUShort;
typedef uint16_t xbUShort;
//! xbShort type
/*!
*/
//typedef short int          xbShort;
typedef int16_t xbShort;

typedef int32_t xbLong;
//#define xbLong long
//typedef long             LONG;
//! xbFloat type
/*!
*/
typedef float              xbFloat;
//! xbDouble type
/*!
*/
typedef double             xbDouble;


#endif      // __XB_XTYPES_H__
