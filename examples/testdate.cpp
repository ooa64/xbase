/*  $Id: testdate.cpp,v 1.5 2000/09/20 23:11:48 dbryson Exp $

    Xbase project source code

    This program tests the xdate routines 

    Copyright (C) 1997  StarTech, Gary A. Kunkel   

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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact:

      Mail:
  
        Technology Associates, Inc.
        XBase Project
        1455 Deming Way #11
        Sparks, NV  89434
        USA

     Email:
       
       xbase@techass.com

     Website:

       linux.techass.com



    V 1.0   10/10/97   - Initial release of software
    V 1.5   1/2/98     - Added memo field support
    V 1.6a  5/1/98     - Added expression support
    V 1.8   11/29/98   - Version 1.8 upgrade
    V 1.9   7/10/99    - CPP ified the date class


    This program tests and demonstrates usage of the various xbDate methods

    Several of the tests pass either StringDate or Chardate to the method 
    being tested - this is done to test the class methods.  In actual usage, 
    this may not be necessary due to the instance of the class already 
    containing a date value.  
*/

#include <xbase/xbase.h>

int main()
{
   xbString StringDate( "19601007" );      /* oct 7 1960 */
   char     CharDate[9] = "19611109";      /* nov 9 1961 */

   xbDate d1;                              /* today is default   */
   xbDate d2( StringDate );                /* from string data   */ 
   xbDate d3( CharDate );                  /* from char data     */
   xbDate d4;                              /* another date class */

   cout << "This program tests the XDATE routines" << endl;
   cout << "Date 1 (Sysdate) is    " << d1.GetDate() << endl;
   cout << "Date 2 (StringDate) is " << d2.GetDate() << endl;
   cout << "Date 3 (CharDate) is   " << d3.GetDate() << endl;

   cout << "This year is  " << d1.YearOf() << endl;
   cout << "Year of xbString Date is " <<  d2.YearOf( StringDate ) << endl;
   cout << "Year of char Date is "     <<  d3.YearOf( CharDate ) << endl;

   cout << "This Month is " << d1.MonthOf() << endl;
   cout << "Month of xbString Date is " <<  d2.MonthOf( StringDate ) << endl;
   cout << "Month of char Date is "     <<  d3.MonthOf( CharDate ) << endl;

   cout << "Today is day " << d1.DayOf( XB_FMT_WEEK ) << " of the week" << endl; 
   cout << "StringDate is day " << d2.DayOf( XB_FMT_MONTH, StringDate ) << " of the month" << endl;
   cout << "CharDate is day " << d3.DayOf( XB_FMT_YEAR, CharDate ) << " of the year" << endl;

   if( d1.IsLeapYear())
      cout << "This is a leapyear" << endl;
   else
      cout << "This is not a leap year." << endl;
 
   if( d2.IsLeapYear( StringDate ))
      cout << "StringDate is a leapyear" << endl;
   else
      cout << "StringDate is not a leap year." << endl;
 
   if( d3.IsLeapYear( CharDate ))
      cout << "CharDate is a leapyear" << endl;
   else
      cout << "CharDate is not a leap year." << endl;
 
   cout << "Today is " << d1.Sysdate() << endl;

   if( d1.DateIsValid( "19951301" ))
      cout << "19951301 is a valid date" << endl;
   else
      cout << "19951301 is not a valid date" << endl;

   if( d1.DateIsValid( "19920229" ))
      cout << "19920229 is a valid date" << endl;
   else
      cout << "19920229 is not a valid date" << endl;

   if( d2.DateIsValid( StringDate ))
      cout << StringDate << " is a valid date" << endl;
   else
      cout << StringDate << " is not a valid date" << endl;

   cout << "Today's Julian date " << d1.JulianDays() << endl;
   cout << "StringDate Julian date " << d2.JulianDays( StringDate ) << endl;

   cout << "There are " << 
        ( d1.JulianDays( "19951101" ) - d1.JulianDays( "19951001" ))
        << " days between 10/1/95 and 11/1/95." << endl;

   cout << "Todays Julian date (Number of days since Jan 1 0100):"
        << d1.JulianDays() << endl; 

   d4 = d1;    // set d4 class = to sysdate 
   cout << "Object d4 initialized to " << d4.GetDate() << endl;
   cout << "This should be todays date: "  
        << d4.JulToDate8(d4.JulianDays()) << endl;
   cout << "In 7 days it will be "  
        << d4.JulToDate8(d4.JulianDays() + 7L ) << endl;

   cout << "Today is " << d1.CharDayOf() << endl;
   cout << "StringDate day is " << d2.CharDayOf( StringDate ) << endl;
   cout << "This month is " << d1.CharMonthOf() << endl;
   cout << "StringDate month is " << d2.CharMonthOf() << endl;


   /* various format routines using different formats, strings and chars */
   xbString xbStFmt( "MM/DD/YY" );
   cout << "Format (YYDDD)     " << d1.FormatDate( "YYDDD" ) << endl;
   cout << "Format (MM/DD/YY)  " << d1.FormatDate( xbStFmt ) << endl;
   xbStFmt = "DD/MM/YY";
   cout << "Format (DD/MM/YY)  " << d1.FormatDate(xbStFmt, "19730110") << endl;
   cout << "Format (MMMM DD,YYYY)    " <<
      d1.FormatDate( "MMMM DD,YYYY", d1.GetDate()) << endl;
   xbStFmt = "DDDD, MMMM DD YYYY";
   cout << "Format (DDDD, MMMM DD YYYY) " <<
      d2.FormatDate( xbStFmt, d2.GetDate()) << endl;

   cout << "Last day this month " << d1.LastDayOfMonth() << endl;
   cout << "Last day of month for StringDate is " <<
      d2.LastDayOfMonth( d2.GetDate()) << endl;

   cout << "Overloaded operators test..." << endl;

   if( d1 == d2 )
     cout << d1.GetDate() << " is equal to " << d2.GetDate() << endl;
   else
     cout << d1.GetDate() << " is not equal to " << d2.GetDate() << endl;

   if( d1 != d3 )
     cout << d1.GetDate() << " is not equal to " << d3.GetDate() << endl;
   else
     cout << d1.GetDate() << " is equal to " << d3.GetDate() << endl;
     
   if( d1 < d2 )
     cout << d1.GetDate() << " is less than " << d2.GetDate() << endl;
   else
     cout << d1.GetDate() << " is not less than " << d2.GetDate() << endl;
     
   if( d1 > d2 )
     cout << d1.GetDate() << " is greater than " << d2.GetDate() << endl;
   else
     cout << d1.GetDate() << " is not greater than " << d2.GetDate() << endl;
     
   if( d1 <= d2 )
     cout << d1.GetDate() << " is less than or equal to " << d2.GetDate() << endl;
   else
     cout << d1.GetDate() << " is not less than or equal to " << d2.GetDate() << endl;
     
   if( d1 >= d2 )
     cout << d1.GetDate() << " is greater than or equal to " << d2.GetDate() << endl;
   else
     cout << d1.GetDate() << " is not greater than or equal to " << d2.GetDate() << endl;

   d1.Sysdate();
   d1++;
   cout << "Tomorrow is " << d1.GetDate() << endl;
   d1-=2;
   cout << "Yesterday was " << d1.GetDate() << endl;
   cout << "There are " << d1 - d2 << " days between " << d1.GetDate() 
        << " and " << d2.GetDate() << endl;

   return 0;
}
    
