/*  $Id: table.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TABLE.CPP
    Implementation of the symbol and database table

    Copyright (C) 1999  Petr Gotthard
    email - petr.gotthard@seznam.cz

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

    V 0.1   20/09/99   - First prerelease
*/

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <xbase/xbexcept.h>
#include "lang.h"
#include "table.h"

// ** Symbol table **************************************************

// -- Generic entry of the symbol table -----------------------------

// constructor from the indentifier name
ybIde::ybIde(char *iName, ybState iState) {
  pName = NCstrdup(iName);
  pState = iState;

  idelft = NULL;
  idergt = NULL;
} // ybIde::ybIde

// destructor
ybIde::~ybIde() {
  NCfree(pName);

  NCdelete(idelft);
  NCdelete(idergt);
} // ybIde::~ybIde

// -- Entry for variables -------------------------------------------

ybIdeVar::ybIdeVar(char *aName,int aConst):ybIde(aName,ybVariable),value() {
  constant = aConst;
}

ybIdeVar::~ybIdeVar() {
}

// Debugging output
int ybIdeVar::dump(void) {
  printf("%s %s",nameof(pState),pName);
  switch (value.type()) {
    case ybLogical: printf(" = %s",(bool)value?".T.":".F."); break;
    case ybNumeric: printf(" = %G",(double)value); break;
    case ybCharacter: printf(" = %s",(const char*)(xbString)value); break;
    case ybUndefined: printf(" = Undefined"); break;
    default: { }
  }
  printf(" [%s%s]\n",constant?"constant ":"",nameof(value.type()));
  if( idelft!=NULL ) idelft->dump();
  if( idergt!=NULL ) idergt->dump();
  return 0;
}

// -- Function prototype --------------------------------------------

ybFunPro::ybFunPro(char* aType, ybFcePtr aPtr, ybFunPro* aNext) {
  proprm = strdup(aType);
  proptr = aPtr;
  pronxt = aNext;
}

ybFunPro::~ybFunPro() {
  NCfree(proprm);
  NCdelete(pronxt);
}

// Debugging output
int ybFunPro::dump(ybIdeFun *fun) {
  printf("%s %s(%s)\n",nameof(fun->state()),fun->name(),proprm);
  if( pronxt!=NULL ) pronxt->dump(fun);
  return 0;
}

// -- Entry for functions -------------------------------------------

ybIdeFun::ybIdeFun(char *aName):ybIde(aName,ybFunction) {
  funpro = NULL;
}

ybIdeFun::~ybIdeFun() {
  NCdelete(funpro);
}

// Debugging output
int ybIdeFun::dump(void) {
  if( funpro!=NULL ) funpro->dump(this);
  if( idelft!=NULL ) idelft->dump();
  if( idergt!=NULL ) idergt->dump();
  return 0;
}

// -- The symbol table, binary tree ---------------------------------

// Constructor of the table
ybSymTab::ybSymTab() {
  root = NULL;
} // ybSymTab::ybSymTab

// Recursive delete of the whole table
ybSymTab::~ybSymTab() {
  NCdelete( root );
} // ybSymTab::~ybSymTab

// Searches an identifier in the table
ybIde **ybSymTab::search(char *name, ybState state) {
  ybIde **ide = &root;
  while( (*ide) != NULL ) {
    if( (*ide)->pState < state ) ide = &((*ide)->idelft);
    else if( (*ide)->pState > state ) ide = &((*ide)->idergt);
    if( NCstrcasecmp((*ide)->pName,name) < 0 ) ide = &((*ide)->idelft);
    else if( NCstrcasecmp((*ide)->pName,name) > 0 ) ide = &((*ide)->idergt);
    else return ide; // found, *ide points into the table
  }
  return ide; // not found, *ide == NULL
}

