/*  $Id: sample2.cpp,v 1.10 2003/08/16 19:59:38 gkunkel Exp $

    Xbase project source code

    This sample program updates the database created by the sample1 
    program

    This program demonstrates the use of the following functions/methods
    OpenDatabase, GetFieldNo, BlankRecord, AppendRecord, 
    PutField and CloseDatabase 

    Copyright (C) 1997  Gary A. Kunkel   

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
       xdb-devel@lists.sourceforge.net
       xdb-users@lists.sourceforge.net

     Website:

       xdb.sourceforge.net

*/

#include <xbase/xbase.h>

/* set the stack large for dos compiles */
#ifdef __XBDOS
#include <stdio.h>
extern unsigned _stklen = 100000;
#endif

int main()
{
  xbShort lname, fname, birthdate, amount, sw, f1, f2, f3, f4, m1, rc, z;
  xbFloat f;

  xbXBase x;
  xbDbf MyFile( &x );
  
#ifdef XB_INDEX_NDX
  xbNdx MyIndex1( &MyFile );
  xbNdx MyIndex2( &MyFile );
  xbNdx MyIndex3( &MyFile );
  xbNdx MyIndex4( &MyFile );
#endif

  xbDate d;


  if(( rc = MyFile.OpenDatabase( "MYFILE.DBF" )) != XB_NO_ERROR )
     std::cout << "\nError opening MYFILE.DBF rc=" << rc;
          
#ifdef XB_INDEX_NDX     
  if(( rc = MyIndex1.OpenIndex( "MYINDEX1.NDX" )) != XB_NO_ERROR )
     std::cout << "\nError opening index1 rc=" << rc;
  if(( rc = MyIndex2.OpenIndex( "MYINDEX2.NDX" )) != XB_NO_ERROR )
     std::cout << "\nError opening index2 rc=" << rc;
  if(( rc = MyIndex3.OpenIndex( "MYINDEX3.NDX" )) != XB_NO_ERROR )
     std::cout << "\nError opening index3 rc=" << rc;
  if(( rc = MyIndex4.OpenIndex( "MYINDEX4.NDX" )) != XB_NO_ERROR )
     std::cout << "\nError opening index4 rc=" << rc;
#endif

  lname     = MyFile.GetFieldNo( "LASTNAME" );
  fname     = MyFile.GetFieldNo( "FIRSTNAME" ); 
  birthdate = MyFile.GetFieldNo( "BIRTHDATE" ); 
  amount    = MyFile.GetFieldNo( "AMOUNT" );
  sw        = MyFile.GetFieldNo( "SWITCH" );
  f1        = MyFile.GetFieldNo( "FLOAT1" );
  f2        = MyFile.GetFieldNo( "FLOAT2" );
  f3        = MyFile.GetFieldNo( "FLOAT3" );
  f4        = MyFile.GetFieldNo( "FLOAT4" );
  m1        = MyFile.GetFieldNo( "MEMO1" );
  z         = MyFile.GetFieldNo( "ZIPCODE" );

  std::cout << "\nLast Name Id  = " << lname;
  std::cout << "\nFirst Name Id = " << fname;
  std::cout << "\nBirthdate Id  = " << birthdate;
  std::cout << "\nAmount Id     = " << amount;
  std::cout << "\nSwitch Id     = " << sw;
  std::cout << "\nFloat 1 Id    = " << f1;  
  std::cout << "\nFloat 2 Id    = " << f2;
  std::cout << "\nFloat 3 Id    = " << f3;
  std::cout << "\nFloat 4 Id    = " << f4;
#ifdef XB_MEMO_FIELDS
  std::cout << "\nMemo1 Id      = " << m1;
#endif
  std::cout << "\nZipcode Id    = " << z << "\n";

  /* build record one */
  MyFile.BlankRecord();			/* blank out the record buffer */
  MyFile.PutField( lname, "Queue" );			/* a name */
  MyFile.PutField( fname, "Suzy" );			/* a name */
  MyFile.PutField( birthdate, d.Sysdate());		/* a date */
  MyFile.PutField( amount, "99.99" );			/* an amount */
  MyFile.PutField( sw, "Y" );				/* a switch */
  f = 1.466f;
  MyFile.PutFloatField( f1, f );
  MyFile.PutFloatField( "FLOAT2", f );
  MyFile.PutField( f3, "1" );
  MyFile.PutField( f4, "1" );
  MyFile.PutField( z, "76262" );
  if(( rc = MyFile.AppendRecord()) != XB_NO_ERROR )       /* write it */
     std::cout << "\nError " << rc << " appending data record.";

  /* build record two */
  MyFile.BlankRecord();			/* blank out the record buffer */
  MyFile.PutField( lname, "Bob" );			/* a name */
  MyFile.PutField( fname, "Billy" );			/* a name */
  MyFile.PutField( birthdate, "19970304" );		/* a date */
  MyFile.PutField( amount, "88.88" );			/* an amount */
  MyFile.PutField( sw, "N" );				/* a switch */
  f = -2.1f; 
  MyFile.PutFloatField( f1, f );
  MyFile.PutFloatField( "FLOAT2", -2.1f );
  MyFile.PutField( f1, "-2.1" );
  MyFile.PutField( f2, "-2.1" );
  MyFile.PutField( f3, "-2.1" );
  MyFile.PutField( f4, "-2.1" );
  MyFile.PutField( z, "76261" );
#ifdef XB_MEMO_FIELDS
  MyFile.UpdateMemoData( m1, 20, "Sample memo field 2", F_SETLKW );
#endif
  if(( rc = MyFile.AppendRecord()) != XB_NO_ERROR )       /* write it */
     std::cout << "\nError " << rc << " appending data record.";

  /* build record three */
  MyFile.BlankRecord();			/* blank out the record buffer */
  MyFile.PutField( lname, "Slippery" );			/* a name */
  MyFile.PutField( fname, "Sam" );			/* a name */
  MyFile.PutField( birthdate, "19970406" );		/* a date */
  MyFile.PutField( amount, "77.77" );			/* an amount */
  MyFile.PutField( sw, "T" );				/* a switch */
  f = 3.21f; 
  MyFile.PutFloatField( f1, f );
  MyFile.PutFloatField( "FLOAT2", 3.21f );
  MyFile.PutField( f1, "3.21" );
  MyFile.PutField( f2, "3.21" );
  MyFile.PutField( f3, "3.21" );
  MyFile.PutField( f4, "3.21" );
  MyFile.PutField( z, "76263" );
#ifdef XB_MEMO_FIELDS
  MyFile.UpdateMemoData( m1, 20, "Sample memo field 3", F_SETLKW );
#endif
  if(( rc = MyFile.AppendRecord()) != XB_NO_ERROR )       /* write it */
     std::cout << "\nError " << rc << " appending data record.";

  /* build record four */
  MyFile.BlankRecord();			/* blank out the record buffer */
  MyFile.PutField( lname, "Lucas" );			/* a name */
  MyFile.PutField( fname, "George" );			/* a name */
  MyFile.PutField( birthdate, "19470406" );		/* a date */
  MyFile.PutField( amount, "77.77" );			/* an amount */
  MyFile.PutField( sw, "T" );				/* a switch */
  f = 4.321f;
  MyFile.PutFloatField( f1, f );
  MyFile.PutFloatField( "FLOAT2", 4.321f );
  MyFile.PutField( f1, "4.321" );
  MyFile.PutField( f2, "4.321" );
  MyFile.PutField( f3, "4.321" );
  MyFile.PutField( f4, "4.321" );
  MyFile.PutField( z, "76260" );
#ifdef XB_MEMO_FIELDS
  MyFile.UpdateMemoData( m1, 20, "Sample memo field 4", F_SETLKW );
#endif
  if(( rc = MyFile.AppendRecord()) != XB_NO_ERROR )       /* write it */
     std::cout << "\nError " << rc << " appending data record.";

  MyFile.CloseDatabase();				/* close database */
  return 0;
}
