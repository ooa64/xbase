/*  $Id: table.h,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TABLE.H
    Definition of the symbol and database table

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

#ifndef _TABLE_H_
#define _TABLE_H_

#include "types.h"
#include "funct.h"

// ** Symbol table **************************************************

// Generic entry of the symbol table
class ybIde {
public:
  ybIde(char*,ybState);
  virtual ~ybIde();
  virtual int dump(void) = 0;

  const char* name() { return pName; }
  const ybState state() { return pState; }
  friend class ybSymTab;
protected:
  ybState pState;  // identifier state
  char* pName;     // identifier name
  ybIde* idelft;   // pointer to the left entry
  ybIde* idergt;   // pointer to the right entry
}; // ybIde

// Function prototype
class ybFunPro {
public:
  ybFunPro(char*, ybFcePtr, ybFunPro*);
  virtual ~ybFunPro();
  virtual int dump(ybIdeFun*);

  char* proprm;     // types of formal parameters
  ybFcePtr proptr;  // pointer to the implementation
  ybFunPro* pronxt; // next prototype
}; // ybFunPro

// Function entry of the symbol table
class ybIdeFun : public ybIde {
public:
  ybIdeFun(char*);
  virtual ~ybIdeFun();
  virtual int dump(void);

  ybFunPro* funpro; // pointer to the prototypes
}; // ybIdeFun

// Variable entry of the symbol table
class ybIdeVar : public ybIde {
public:
  ybIdeVar(char*,int);
  virtual ~ybIdeVar();
  virtual int dump(void);

  int constant;   // can be value changed?
  ybValue value;  // actual value
}; // ybIdeVar

// The symbol table, binary tree
class ybSymTab {
public:
  ybSymTab();
  ~ybSymTab();
  ybIde **search(char*,ybState);
  void free(ybIde**);
  int dump(void);
protected:
  ybIde *root;      // root of the table
}; //ybSymTab

// The main symbol table
extern ybSymTab *YBsymtab;

// ** Database table ************************************************

// Number of workspaces
#define YB_DBFMAX 25

// Entry of the database table
class ybDbf : public xbDbf {
public:
  ybDbf( xbXBase* );
  virtual ~ybDbf();
  xbShort OpenDatabase( ybValue&, ybValue& );
  char* GetDbfAlias( void ) { return alias; }
  ybValue GetField( xbShort );
  xbShort PutField( xbShort, ybValue& );
protected:
  char* alias;
}; // ybDbf

// The database table, ordered list
class ybDbfTab {
public:
  ybDbfTab( xbXBase& );
  ~ybDbfTab();
  // get database
  ybDbf* dbf();
  ybDbf* dbf( long );
  ybDbf* dbf( char* );
  // get-set active workspace
  long select( ybValue& );
  long select( void );
protected:
  xbXBase& xbase;
  long active;
  ybDbf* dbfs[YB_DBFMAX];
}; // ybDbfTab

// The main database table
extern ybDbfTab *YBdbftab;

# endif

// end of file

