/*  $Id: testdbinfo.cpp,v 1.3 2000/09/20 23:11:48 dbryson Exp $

    Xbase project source code

    This program creates a sample database and four indexes

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
#ifdef __XBDOS
#include <stdio.h>
extern unsigned _stklen = 100000;
#endif

int main()
{

  int
    i;
    
#ifdef XB_INDEX_NDX    
  char
    expBuf[1024];

  xbIndex
    *index;
#endif // XB_INDEX_NDX
    
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
#ifdef XB_INDEX_NDX
  xbNdx MyIndex1( &MyFile );	/* class for index 1 */
  xbNdx MyIndex2( &MyFile );	/* class for index 2 */
  xbNdx MyIndex3( &MyFile );	/* class for index 3 */
  xbNdx MyIndex4( &MyFile );	/* class for index 4 */
#endif // XB_INDEX_NDX


  xbShort rc;
  MyFile.SetVersion( 4 );   /* create dbase IV style files */

  if(( rc = MyFile.CreateDatabase( "MYFILE.DBF", MyRecord, XB_OVERLAY )) 
        != XB_NO_ERROR )
     cout << "\nError creating database = " << rc << "\n";
  else
  {
     /* define a simple index */
#ifdef XB_INDEX_NDX
     if(( rc = MyIndex1.CreateIndex( 
       "MYINDEX1.NDX", "LASTNAME", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
        cout << "\nError creating index 1 = " << rc << "\n";

     /* define a multi-field index "LASTNAME            FIRSTNAME" */
     if(( rc = MyIndex2.CreateIndex( 
       "MYINDEX2.NDX", "LASTNAME+FIRSTNAME", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
        cout << "\nError creating index 2 = " << rc << "\n";

     /* define a multi-field index "LASTNAMEFIRSTNAME" */
     if(( rc = MyIndex3.CreateIndex( 
       "MYINDEX3.NDX", "LASTNAME-FIRSTNAME", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
        cout << "\nError creating index 3 = " << rc << "\n";

     /* define a numeric index "ZIPCODE" */
     if(( rc = MyIndex4.CreateIndex( 
       "MYINDEX4.NDX", "ZIPCODE", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
        cout << "\nError creating index 4 = " << rc << "\n";
#endif // XB_INDEX_NDX
  }
  
  printf("FieldCount = %ld\n", MyFile.FieldCount());
  for(i = 0; i < MyFile.FieldCount(); i++)
  {
    printf("Field %d: Name = '%s', Type = '%c', Len = %d\n", i,
           MyFile.GetFieldName(i), MyFile.GetFieldType(i),
           MyFile.GetFieldLen(i));
  }
  printf("\n");
  
#ifdef XB_INDEX_NDX
  printf("IndexCount = %d\n", MyFile.IndexCount());
  for(i = 0; i < MyFile.IndexCount(); i++)
  {
    index = MyFile.GetIndex(i);
    index->GetExpression(expBuf, 1024);
    printf("Index %d: '%s'\n", i, expBuf);
  }
#endif // XB_INDEX_NDX

  MyFile.CloseDatabase();   /* Close database and associated indexes */
  return 0;
}     
