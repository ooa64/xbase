/*  $Id: funct.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    FUNCT.CPP
    Implementation of system functions

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

#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "funct.h"
#include "table.h"
#include "lang.h"

// absolute value
ybValue YBabs(ybValue varg[]) {
  double inval = varg[0];
  return ybValue( fabs(inval) );
}

// alias name of database in the given workspace
ybValue YBalias(ybValue varg[]) {
  // read parameters, get workspace number
  ybDbf* datab = NULL;
  if( varg[0].type() == ybUndefined ) datab = YBdbftab->dbf();
  else {
    long area = varg[0];
    // test if the workspace number is within 1..YB_DBFMAX
    if( area < 0 || area > YB_DBFMAX )
      throw yBaseExc(YBstrings[YB_NOWRKSP], area);

    datab = YBdbftab->dbf( area );
  }
  // read and return the alias name
  char* outval = datab->GetDbfAlias();
  if( outval==NULL ) return xbString("");
  else return xbString( outval );
}

// ASCII value of first character in string
ybValue YBasc(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( double( inval[0] ) );
}

// position of substring in string
ybValue YBat(ybValue varg[]) {
  xbString inlft = varg[0];
  xbString inrgt = varg[1];
  return ybValue( double( inrgt.pos(inlft)+1.0 ) );
}

// name of day in week ( Sunday - Saturday )
ybValue YBcdow(ybValue varg[]) {
  xbDate inval = varg[0];
  return ybValue( inval.CharDayOf() );
}

// character of given ASCII value
ybValue YBchr(ybValue varg[]) {
  long inval = varg[0];
  return ybValue( xbString( (char)inval ) );
}

// name of month in year ( Januray - December )
ybValue YBcmonth(ybValue varg[]) {
  xbDate inval = varg[0];
  return ybValue( inval.CharMonthOf() );
}

// conversion Character -> Date
ybValue YBctod(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( xbDate(inval) );
}

// system date
ybValue YBdate(ybValue varg[]) {
  return ybValue( xbDate() );
}

// number of day in month ( 1-31 )
ybValue YBday(ybValue varg[]) {
  xbDate inval = varg[0];
  return ybValue( double( inval.DayOf( XB_FMT_MONTH ) ) );
}

// number of day in week ( 1-7 )
ybValue YBdow(ybValue varg[]) {
  xbDate inval = varg[0];
  return ybValue( double( inval.DayOf( XB_FMT_WEEK )+1.0 ) );
}

// get the setting of enviroment variable
ybValue YBgetenv(ybValue varg[]) {
  xbString inval = varg[0];
  char* temp = getenv(inval);
  return ybValue( xbString( temp != NULL ? temp : "" ) );
}

// condition command
ybValue YBiff(ybValue varg[]) {
  bool logval = varg[0];
  return ybValue( logval ? varg[1] : varg[2] );
}

// integer value
ybValue YBint(ybValue varg[]) {
  double inval = varg[0];
  return ybValue( floor( inval ) );
}

// is first character a letter?
ybValue YBisalpha(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( (bool)isalpha(inval[0]) );
}

// is first character lower case?
ybValue YBislower(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( (bool)islower(inval[0]) );
}

// is first character upper case?
ybValue YBisupper(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( (bool)isupper(inval[0]) );
}

// number of characters in string
ybValue YBlen(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( (double)inval.len() );
}

// the natural logarithm
ybValue YBlog(ybValue varg[]) {
  double inval = varg[0];
  return ybValue( log(inval) );
}

// returns the higher value
ybValue YBmax(ybValue varg[]) {
  return ybValue( varg[0] > varg[1] ? varg[0] : varg[1] );
}

// returns the lower value
ybValue YBmin(ybValue varg[]) {
  return ybValue( varg[0] < varg[1] ? varg[0] : varg[1] );
}

// the remainder of A / B
ybValue YBmod(ybValue varg[]) {
  double inlft = varg[0];
  double inrgt = varg[1];
  return ybValue( fmod(inlft,inrgt) );
}

// number of month in year ( 1-12 )
ybValue YBmonth(ybValue varg[]) {
  xbDate inval = varg[0];
  return ybValue( (double)inval.MonthOf() );
}

// name of current operating system
// TODO: add more systems (in alphabetical order)
ybValue YBos(ybValue varg[]) {
#if defined( __MSDOS__ )
  return ybValue( xbString( "MS-DOS" ) );
#elif defined( __UNIX__ )
  return ybValue( xbString( "UNIX" ) );
#else
  return ybValue( xbString( "unknown OS" ) );
#endif
}

// count of records in database
ybValue YBreccount(ybValue varg[]) {
  ybDbf* dbf = YBdbftab->dbf();
  return ybValue( (double)dbf->NoOfRecords() );
}

// active record record
ybValue YBrecno(ybValue varg[]) {
  ybDbf* dbf = YBdbftab->dbf();
  return ybValue( (double)dbf->GetCurRecNo() );
}

// rounding
ybValue YBround(ybValue varg[]) {
  double inlft = varg[0];
  long inrgt = varg[1];

  inlft = floor( inlft * pow(10,inrgt) + 0.5 );
  inlft = inlft / pow(10,inrgt);

  return ybValue( inlft );
}

// number or active workspace
ybValue YBselect(ybValue varg[]) {
  return ybValue( (double)YBdbftab->select() );
}

// square root
ybValue YBsqrt(ybValue varg[]) {
  double inval = varg[0];
  return ybValue( sqrt(inval) );
}

// conversion Numeric -> Character
// SORRY: this code is (meanwhile) commented in the Czech language
ybValue YBstr(ybValue varg[]) {
  double inval = varg[0];
  int chars = ( varg[1].type()!=ybUndefined ? (long)varg[1] : 10 );
  int decs = ( varg[2].type()!=ybUndefined ? (long)varg[2] : 0 );

  xbString outval;

  // cela cast zobrazovaneho cisla
  double intnum = floor(inval);

  // minimalni pocet desestinnych mist
  // pro zobrazeni cele casti cisla v rozbalenem 'F' tvaru
  int minchr;
  if( intnum > 0 ) minchr = (int)floor( log10(intnum) + 1 ); // cifry
  else if( intnum < 0 ) minchr = 1 + (int)floor( log10(fabs(intnum)) + 1 ); // znamenko a cifry
  else minchr = 1; // "0"

  // minimalni pocet desestinnych mist exponentu
  int minexp;
  if( intnum > 0 ) minexp = 2 + (int)floor( log10(log10(intnum)) + 1 ); // "E+" a cifry
  else if( intnum < 0 ) minexp = 2 + (int)floor( log10(log10(fabs(intnum))) + 1 ); // "E+" a cifry
  else minexp = 4; // "E+00"
  // jednociferny exponent je vydy zobrazovan jako dvojciferny
  // E+00, E+01, E+11, E+111, E+1111, atd.
  if( minexp < 4 ) minexp = 4;
  // pridame minimalni pocet cifer cisla
  if( intnum >= 0 ) minexp += 1; // jedna cifra
  else minexp += 2; // znamenko "-" a jedna cifra

  if( minchr <= chars ) {
    // mam zobrazovat desetiny?
    // je tu misto na desetinnou tecku a jeste nejake cislo?
    if( decs > 0 && (chars-minchr) > 1 ) outval.sprintf("%*.*f",chars,chars-(minchr+1),inval);
      else outval.sprintf("%*.0f",chars,inval);
  } else if( minexp <= chars ) {
    // mam zobrazovat desetiny?
    // je tu misto na desetinnou tecku a jeste nejake cislo?
    if( decs > 0 && (chars-minexp) > 1 ) outval.sprintf("%*.*e",chars,chars-(minexp+1),inval);
      else outval.sprintf("%*.0e",chars,inval);
  } else for( int i = 0; i< chars; i++ ) outval += '*';

  return outval;
}

// current time in format HH:MM:SS
// FIXME: doesn't work according to FoxBase+ specification
ybValue YBtime(ybValue varg[]) {
  time_t systim;
  char strtim[9];
  time( &systim ); // get time
  strftime( strtim, 8, "%H:%M:%S", localtime( &systim ) );
  return ybValue( xbString( strtim ) );
}

// conversion Character -> Numeric
ybValue YBval(ybValue varg[]) {
  xbString inval = varg[0];
  return ybValue( strtod( inval, NULL ) );
}

// yBase version
ybValue YBversion(ybValue varg[]) {
  return ybValue( xbString("xBase "VERSION", yBase "YB_VERSION" ("__DATE__" "__TIME__")") );
}

// number of year
ybValue YByear(ybValue varg[]) {
  xbDate inval = varg[0];
  return ybValue( (double)inval.YearOf() );
}

// end of file

