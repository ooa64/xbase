/*  $Id: sample4.cpp,v 1.8 2003/08/16 19:59:38 gkunkel Exp $

    Xbase project source code

    This sample program reads the database created and updated by the sample1 
    and sample2 program

    This program demonstrates the use of the following functions/methods
    DeleteRecord, UndeleteRecord, RecordDeleted

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

#ifdef __XBDOS
#include <stdio.h>
extern unsigned _stklen = 100000;
#endif

int main()
{
  xbShort rc;
  xbXBase x;
  xbDbf MyFile( &x );

#ifdef XB_INDEX_NDX
  xbNdx MyIndex1( &MyFile );
  xbNdx MyIndex2( &MyFile );
  xbNdx MyIndex3( &MyFile );
  xbNdx MyIndex4( &MyFile );
#endif

  if(( rc = MyFile.OpenDatabase( "MYFILE.DBF" )) != XB_NO_ERROR )
  {
     std::cout << "\nError opening file rc = " << rc << "\n";
     exit(1);
  }
  
#ifdef XB_INDEX_NDX
  if(( rc = MyIndex1.OpenIndex( "MYINDEX1.NDX" )) != XB_NO_ERROR )
  {
     std::cout << "\nError opening index1 rc = " << rc << "\n";
     exit(1);
  }

  if(( rc = MyIndex2.OpenIndex( "MYINDEX2.NDX" )) != XB_NO_ERROR )
  {
     std::cout << "\nError opening index2 rc = " << rc << "\n";
     exit(1);
  }
  if(( rc = MyIndex3.OpenIndex( "MYINDEX3.NDX" )) != XB_NO_ERROR )
  {
     std::cout << "\nError opening index3 rc = " << rc << "\n";
     exit(1);
  }
  if(( rc = MyIndex4.OpenIndex( "MYINDEX4.NDX" )) != XB_NO_ERROR )
  {
     std::cout << "\nError opening index4 rc = " << rc << "\n";
     exit(1);
  }
#endif

  std::cout << "Sample GetRecord\n"; 

  MyFile.ExclusiveLock( F_SETLKW ); /* lock the files for our exclusive use */

  MyFile.GetRecord( 2L );

  MyFile.DeleteRecord();
  
  if( MyFile.RecordDeleted() )
    std::cout << "Record is deleted...\n";
  else 
    std::cout << "Record is not deleted...\n";


/* to undelete a record the following commented code could be used

  MyFile.UndeleteRecord();
  if( MyFile.RecordDeleted() )
     std::cout << "Record is deleted...\n";
  else 
    std::cout << "Record is not deleted...\n";
*/

/* to permanently remove deleted records from the file, pack the database */

   if(( rc = MyFile.PackDatabase( F_SETLKW )) != XB_NO_ERROR )
      std::cout << "\nError packing database rc = " << rc;
   else
      std::cout << "\nDatabase packed.\n";

  MyFile.ExclusiveUnlock();     /* unlock the files */
  MyFile.CloseDatabase();	/* close database */
  return 0;
}     
