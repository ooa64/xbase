/*  $Id: sample5-ntx.cpp,v 1.5 2003/08/16 19:59:38 gkunkel Exp $

    Xbase project source code

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
#ifdef DOS
#include <stdio.h>
extern unsigned _stklen = 100000;
#endif

main()
{
   xbShort rc;     /* return code */
   xbXBase x;
   xbDbf d( &x );
 
#ifdef XB_INDEX_NTX
   xbNtx i1( &d );
   xbNtx i2( &d );
   xbNtx i3( &d );
   xbNtx i4( &d );
#endif

   rc = d.OpenDatabase( "MYFILE" );
   cout << "OpenDatabase rc = " << rc << "\n";

#ifdef XB_INDEX_NTX
   rc = i1.OpenIndex( "MYINDEX1" );
   cout << "OpenIndex 1 rc = " << rc << "\n";

   rc = i2.OpenIndex( "MYINDEX2" );
   cout << "OpenIndex 2 rc = " << rc << "\n";

   rc = i3.OpenIndex( "MYINDEX3" );
   cout << "OpenIndex 3 rc = " << rc << "\n";

   rc = i4.OpenIndex( "MYINDEX4" );
   cout << "OpenIndex 4 rc = " << rc << "\n";

   rc = i1.FindKey( "Queue" );			/* alpha search   */
   cout << "Find Key rc = " << rc << "\n";

   rc = i4.FindKey( (xbDouble) 77.77 );		/* numeric search */
   cout << "Find Key rc = " << rc << "\n";

   rc = i1.GetFirstKey();
   cout << "GetFirstKey rc = " << rc << "\n";

   rc = i1.GetNextKey();
   cout << "GetNextKey rc = " << rc << "\n";

   rc = i1.GetLastKey();
   cout << "GetLastKey rc = " << rc << "\n";

   rc = i1.GetPrevKey();
   cout << "GetPrevKey rc = " << rc << "\n";
#endif

   d.CloseDatabase();
   return 0;
}
