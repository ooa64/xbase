/*  indextst.cpp 

    Xbase project source code

    This program creates a sample database and multiple indices.
    It tests the index logic.

    Copyright (C) 1997  StarTech, Gary A. Kunkel   
    email - xbase@startech.keller.tx.us
    www   - http://www.startech.keller.tx.us/xbase.html
    
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

int CheckIndex(
#ifdef XB_INDEX_NDX
               xbNdx *indx1, xbNdx *indx2, xbNdx *indx3
#endif
#if defined( XB_INDEX_NDX ) && defined ( XB_INDEX_NTX )
               ,
#endif
#ifdef XB_INDEX_NTX
               xbNtx *intx1, xbNtx *intx2, xbNtx *intx3
#endif
              );

int main()
{
  xbShort f1, f2, f3, rc, sts = 0;
  char charbuf[10];

  xbSchema MyRecord[] =
  {
    { "CHARFLD1",  XB_CHAR_FLD,      6, 0 },
    { "CHARFLD2",  XB_CHAR_FLD,      6, 0 },
    { "NUMFLD1",   XB_NUMERIC_FLD,   6, 0 },
    { "",0,0,0 }
  };

  /* define the classes */
  xbXBase x;			/* initialize xbase  */
  xbDbf MyFile( &x );		/* class for table   */

#ifdef XB_INDEX_NDX
  xbNdx indx1( &MyFile );	/* class for ndx index 1 */
  xbNdx indx2( &MyFile );	/* class for ndx index 2 */
  xbNdx indx3( &MyFile );	/* class for ndx index 3 */
#endif

#ifdef XB_INDEX_NTX
  xbNtx intx1( &MyFile );	/* class for ntx index 1 */
  xbNtx intx2( &MyFile );	/* class for ntx index 2 */
  xbNtx intx3( &MyFile );	/* class for ntx index 3 */
#endif

#ifndef XBASE_DEBUG
  cout << "XBASE_DEBUG support option not compiled into library";
  return 1;
