/* string.cpp

    Xbase project source code

    This program demonstrates the usage of the xbString class

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

int main()
{
  xbString s1;
  xbString s2;
  
  s1 = "Some string data";
  s2 = "some more string data";
  
  std::cout << "s1 => " << s1 << std::endl;
  std::cout << "s2 => " << s2 << std::endl;


  s1 = "s1   ";
  s2 = "s2";
  s1 -= s2;
  
  std::cout << "-= operator => " << s1 << std::endl;

  s1 = "s1   "; 
  s2 = "s2";
  s1 += s2;
  
  std::cout << "+= operator => " << s1 << std::endl;

  s1 = "some data";
  s2 = s1.mid( 2, 3 );
  std::cout << "mid() = " << s2 << std::endl; 
  
  

  return 0;
}

