/*  $Id: types.h,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TYPES.H
    Various useful definitions

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

#ifndef _TYPES_H_
#define _TYPES_H_

#include<assert.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<xbase.h>

#define YB_VERSION "0.1.1"

void yyerror(char *msg, ...);

// -- NULL-compatibile versions of some functions -------------------

#define NCstrdup(s)	  (s!=NULL?strdup(s):NULL)
#define NCstrlen(s)	  (s!=NULL?strlen(s):0)
#define NCstrcasecmp(a,b) (a==NULL?(b==NULL?0:1):(b==NULL?-1:strcasecmp(a,b)))
#define NCstrchr(s,a)	  (s!=NULL?strchr(s,a):NULL)
#define NCfree(s)	  if (s!=NULL) free(s)
#define NCdelete(s)	  if (s!=NULL) delete s

// possible data types
enum ybType {
  ybCharacter = 'C',
  ybNumeric = 'N',
  ybLogical = 'L',
  ybDate = 'D',
  ybUndefined = 'U'
}; // ybType

// possible identifier states
enum ybState {
  ybFunction,ybVariable
}; //ybState

// The main exception class
class yBaseExc {
public:
  yBaseExc(const char* msg,...) {
    char tmpmsg[100];
    va_list ap;
    va_start( ap, msg );
    vsprintf( tmpmsg, msg, ap );
    message = NCstrdup( tmpmsg );
    va_end( ap );
  }
  yBaseExc(const yBaseExc& exc) { message=NCstrdup( exc.message ); }
  ~yBaseExc() { NCfree( message ); }
  char* message;
};

// Special multitype which provides run-time type control
// For use in Fox++ programs
class ybValue {
public:
  // Constructors
  ybValue(const xbString&);
  ybValue(const double&);
  ybValue(const bool&);
  ybValue(const xbDate&);
  ybValue();
  ybValue(const ybValue&);
  // Assignments
  ybValue& operator = (const xbString&);
  ybValue& operator = (const double&);
  ybValue& operator = (const bool&);
  ybValue& operator = (const xbDate&);
  ybValue& operator = (const ybValue&);
  // Destructor
  ~ybValue();
  // Methods
  const ybType type() { return pType; }
  // Operators for type conversion
  operator xbString& ();
  operator double ();
  operator long ();
  operator bool ();
  operator xbDate& ();
  // Stream operators
  friend ostream& operator << ( ostream&, const ybValue& );
  // Numeric unary operators
  friend ybValue operator + ( const ybValue& );
  friend ybValue operator - ( const ybValue& );
  friend ybValue operator ! ( const ybValue& );
  // Numeric binary operators
  friend ybValue operator + ( const ybValue&, const ybValue& );
  friend ybValue operator - ( const ybValue&, const ybValue& );
  friend ybValue operator * ( const ybValue&, const ybValue& );
  friend ybValue operator / ( const ybValue&, const ybValue& );
  friend ybValue operator % ( const ybValue&, const ybValue& );
  friend ybValue operator && ( const ybValue&, const ybValue& );
  friend ybValue operator || ( const ybValue&, const ybValue& );
  friend ybValue in ( const ybValue&, const ybValue& ); /* operator '$' */
  // Comparing operators
  friend ybValue operator == ( const ybValue&, const ybValue& );
  friend ybValue operator != ( const ybValue&, const ybValue& );
  friend ybValue operator < ( const ybValue&, const ybValue& );
  friend ybValue operator > ( const ybValue&, const ybValue& );
  friend ybValue operator <= ( const ybValue&, const ybValue& );
  friend ybValue operator >= ( const ybValue&, const ybValue& );
private:
  ybType pType;	    // value type
  union {
    xbString* pChar;
    double pNum;
    bool pLogic;
    xbDate* pDate;
  };
}; // ybValue

// Error class, the main element of all syntactic structures
class ybEc {
public:
  ybEc();
  void error(const char *msg, ...);
protected:
  unsigned row;
  unsigned column;
}; // ybEc

// Possible operators
enum ybOper {
  addOp, subOp, mulOp, divOp, modOp,	// numerical operations
  andLop, orLop, notLop,                // logical operations
  equOp, neqOp,                         // equivalency
  lthOp, gthOp, leqOp, geqOp, subsOp    // porovn v n¡
}; // ybOper

const char* nameof(ybType);
const char* nameof(ybState);

class ybIdeVar;
class ybIdeFun;
class ybCom;
class ybExp;
class ybExpLst;

typedef ybValue(*ybFcePtr)(ybValue[]);

# endif

// end of file

