/*  $Id: xstack.h,v 1.1 2000/09/20 19:45:31 dbryson Exp $

    Xbase project source code
  
    This file conatains a header file for the xbStack object which
    is used for handling expressions.

    Copyright (C) 1997  Startech, Gary A. Kunkel   
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
    V 1.5   1/2/97     - Added memo field support
    V 1.6a  4/1/98     - Added expression support
    V 1.6b  4/8/98     - Numeric index keys 
*/

/*! \file xstack.h
*/

#ifndef __XB_STACK_H__
#define __XB_STACK_H__

#include <xdb/xtypes.h>

//! xbStackElement struct
/*!
*/

struct xbStackElement{
   xbStackElement * Previous;
   xbStackElement * Next;
   void * UserPtr;
};

//! xbStack class
/*!
*/

class XBDLLEXPORT xbStack{

 public:
   xbStack( void );
   void    InitStack( void );
   void *  Pop( void );
   xbShort Push( void * );
   //! Short description.
   /*!
   */
   xbShort GetStackDepth( void ) { return StackDepth; }
   void    DumpStack( void );

 private:
   xbShort StackDepth;
   xbStackElement * First;
   xbStackElement * Last;
   xbStackElement * Free;          /* points to free stack items */
   xbStackElement * GetStackElement( void );
   void FreeStackElement( xbStackElement * );
};

#endif               // __XB_STACK_H__