// Deletes an identifier from the table
void ybSymTab::free(ybIde **ide) {
  ybIde** newide = ide;
  if( (*newide)->idelft == NULL ) {
    // this node has only right subtree
    (*newide) = (*ide)->idergt;
    (*ide)->idergt = NULL;
  } else if( (*ide)->idergt == NULL ) {
    // this node has only left subtree
    (*newide) = (*ide)->idelft;
    (*ide)->idelft = NULL;
  } else {
    // this node has both subtrees
    // look for the rightmost node in the left subtree
    ybIde** rptr = &((*newide)->idelft);
    while( (*rptr)->idergt != NULL ) rptr = &((*rptr)->idergt);
    // replace deleted node with rightmost node
    (*newide) = *rptr;
    (*rptr) = (*rptr)->idelft;
    // connect both subtrees
    (*newide)->idelft = (*ide)->idelft;
    (*newide)->idergt = (*ide)->idergt;
    // clear pointers to the subtrees (due to recursive destructor)
    (*ide)->idelft = NULL;
    (*ide)->idergt = NULL;
  }
  // call destructor
  delete (*ide);
  (*ide) = NULL;
}

// Debugging output
int ybSymTab::dump(void) {
  printf("\nContent of the symbol table\n");

  if( root!=NULL ) root->dump();
  else printf("(empty)\n");
  return 0;
}

// The main symbol table
ybSymTab *YBsymtab = NULL;

// ** Database table ************************************************

// -- Entry of the database table -----------------------------------

ybDbf::ybDbf(xbXBase* xb) : xbDbf(xb) {
  alias = NULL;
}

ybDbf::~ybDbf() {
  // close already opened database
  if( GetDbfStatus() != XB_CLOSED ) {
    CloseDatabase();
    NCfree(alias);
  }
}

xbShort ybDbf::OpenDatabase(ybValue& inname, ybValue& inalias) {
  xbShort result = 0;

  // close already opened database
  if( GetDbfStatus() != XB_CLOSED ) {
    CloseDatabase();
    NCfree(alias);
  }

  switch( inname.type() ) {
    case ybCharacter: {
      const char* filename = (xbString&)inname;
      // open new database
      result = xbDbf::OpenDatabase( filename );
      if( result != XB_NO_ERROR ) throw yBaseExc(xbStrError(result));

      // create new alias name for this workspace
      switch( inalias.type() ) {
        case ybCharacter: {
          const char* myalias = (xbString&)inalias;
          alias = strdup(myalias);
          break;
        }
        case ybUndefined: {
          const char* filename = (xbString&)GetDbfName();
          // extract path
          const char* name = strrchr(filename,PATH_SEPARATOR);
          if( name == NULL ) name = filename;
          else name++;
          // extract extension
          size_t leng = strlen(name)-4;
          alias = (char*)malloc(leng+1);
          memcpy(alias,name,leng);
          alias[leng] = '\x0';
          break;
        }
        default:
          throw yBaseExc(YBstrings[YB_COMNOWORK],"ALIAS",nameof( inalias.type() ));
      } /* switch */
      break;
    }
    case ybUndefined:
      break;
    default:
      throw yBaseExc(YBstrings[YB_COMNOWORK],"USE",nameof( inname.type() ));
  }

  return result;
}

ybValue ybDbf::GetField( xbShort num ) {
  ybValue outval;

  char* tmpval = (char*)malloc( GetFieldLen(num)+1 );
  xbDbf::GetField( num, tmpval );

  switch( GetFieldType(num) ) {
    case 'c': case 'C':
      outval = xbString( tmpval );
      break;
    case 'n': case 'N':
      outval = atof( tmpval );
      break;
    case 'l': case 'L':
      switch( tmpval[0] ) {
        case 'y': case 'Y': case 't': case 'T': outval = true; break;
        case 'n': case 'N': case 'f': case 'F': outval = false; break;
        default: break; // result will be NAN
      }
      break;
    case 'd': case 'D':
      outval = xbDate( tmpval );
      break;
    default:
      break;
  }

  NCfree(tmpval);
  return outval;
}