#endif


  cout << "Creating test database and indices" << endl;
  if(( rc = MyFile.CreateDatabase( "IXTEST.DBF", MyRecord, XB_OVERLAY ))
        != XB_NO_ERROR )
     cout << "Error creating database = " << rc << "\n";
  else
  {
#ifdef XB_INDEX_NDX
     if(( rc = indx1.CreateIndex(
       "IXNDX1.NDX", "CHARFLD1", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
     {
        cout << "Error creating index 1 = " << rc << endl;
        exit( 1 );
     }

     if(( rc = indx2.CreateIndex(
       "IXNDX2.NDX", "CHARFLD1+CHARFLD2", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
     {
        cout << "Error creating index 2 = " << rc << endl;
        exit( 1 );
     }

     if(( rc = indx3.CreateIndex(
       "IXNDX3.NDX", "NUMFLD1", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
     {
        cout << "Error creating index 3 = " << rc << endl;
        exit( 1 );
     }
#endif


#ifdef XB_INDEX_NTX
     if(( rc = intx1.CreateIndex(
       "IXNTX1.NTX", "CHARFLD1", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
     {
        cout << "Error creating index 4 = " << rc << endl;
        exit( 1 );
     }

     if(( rc = intx2.CreateIndex( 
       "IXNTX2.NTX", "CHARFLD1+CHARFLD2", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
     {
        cout << "Error creating index 5 = " << rc << endl;
        exit( 1 );
     }

     if(( rc = intx3.CreateIndex( 
       "IXNTX3.NTX", "NUMFLD1", XB_NOT_UNIQUE, XB_OVERLAY )) != XB_NO_ERROR )
     {
        cout << "Error creating index 6 = " << rc << endl;
        exit( 1 );
     }
#endif
  }

  f1 = MyFile.GetFieldNo( "CHARFLD1" );
  f2 = MyFile.GetFieldNo( "CHARFLD2" );
  f3 = MyFile.GetFieldNo( "NUMFLD1" ); 

  cout << "Populating database and indices with data" << endl;
  cout << ".";
  cout.flush();
  for( int i = 0; i < 10000; i++ ){
      if (i % 1000 == 0)
      {
          cout << ".";
          cout.flush();
      }
    memset( charbuf, 0x00, 10 );
    sprintf( charbuf, "%d", i );
    MyFile.BlankRecord();
    MyFile.PutField( f1, charbuf );
    MyFile.PutField( f2, charbuf );
    MyFile.PutLongField( f3, i );
    MyFile.AppendRecord();
  }
  cout << " Done." << endl;

  // Check all indices.
  sts += CheckIndex(
#ifdef XB_INDEX_NDX
                    &indx1, &indx2, &indx3
#endif
#if defined( XB_INDEX_NDX ) && defined( XB_INDEX_NTX )
                    ,
#endif
#ifdef XB_INDEX_NTX
                    &intx1, &intx2, &intx3
#endif
                    );
  //
  // Appending a "Z" to the records will cause the index delete
  // functions to be used.
  //
  // - Bob Cotton <bob@synxis.com>
  //
  cout << "Reversing records." << endl;
  cout << ".";
  cout.flush();
    for( xbLong j = 1; j < MyFile.NoOfRecords(); j++ ){
      if (j  % 1000 == 0)
      {
          cout << ".";
          cout.flush();
      }
      MyFile.GetRecord(j);
      memset( charbuf, 0x00, 10 );
      sprintf( charbuf, "Z%d", (int)j-1 );
      MyFile.PutField( f1, charbuf );
      MyFile.PutField( f2, charbuf );
      MyFile.PutLongField( f3, j-1 );
      MyFile.PutRecord();
  }
  cout << " Done." << endl;
    
          
  // Check all indices
  sts += CheckIndex(
#ifdef XB_INDEX_NDX
                     &indx1, &indx2, &indx3
#endif
#if defined( XB_INDEX_NDX ) && defined( XB_INDEX_NTX )
                     ,
#endif
#ifdef XB_INDEX_NTX
                    &intx1, &intx2, &intx3
#endif
                    );
    

  cout << "Index testing completed" << endl;cout.flush();
  MyFile.CloseDatabase();   /* Close database and associated indexes */
  return sts;
}     


int CheckIndex(
#ifdef XB_INDEX_NDX
    xbNdx *indx1,
    xbNdx *indx2,
    xbNdx *indx3
#endif
#if defined( XB_INDEX_NDX ) && defined( XB_INDEX_NTX )
   ,
#endif
#ifdef XB_INDEX_NTX
    xbNtx *intx1,
    xbNtx *intx2,
    xbNtx *intx3
#endif
    )
{
    int rc;
    int sts = 0;
    
#if defined( XB_INDEX_NDX ) && defined( XBASE_DEBUG )
    cout << "Testing NDX index 1 "; cout.flush();
    if(( rc = indx1->CheckIndexIntegrity(0)) != XB_NO_ERROR ){
        cout << "Error " << rc << " with index indx1" << endl;
        sts++;
    }
    else
        cout << "OK" << endl;

    cout << "Testing NDX index 2 ";cout.flush();
    if(( rc = indx2->CheckIndexIntegrity(0)) != XB_NO_ERROR ){
        cout << "Error " << rc << " with index indx2" << endl;
        sts++;
    }
    else
        cout << "OK" << endl;


    cout << "Testing NDX index 3 ";cout.flush();
    if(( rc = indx3->CheckIndexIntegrity(0)) != XB_NO_ERROR ){
        cout << "Error " << rc << " with index indx3" << endl;
        sts++;
    }
    else
        cout << "OK" << endl;
#endif
#if defined( XB_INDEX_NTX ) && defined( XBASE_DEBUG )
    cout << "Testing NTX index 1 ";cout.flush();
    if(( rc = intx1->CheckIndexIntegrity(0)) != XB_NO_ERROR ){
        cout << "Error " << rc << " with index intx1" << endl;
        sts++;
    }
    else
        cout << "OK" << endl;

    cout << "Testing NTX index 2 ";cout.flush();
    if(( rc = intx2->CheckIndexIntegrity(0)) != XB_NO_ERROR ){
        cout << "Error " << rc << " with index intx2" << endl;
        sts++;
    }
    else
        cout << "OK" << endl;

    cout << "Testing NTX index 3 ";cout.flush();
    if(( rc = intx3->CheckIndexIntegrity(0)) != XB_NO_ERROR ){
        cout << "Error " << rc << " with index intx3" << endl;
        sts++;
    }
    else
        cout << "OK" << endl;
#endif
    return sts;

}

