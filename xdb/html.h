/*  $Id: html.h,v 1.3 2000/06/27 04:44:07 dbryson Exp $

    Xbase project source code
 
    This file contains a header file for the HTML object which is used
    for HTML generation.

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

    V 1.0    10/10/97   - Initial release of software
    V 1.5    1/2/98     - Added memo field support
    V 1.6a   4/1/98     - Added expression support
    V 1.6b   4/8/98     - Numeric index keys
    V 1.7.4c 10/26/98   - Added GenFormFields, SetCookie, GetCookie
    V 1.8    11/29/98   - Version 1.8 upgrade
*/

#ifndef __XB_HTML_H__
#define __XB_HTML_H__

#include <xdb/xbconfig.h>

#include <stdlib.h>
#include <string.h>

#include <xdb/xtypes.h>

/*! \file html.h
*/

//! xbFieldList struct
/*!
*/

struct xbFieldList{
   char * Label;                     /* field label on form */
   char * FieldName;                 /* form field name     */
   xbShort FieldLen;                   /* form display length */
   xbShort FieldNo;                    /* database field no   */
   xbShort Option;                     /* field option        */
};

typedef char **xbArrayPtr;

//! xbHtml class
/*!
*/
class XBDLLEXPORT xbHtml {
public:
   xbHtml  ( void );
   //! Short description.
   /*!
   */
   void   BoldOff( void ) { cout << "</b>\n"; };
   //! Short description.
   /*!
   */
   void   BoldOn( void ) { cout << "<b>"; };
   //! Short description.
   /*!
   */
   void   Bullet( void ) { cout << "<li>"; };
   void   DumpArray( void );
   //! Short description.
   /*!
   */
   void   EmphasizeOff( void ) { cout << "</em>\n"; };
   //! Short description.
   /*!
   */
   void   EmphasizeOn( void ) { cout << "<em>"; };
   //! Short description.
   /*!
   */
   void   EndHtmlPage( void ) { cout << "</BODY>\n</HTML>\n"; }
   xbShort  GenFormFields(xbDbf *d, xbShort Option,const char * Title,xbFieldList *fl);
   xbShort  GetArrayNo( const char * FieldName );
   const  char * GetCookie( const char *CookieName );
   char * GetData( xbShort );
   char * GetDataForField( const char *FieldName );
   char * GetEnv( char * s ){ return getenv( s ); }
   xbShort  GetMethod( void );
   //! Short description.
   /*!
   */
   void   HeaderOff( xbShort i ){ cout << "</h" << i << ">\n"; };
   //! Short description.
   /*!
   */
   void   HeaderOn( xbShort i ){ cout << "<h" << i << ">\n"; };
   //! Short description.
   /*!
   */
   void   ItalicOff( void ) { cout << "</i>\n"; };
   //! Short description.
   /*!
   */
   void   ItalicOn( void ) { cout << "<i>"; };
   //! Short description.
   /*!
   */
   void   NewLine( void ) { cout << "<br>"; }
   xbShort  PostMethod( void );
   void   PrintEncodedChar( char );
   void   PrintEncodedString( const char *s );
   //! Short description.
   /*!
   */
   void   PrintHtml( char * s ) { cout << s; };
   //! Short description.
   /*!
   */
   void   PrintHtml( xbLong l ) { cout << l; };
   //! Short description.
   /*!
   */
   void   PrintHtml( xbShort i ) { cout << i; };
   //! Short description.
   /*!
   */
   void   PrintHtml( int i ) { cout << i; };
   void   StartHtmlPage( const char *Title );
   //! Short description.
   /*!
   */
   void   StartTextPage( void ) { cout << "Content-type: text/plain\n\n"; }
   void   TextOut( const char *String );
   xbLong   Tally( const char *FileName );
   xbShort  SetCookie(const char *Name, const char *Value, const char *ExpDate,
           const char *ExpTime,const char *TimeZone, const char *Path,
           const char *Domain, xbShort Secure );
   void   SpaceToPlus( char * );
   void   PlusToSpace( char * );
   void   SendRedirect( char * ) const;

private:
   xbArrayPtr FieldNameArray;
   xbArrayPtr DataValueArray;
   xbShort    NoOfDataFields;
   char     * HtmlWorkBuf;
   xbShort    HtmlBufLen;
   void     LoadArray( void );
   void     DeleteEscChars( char *String );
   void     InitVals( void );
};

#endif		// __XB_HTML_H__