xbShort ybDbf::PutField( xbShort num, ybValue& val ) {
  ybType fietyp = (ybType)toupper( GetFieldType(num) );

  switch( fietyp ) {
    case ybCharacter:
      switch( val.type() ) {
        case ybCharacter: {
          xbString inval = val;
          return xbDbf::PutField( num, (const char*)inval );
        }
        case ybUndefined: {
          return XB_NO_ERROR;
        }
        default: throw yBaseExc(YBstrings[YB_ASGTMIS],nameof( val.type() ),nameof( fietyp ),GetFieldName(num));
      } /* switch */
      break;
    case ybNumeric:
      switch( val.type() ) {
        case ybNumeric: {
          double inval = val;
          return xbDbf::PutDoubleField( num, inval );
        }
        case ybUndefined: {
          return XB_NO_ERROR;
        }
        default: throw yBaseExc(YBstrings[YB_ASGTMIS],nameof( val.type() ),nameof( fietyp ),GetFieldName(num));
      } /* switch */
      break;
    case ybLogical:
      switch( val.type() ) {
        case ybLogical: {
          if( (bool)val ) return xbDbf::PutField( num, "T" );
          else return xbDbf::PutField( num, "F" );
        }
        case ybUndefined: {
          return xbDbf::PutField( num, "?" );
        }
        default: throw yBaseExc(YBstrings[YB_ASGTMIS],nameof( val.type() ),nameof( fietyp ),GetFieldName(num));
      } /* switch */
      break;
    case ybDate:
      switch( val.type() ) {
        case ybDate: {
          xbDate inval = val;
          return xbDbf::PutField( num, (const char*)inval.GetDate() );
        }
        case ybUndefined: {
          return XB_NO_ERROR;
        }
        default: throw yBaseExc(YBstrings[YB_ASGTMIS],nameof( val.type() ),nameof( fietyp ),GetFieldName(num));
      } /* switch */
      break;
    default:
      return XB_NO_ERROR;
  }
}

// -- The database table, ordered list ------------------------------

ybDbfTab::ybDbfTab(xbXBase& xb) : xbase(xb) {
  for( int i=0; i < YB_DBFMAX; i++ )
    dbfs[i] = new ybDbf(&xb);

  // set active workspace
  active = 1;
}

ybDbfTab::~ybDbfTab() {
  for( int i=0; i < YB_DBFMAX; i++ )
    NCdelete(dbfs[i]);
}

// return pointer to database in the active workspace
ybDbf* ybDbfTab::dbf() {
  // active is within interval 1..YB_DBFMAX
  return dbfs[active-1];
}

// return pointer to database in the given workspace
ybDbf* ybDbfTab::dbf( long num ) {
  // num MUST be within interval 1..YB_DBFMAX
  return dbfs[num-1];
}

// return pointer to database
ybDbf* ybDbfTab::dbf( char *alias ) {
  long area;
  // find database with the given alias
  for( area = 1; area <= YB_DBFMAX; area++ )
    if( NCstrcasecmp( dbfs[area-1]->GetDbfAlias(), alias ) == 0 ) break;
  // success?
  if( area > YB_DBFMAX ) throw yBaseExc(YBstrings[YB_ALINDEF], alias);
  return dbfs[area-1];
}

long ybDbfTab::select(ybValue& data) {
  long area = active;

  switch( data.type() ) {
    case ybCharacter: {
      const char* alias = (xbString&)data;
      // find database with the given alias
      for( area = 1; area <= YB_DBFMAX; area++ )
        if( NCstrcasecmp( dbfs[area-1]->GetDbfAlias(), alias ) == 0 ) break;
      // success?
      if( area > YB_DBFMAX ) throw yBaseExc(YBstrings[YB_ALINDEF], alias);
      break;
    }
    case ybNumeric:
      area = (long)data;
      // test if the workspace number is 0 or 1..YB_DBFMAX
      if( area < 0 || area > YB_DBFMAX )
        throw yBaseExc(YBstrings[YB_NOWRKSP], area);

      if( area == 0 ) {
        // find the lowest unused workarea
        for( area = 1; area <= YB_DBFMAX; area++ )
          if( dbfs[area-1]->GetDbfStatus() == 0 ) break;
        // was found any free workspace?
        if( area > YB_DBFMAX ) throw yBaseExc(YBstrings[YB_NOFRESP]);
      }
      break;
    case ybUndefined:
      break;
    default:
      // Command ... doesn't work with type ...
      throw yBaseExc(YBstrings[YB_COMNOWORK], "SELECT", nameof( data.type() ));
  }
  // active is within interval 1..YB_DBFMAX
  return( active = area );
}

long ybDbfTab::select(void) {
  // active is within interval 1..YB_DBFMAX
  return( active );
}

// The main database table
ybDbfTab *YBdbftab = NULL;

// end of file

