/*  $Id: sample1-ntx.cpp,v 1.4 2000/09/20 23:11:48 dbryson Exp $

    Xbase project source code

    This program creates a sample database and two indexes

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
*/

#include <xbase/xbase.h>

/* set the stack large for dos compiles */
#ifdef DOS
#include <stdio.h>
extern unsigned _stklen = 100000;
#endif

main()
{

  xbSchema MyRecord[] = 
  {
    { "FIRSTNAME", XB_CHAR_FLD,     15, 0 },
    { "LASTNAME",  XB_CHAR_FLD,     20, 0 },
    { "BIRTHDATE", XB_DATE_FLD,      8,  0 },
    { "AMOUNT",    XB_NUMERIC_FLD,   9,  2 },
    { "SWITCH",    XB_LOGICAL_FLD,   1,  0 },
    { "FLOAT1",    XB_FLOAT_FLD,     9,  2 },
    { "FLOAT2",    XB_FLOAT_FLD,     9,  1 },
    { "FLOAT3",    XB_FLOAT_FLD,     9,  2 },
    { "FLOAT4",    XB_FLOAT_FLD,     9,  3 },
    { "MEMO1",     XB_MEMO_FLD,     10, 0 },
    { "ZIPCODE",   XB_NUMERIC_FLD,   5,  0 },      
    { "",0,0,0 }
  };

  /* define the classes */
  xbXBase x;			/* initialize xbase  */
  xbDbf MyFile( &x );		/* class for table   */
  xbNtx MyIndex1( &MyFile );	/* class for index 1 */
  xbNtx MyIndex2( &MyFile );	/* class for index 2 */
  xbNtx MyIndex3( &MyFile );	/* class for index 3 */
  xbNtx MyIndex4( &MyFile );	/* class for index 4 */


  xbShort rc;
  MyFile.SetVersion( 4 );   /* create dbase IV style files */

  if(( rc = MyFile.CreateDatabase( "MYFILE.DBF", MyRecord, XB_OVERLAY )) 
        != XB_NO_ERROR )
     cout << "\nError creating database = " << rc << "\n";
  else
  {
     /* define a simple index */

     if(( rc = MyIndex1.CreateIndex( 
       "MYINDEX1.NTX", "LASTNAME", 0, 1 )) != XB_NO_ERROR )
        cout << "\nError creating index 1 = " << rc << "\n";

     /* define a multi-field index "LASTNAME            FIRSTNAME" */
     if(( rc = MyIndex2.CreateIndex( 
       "MYINDEX2.NTX", "LASTNAME+FIRSTNAME", 0, 1 )) != XB_NO_ERROR )
        cout << "\nError creating index 2 = " << rc << "\n";

     /* define a multi-field index "LASTNAMEFIRSTNAME" */
     if(( rc = MyIndex3.CreateIndex( 
       "MYINDEX3.NTX", "LASTNAME-FIRSTNAME", 0, 1 )) != XB_NO_ERROR )
        cout << "\nError creating index 3 = " << rc << "\n";

     /* define a numeric index "ZIPCODE" */
     if(( rc = MyIndex4.CreateIndex( 
       "MYINDEX4.NTX", "ZIPCODE", 0, 1 )) != XB_NO_ERROR )
        cout << "\nError creating index 4 = " << rc << "\n";
  }

  MyFile.CloseDatabase();   /* Close database and associated indexes */
  return 0;
}     
