/*  $Id: deletall.cpp,v 1.5 2000/09/20 23:11:47 dbryson Exp $

    Xbase project source code

    This sample program deletes all records in an Xbase DBF file

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
    V 1.8   11/29/98   - Version 1.8 upgrade
*/

#include <xbase/xbase.h>

int main(int ac,char** av)
{
    if (ac <= 1) {
        cout <<
            "\nUsage: deletall filename...\n"
            ;
        return 1;
    }
    
    for (int i=1; i<ac; ++i) {
        char* filename = av[i];

   xbXBase x;
   xbDbf MyFile( &x );

        if( MyFile.OpenDatabase( filename )) {
      cout << "Could not open file " << filename << "\n";
            return 2;
   }

        xbShort rc = MyFile.DeleteAllRecords(); 
        if( rc != XB_NO_ERROR ) {
      cout << "\nError Deleting all records - database ==> " << filename;
      cout << " Return Code = " << rc;
   }
   MyFile.CloseDatabase();	/* close database */

   cout << "\nDone...\n\n";
    }
    
    return 0;
}     
