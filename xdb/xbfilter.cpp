/*  $Id: xbfilter.cpp

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
    V 1.8.x 3/29/99    - Initial support for filters added
*/

#include <xdb/xbase.h>
#include <xdb/xbexcept.h>

/*! \file xbfilter.cpp
*/

#ifdef XB_FILTERS
/************************************************************************/
xbFilter::xbFilter( xbDbf * dbf, xbIndex * index, char * exp )
{
  xbShort rc;
  Status = 0;
  CurFilterRecNo = 0L;
  d = dbf;
  i = index;

  if(( rc = d->xbase->ParseExpression( exp, d )) != XB_NO_ERROR )
    Status = rc;
  else{
    e = d->xbase->GetExpressionHandle();
    if( d->xbase->GetExpressionResultType( e ) != 'L' )
      Status = XB_PARSE_ERROR;
  }
}
/***********************************************************************/
xbShort xbFilter::GetFirstFilterRec()
{
  xbShort rc;

  if( Status ) 
    return Status;

  if( i )
    rc = i->GetFirstKey();
  else
    rc = d->GetFirstRecord();

  while( rc == XB_NO_ERROR ){
    if(( rc = d->xbase->ProcessExpression( e )) != XB_NO_ERROR )
      xb_error( rc );

  cout << "xbfilter fixme" << endl;
//    if( d->xbase->GetInt()){
      CurFilterRecNo = d->GetCurRecNo();
      return XB_NO_ERROR;
//    }
    if( i )
      rc = i->GetNextKey();
    else
      rc = d->GetNextRecord();
  }
  return rc;
}
/***********************************************************************/
xbShort xbFilter::GetLastFilterRec()
{
  xbShort rc;

  if( Status ) 
    return Status;

  if( i )
    rc = i->GetLastKey();
  else
    rc = d->GetPrevRecord();

  while( rc == XB_NO_ERROR ){
    if(( rc = d->xbase->ProcessExpression( e )) != XB_NO_ERROR )
      xb_error( rc );

cout << "xbfilter fixme" << endl;
//    if( d->xbase->GetInt()){
      CurFilterRecNo = d->GetCurRecNo();
      return XB_NO_ERROR;
//    }
    if( i )
      rc = i->GetPrevKey();
    else
      rc = d->GetPrevRecord();
  }
  return rc;
}
/***********************************************************************/
xbShort xbFilter::GetNextFilterRec()
{
  xbShort rc;

  if( Status ) 
    return Status;

  if( !CurFilterRecNo )
    return GetFirstFilterRec();

  if( i ){
    rc = i->GetNextKey();
  }
  else
    rc = d->GetNextRecord();

  while( rc == XB_NO_ERROR ){
    if(( rc = d->xbase->ProcessExpression( e )) != XB_NO_ERROR )
      xb_error( rc );

    cout << "xbfilter fix me" << endl;
//    if( d->xbase->GetInt()){
      CurFilterRecNo = d->GetCurRecNo();
      return XB_NO_ERROR;
//    }
    if( i )
      rc = i->GetNextKey();
    else
      rc = d->GetNextRecord();
  }
  return rc;
} 
/***********************************************************************/
xbShort xbFilter::GetPrevFilterRec()
{
  xbShort rc;

  if( Status ) 
    return Status;

  if( !CurFilterRecNo )
    return GetLastFilterRec();

  if( i ){
    rc = i->GetPrevKey();
  }
  else
    rc = d->GetPrevRecord();

  while( rc == XB_NO_ERROR ){
    if(( rc = d->xbase->ProcessExpression( e )) != XB_NO_ERROR )
      xb_error( rc );
   cout << "xbfilter fix me" << endl;
  //  if( d->xbase->GetInt()){
      CurFilterRecNo = d->GetCurRecNo();
      return XB_NO_ERROR;
//    }
    if( i )
      rc = i->GetPrevKey();
    else
      rc = d->GetPrevRecord();
  }
  return rc;
} 
/***********************************************************************/
#endif  // XB_FILTERS_ON
