/*  $Id: reindex.cpp,v 1.6 2000/09/27 17:25:07 dbryson Exp $

    Xbase project source code

    This sample program packs an Xbase DBF file

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

       xdb.sourceforge.net


    V 1.0   10/10/97   - Initial release of software
    V 1.5   1/2/98     - Added memo field support
    V 1.6a  5/1/98     - Added expression support
    V 1.8   11/29/98   - Version 1.8 upgrade
*/

#include <xbase/xbase.h>

int main(int ac,char** av)
{
    if (3 != ac) {
        cout << 
            "\nUsage: reindex dbf_file ndx_file\n"
            ;
        return 1;
    }
    
    char* filename = av[1];
    char* filename2 = av[2];

   xbXBase x;
   xbDbf MyFile( &x );
   xbNdx MyIndex( &MyFile );

    if( MyFile.OpenDatabase( filename )) {
      cout << "Could not open file " << filename << "\n";
      return 0;
   }
    if( MyIndex.OpenIndex( filename2 )) {
      cout << "Could not open index file " << filename2 << "\n";
      return 0;
   }

    xbShort rc = MyIndex.ReIndex();
    if( rc != XB_NO_ERROR ) {
      cout << "\nError reindexing index ==> " << filename2;
      cout << " Return Code = " << rc;
   }

    /* or
   if(( rc = MyFile.RebuildAllIndicis()) != XB_NO_ERROR ) 
   {
      cout << "\nError reindexing...";
      cout << "\nReturn Code = " << rc;
   }
    */
   MyFile.CloseDatabase();	/* close database */
    
    return 0;
}     
