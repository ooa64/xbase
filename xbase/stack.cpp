/*  $Id: stack.cpp,v 1.6 2002/04/04 22:58:42 dbryson Exp $

    Xbase project source code

    This file contains logic for handling basic stack functions.

    Copyright (C) 1997 Startech, Gary A. Kunkel

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact:

      Mail:

        Technology Associates, Inc.
        XBase Project
        1455 Deming Way #11
        Sparks, NV 89434
        USA

      Email:

        xbase@techass.com

      See our website at:

        xdb.sourceforge.net


    V 1.0   10/10/97   - Initial release of software
    V 1.5   1/2/97     - Added memo field support
    V 1.6a  4/1/98     - Added expression support
    V 1.6b  4/8/98     - Added expression support
*/

#ifdef __GNUG__
  #pragma implementation "xstack.h"
#endif

#ifdef __WIN32__
#include <xbase/xbconfigw32.h>
#else
#include <xbase/xbconfig.h>
#endif

#include "xbase/xbase.h"

#include <string.h>
#include <stdlib.h>
#include <iostream.h>

#include <xbase/xstack.h>

/*! \file stack.cpp
*/

/*************************************************************************/
//! Constructor.
/*!
*/
xbStackElement::xbStackElement()
{
  Next = 0;
  Previous = 0;
  UserPtr = 0;
}

/*************************************************************************/
//! Destructor.
/*!
*/
xbStackElement::~xbStackElement()
{
}

/*************************************************************************/
//! Constructor.
/*!
*/
xbStack::xbStack( void )
{
  First      = 0;
  Last       = 0;
  StackDepth = 0;
}

/*************************************************************************/
//! Destructor.
/*!
*/
xbStack::~xbStack( void )
{
  InitStack();
}

/*************************************************************************/
//! Short description.
/*!
*/
void xbStack::InitStack( void )
{
  xbStackElement
    *next;

  while(First)
  {
    next = First->Next;
    delete First;
    First = next;
  }

  First = 0;
  Last  = 0;
  StackDepth = 0;

  return;
}

/*************************************************************************/
//! Push a value onto the stack.
/*!
  \param p
*/
xbShort
xbStack::Push(void *p)
{
 xbStackElement
   *Temp = new xbStackElement;

 if(!Temp)
   return XB_NO_MEMORY;

 Temp->UserPtr = p;

 if( !First )
 {
   First = Temp;
   Last  = Temp;
   StackDepth = 1;
 }
 else
 {
   Last->Next = Temp;
   Temp->Previous = Last;
   Last = Temp;
   StackDepth++;
 }

 return XB_NO_ERROR;
}

/*************************************************************************/
//! Pop the top value from the stack.
/*!
*/
void *
xbStack::Pop()
{
  void
    *p;

  xbStackElement
    *Save;

  if(StackDepth == 0)
     return 0;
  else
  {
    p = Last->UserPtr;
    if( StackDepth == 1 )
    {
      delete First;
      First = 0;
      Last  = 0;
    }
    else  /* number of items in Stack must be > 1 */
    {
      Last->Previous->Next = 0;
      Save = Last;
      Last = Last->Previous;
      delete Save;
    }
    StackDepth--;
    return p;
  }
}

/*************************************************************************/
//! Short description.
/*!
*/
#ifdef XBASE_DEBUG
void xbStack::DumpStack( void )
{
   xbStackElement * e;
   if( StackDepth == 0 )
   {
      cout << "\nStack is empty...";
      return;
   }

   cout << "\nThere are " << StackDepth << " entries.";
   cout << "\nFirst = " << First << "  Last = " << Last;

   e = First;
   while( e )
   {
      cout << "\n*****************************";
      cout << "\nThis      = " << e;
      cout << "\nNext      = " << e->Next;
      cout << "\nPrevious  = " << e->Previous;
      cout << "\nUser Pointer = " << e->UserPtr;
      e = e->Next;
   }
   return;
}
#endif
/*************************************************************************/
