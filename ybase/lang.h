/*  $Id: lang.h,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    LANG.H
    Language dependent definitions

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

#ifndef _LANG_H_
#define _LANG_H_

#define LANG_EN
//#define LANG_CZ

#define YB_ERROR 0
#define YB_ERROPEN 1
#define YB_VARNDEF 2
#define YB_FUNNDEF 3
#define YB_ALINDEF 4
#define YB_FIENDEF 5
#define YB_UNNOWORK 6
#define YB_OPNOWORK 7
#define YB_COMNOWORK 8
#define YB_TYPINCOM 9
#define YB_DIVZERO 10
#define YB_CONSNOCH 11
#define YB_INVFUCAL 12
#define YB_POSCALL 13
#define YB_EXPETYP 14
#define YB_NOWRKSP 15
#define YB_NOFRESP 16
#define YB_ASGTMIS 17

extern const char* YBstrings[];

#endif

// end of file

