/*  $Id: locktest.cpp,v 1.1 2000/06/07 00:05:28 dbryson Exp $

    this debugging program tests Xbase locking functions
*/

#include "xbase.h"
#include <stdlib.h>
#include <stdio.h>

int main(int, char**)
{
#ifdef XBASE_LOCKING_ON
   xbShort rc;

   if(2!=ac){
     cout << "\nUsage: locktest filename\n";
     return 1;
   }

   if(( rc = d.OpenDatabase( av[1] )) != XB_NO_ERROR ) {
     cout << "\nError " << rc << " opening file " << av[1] << endl;
     exit(0);
   }
   xbXBase x;
   xbDbf d( &x );

   cout << "\nGoing to lock database..." << endl;
   rc = d.LockDatabase( F_SETLKW, F_WRLCK, 1L );
   if ( rc == -1 ){
     perror("Lock Error");
     return 2;
   }
   cout << "Database locked\n\nEnter a keystroke to release lock\n";
   char xx[4];
   cin  >> xx;

   rc = d.LockDatabase( F_SETLK, F_UNLCK, 1L );
   cout << "Unlock database rc = " << rc << "\n";
   d.CloseDatabase();
#else
   cout << "\nXBASE_LOCKING_ON is not compiled in\n";
#endif
   return 0;
}
