/*  $Id: xdate.h,v 1.2 2000/06/06 23:36:46 dbryson Exp $

    Xbase project source code

    This file contains a header file for the xbDate object, which is 
    used for handling dates.

    Copyright (C) 1997  StarTech, Gary A. Kunkel   
    email - xbase@startech.keller.tx.us
    www   - http://www.startech.keller.tx.us/xbase.html
    ftp   - ftp://ftp.startech.keller.tx.us

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

    V 1.0    10/10/97   - Initial release of software
    V 1.5    1/2/98     - Added memo field support
    V 1.6a   4/1/98     - Added expression support
    V 1.6b   4/8/98     - Numeric index keys
    V 1.7.1  5/25/98    - Enhancements/bug fixes from eljorgo@fontun.com
    V 1.8.0a 1/29/99    - Added Default Date Format processing
    V 1.9.0  6/23/99    - CPP ified and minor bug fixes
*/

#ifndef __XB_XDATE_H__
#define __XB_XDATE_H__

#include <xdb/xbconfig.h>
#include <xdb/xbstring.h>

#define XB_FMT_WEEK   1
#define XB_FMT_MONTH  2
#define XB_FMT_YEAR   3

class XBDLLEXPORT xbDate {
  public:
    xbDate();
    xbDate( const char * Date8 );
    xbDate( const xbString &Date8 );

    const xbString & GetDate() const 
      { return cDate8; };
    xbString & GetDate() 
      { return cDate8; };
    const xbString & GetFormattedDate() const 
      { return fDate; };
    xbString & GetFormattedDate() 
      { return fDate; };

    int SetDate( const char * Date8 );
    int SetDate( const xbString & Date8 )
         { return SetDate((const char *) Date8 ); };

    long JulianDays    ( const char *Date8 ) const;
    long JulianDays    ( const xbString & Date8 ) const
         { return JulianDays((const char *) Date8 ); };
    long JulianDays    () const
         { return JulianDays((const char *) cDate8 ); };

    int  YearOf        ( const char *Date8 ) const;
    int  YearOf        ( const xbString & Date8 ) const
         { return YearOf((const char *) Date8 ); };
    int  YearOf        () const
         { return YearOf((const char *) cDate8 ); };

    int  MonthOf       ( const char *Date8 ) const;
    int  MonthOf       ( const xbString &Date8 ) const
         { return MonthOf((const char *) Date8 ); };
    int  MonthOf       () const
         { return MonthOf(( const char *) cDate8 ); };
    
    int  DayOf         ( int Format, const char *Date8 ) const;
    int  DayOf         ( int Format, const xbString &Date8 ) const
         { return DayOf( Format, (const char *) Date8 ); };
    int  DayOf         ( int Format ) const
         { return DayOf( Format, (const char *) cDate8 ); };
    
    int  IsLeapYear    ( const char *Date8 ) const;
    int  IsLeapYear    ( const xbString &Date8 ) const
         { return IsLeapYear((const char *) Date8 ); };
    int  IsLeapYear    () const
         { return IsLeapYear((const char *) cDate8 ); };
    
    int  DateIsValid   ( const char *Date8 ) const;
    int  DateIsValid   ( const xbString & Date8 ) const
         { return DateIsValid( (const char *) Date8 ); };
	 
    xbString& LastDayOfMonth( const char *Date8 );
    xbString& LastDayOfMonth( const xbString & Date8 )
         { return LastDayOfMonth((const char *) Date8 ); };
    xbString& LastDayOfMonth()
         { return LastDayOfMonth((const char *) cDate8 ); };

    xbString& Sysdate   ();
    xbString& JulToDate8( long );
  
    xbString& FormatDate( const char *Format, const char *Date8 );
    xbString& FormatDate( const xbString &Format, const char *Date8 )
     { return FormatDate((const char *) Format, Date8 ); };
    xbString& FormatDate( const char *Format, const xbString &Date8 )
     { return FormatDate( Format, (const char *) Date8 ); };
    xbString& FormatDate( const xbString &Format, const xbString &Date8 )
     { return FormatDate((const char *) Format,(const char *) Date8 ); };
    xbString& FormatDate( const char *Format )
     { return FormatDate( (const char *) Format, (const char *) cDate8 ); };
    xbString& FormatDate( const xbString &Format )
     { return FormatDate((const char *) Format, (const char *) cDate8 ); };
    
    xbString& CharDayOf ( const char *Date8 );
    xbString& CharDayOf ( const xbString &Date8 )
      { return CharDayOf((const char *) Date8 ); };
    xbString& CharDayOf ()
      { return CharDayOf((const char *) cDate8 ); };

    xbString& CharMonthOf ( const char *Date8 );
    xbString& CharMonthOf ( const xbString &Date8 )
      { return CharMonthOf(( const char *) Date8 ); };
    xbString& CharMonthOf ()
      { return CharMonthOf(( const char *) cDate8 ); };

    xbString &operator+=( int );
    xbString &operator-=( int );
    xbString &operator++( int );              /* post increment */
    xbString &operator--( int );              /* post increment */
    xbString &operator+ ( int );
    xbString &operator- ( int );
    long operator-( const xbDate & ) const;
    int operator==( const xbDate & ) const;
    int operator!=( const xbDate & ) const;
    int operator< ( const xbDate & ) const;
    int operator> ( const xbDate & ) const;
    int operator<=( const xbDate & ) const;
    int operator>=( const xbDate & ) const;

  private:
    void SetDateTables();
    xbString cDate8;  /* CCYYMMDD date format */
    xbString fDate;   /* other date format    */
    static int AggregatedDaysInMonths[2][13];
    static int DaysInMonths[2][13];
    static const xbString *Days[7];
    static const xbString *Months[12];
};    

#endif    // __XB_XDATE_H__
