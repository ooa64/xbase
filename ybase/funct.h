/*  $Id: funct.h,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    FUNCT.H
    Definition of system functions

    Copyright (C) 1999  Petr Gotthard
    email - petr.gotthard@seznam.cz

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    V 0.1   20/09/99   - First prerelease
*/

#ifndef _FUNCT_H_
#define _FUNCT_H_

#include "types.h"
#include "treexp.h"

ybValue YBabs(ybValue[]);
ybValue YBalias(ybValue[]);
ybValue YBasc(ybValue[]);
ybValue YBat(ybValue[]);
ybValue YBcdow(ybValue[]);
ybValue YBchr(ybValue[]);
ybValue YBcmonth(ybValue[]);
ybValue YBctod(ybValue[]);
ybValue YBdate(ybValue[]);
ybValue YBday(ybValue[]);
ybValue YBdow(ybValue[]);
ybValue YBgetenv(ybValue[]);
ybValue YBiff(ybValue[]);
ybValue YBint(ybValue[]);
ybValue YBisalpha(ybValue[]);
ybValue YBislower(ybValue[]);
ybValue YBisupper(ybValue[]);
ybValue YBlen(ybValue[]);
ybValue YBlog(ybValue[]);
ybValue YBmax(ybValue[]);
ybValue YBmin(ybValue[]);
ybValue YBmod(ybValue[]);
ybValue YBmonth(ybValue[]);
ybValue YBos(ybValue[]);
ybValue YBreccount(ybValue[]);
ybValue YBrecno(ybValue[]);
ybValue YBround(ybValue[]);
ybValue YBselect(ybValue[]);
ybValue YBsqrt(ybValue[]);
ybValue YBstr(ybValue[]);
ybValue YBtime(ybValue[]);
ybValue YBval(ybValue[]);
ybValue YBversion(ybValue[]);
ybValue YByear(ybValue[]);

# endif

// end of file

