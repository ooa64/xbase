/*  $Id: types.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TYPES.CPP
    Various useful implementations

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
#include <stdarg.h>
#include <iomanip.h>
#include <math.h>
#include <assert.h>
#include "lang.h"
#include "types.h"

extern int yylineno;   // line number, position in the LEX's input file
extern char *FileName; // name of the input file

// show an error message, for use only within LEX and BISON code
void yyerror(char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  fprintf(stderr, YBstrings[YB_ERROR], FileName, yylineno);
  vfprintf(stderr, msg, ap);
  fputc('\n', stderr);
  va_end(ap);
}

// -- Representation of a value with type ---------------------------

// character value constructor
ybValue::ybValue(const xbString& aValue) {
  pType = ybCharacter;
  pChar = new xbString( aValue );
}

// numeric value constructor
ybValue::ybValue(const double& aValue) {
  pType = ybNumeric;
  pNum = aValue;
}

// logical value constructor
ybValue::ybValue(const bool& aValue) {
  pType = ybLogical;
  pLogic = aValue;
}

// date value constructor
ybValue::ybValue(const xbDate& aValue) {
  pType = ybDate;
  pDate = new xbDate( aValue );
}

// undefined value constructor
ybValue::ybValue() {
  pType = ybUndefined;
}

// copy constructor (character values ARE duplicated)
ybValue::ybValue(const ybValue& value) {
  switch( pType = value.pType ) {
    case ybCharacter: pChar = new xbString( *value.pChar ); break;
    case ybNumeric: pNum = value.pNum; break;
    case ybLogical: pLogic = value.pLogic; break;
    case ybDate: pDate = new xbDate( *value.pDate ); break;
    default: break;
  }
}

// destructor
ybValue::~ybValue() {
  switch( pType ) {
    case ybCharacter: NCdelete( pChar ); break;
    case ybDate: NCdelete( pDate ); break;
    default: break;
  }
}

// character assignment operator (date ARE duplicated)
ybValue& ybValue::operator = (const xbString& value) {
  // free current value
  switch( pType ) {
    case ybCharacter: NCfree( pChar ); break;
    case ybDate: NCdelete( pDate ); break;
    default: break;
  }
  // assign new value
  pType = ybCharacter;
  pChar = new xbString( value );

  return *this;
}

// numeric assignment operator
ybValue& ybValue::operator = (const double& value) {
  // free current value
  switch( pType ) {
    case ybCharacter: NCfree( pChar ); break;
    case ybDate: NCdelete( pDate ); break;
    default: break;
  }
  // assign new value
  pType = ybNumeric;
  pNum = value;

  return *this;
}

// logical assignment operator
ybValue& ybValue::operator = (const bool& value) {
  // free current value
  switch( pType ) {
    case ybCharacter: NCfree( pChar ); break;
    case ybDate: NCdelete( pDate ); break;
    default: break;
  }
  // assign new value
  pType = ybLogical;
  pLogic = value;

  return *this;
}

// date assignment operator
ybValue& ybValue::operator = (const xbDate& value) {
  // free current value
  switch( pType ) {
    case ybCharacter: NCfree( pChar ); break;
    case ybDate: NCdelete( pDate ); break;
    default: break;
  }
  // assign new value
  pType = ybDate;
  pDate = new xbDate( value );

  return *this;
}

// Value assignment operator (character values ARE duplicated)
ybValue& ybValue::operator = (const ybValue& value) {
  // free current value
  switch( pType ) {
    case ybCharacter: NCfree( pChar ); break;
    case ybDate: NCdelete( pDate ); break;
    default: break;
  }
  // assign new value
  switch( pType = value.pType ) {
    case ybCharacter: pChar = new xbString( *value.pChar ); break;
    case ybNumeric: pNum = value.pNum; break;
    case ybLogical: pLogic = value.pLogic; break;
    case ybDate: pDate = new xbDate( *value.pDate ); break;
    default: break;
  }
  return *this;
}

// Operators for type conversion

ybValue::operator xbString& () {
  if( pType != ybCharacter ) throw yBaseExc("Type conversion error");
  return *pChar;
}

ybValue::operator double () {
  if( pType != ybNumeric ) throw yBaseExc("Type conversion error");
  return pNum;
}

ybValue::operator long () {
  if( pType != ybNumeric ) throw yBaseExc("Type conversion error");
  return (long)floor(pNum+0.5);
}

ybValue::operator bool () {
  if( pType != ybLogical ) throw yBaseExc("Type conversion error");
  return pLogic;
}

ybValue::operator xbDate& () {
  if( pType != ybDate ) throw yBaseExc("Type conversion error");
  return *pDate;
}

// Stream operators

ostream& operator << ( ostream& s, const ybValue& v ) {
  switch( v.pType ) {
    case ybCharacter: s << *v.pChar; break;
    case ybNumeric: s << setprecision(15) << v.pNum; break;
    case ybLogical: s << ( v.pLogic ? ".T." : ".F." ); break;
    case ybDate: s << v.pDate->GetDate(); break;
    default: /* s << "undefined"; */ break;
  }
  return s;
}

// Numeric unary operators

ybValue operator + (const ybValue& inlft) {
  switch ( inlft.pType ) {
    case ybNumeric: return (double)( inlft.pNum );
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"+",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator - (const ybValue& inlft) {
  switch ( inlft.pType ) {
    case ybNumeric: return (double)( - inlft.pNum );
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"-",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator ! (const ybValue& inlft) {
  switch ( inlft.pType ) {
    case ybLogical: return (bool)( ! inlft.pLogic );
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],".NOT.",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

// Numeric binary operators

ybValue operator + (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (xbString)( *inlft.pChar + *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"+");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (double)( inlft.pNum + inrgt.pNum );
        // left and right side commuted due to operator +
        case ybDate: return (xbDate)( *inrgt.pDate + inlft.pNum);
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"+");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybNumeric: return (xbDate)( *inlft.pDate + inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"+");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"+",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator - (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (xbString)( *inlft.pChar - *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"-");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (double)( inlft.pNum - inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"-");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybNumeric: return (xbDate)( *inlft.pDate - inrgt.pNum );
        case ybDate: return (double)( *inlft.pDate - *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"-");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"-",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator * (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (double)( inlft.pNum * inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"*");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"*",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator / (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric:
          if( inrgt.pNum == 0 ) throw yBaseExc(YBstrings[YB_DIVZERO]);
          return (double)( inlft.pNum / inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"/");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"/",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator % (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric:
          if( inrgt.pNum == 0 ) throw yBaseExc(YBstrings[YB_DIVZERO]);
          return fmod( inlft.pNum, inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"%");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"%",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator && (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybLogical:
      switch ( inrgt.pType ) {
        case ybLogical: return (bool)( inlft.pLogic && inrgt.pLogic );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),".AND.");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],".AND.",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator || (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybLogical:
      switch ( inrgt.pType ) {
        case ybLogical: return (bool)( inlft.pLogic || inrgt.pLogic );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),".OR.");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],".OR.",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue in (const ybValue& inlft, const ybValue& inrgt) { /* operator '$' */
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( inrgt.pChar->pos(*inlft.pChar) >= 0 );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"$");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"$",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

// Comparing operators

ybValue operator == (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( *inlft.pChar == *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"=");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (bool)( inlft.pNum == inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"=");
      }
      break;
    case ybLogical:
      switch ( inrgt.pType ) {
        case ybLogical: return (bool)( inlft.pLogic == inrgt.pLogic );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"=");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybDate: return (bool)( *inlft.pDate == *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"=");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"=",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator != (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( *inlft.pChar != *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<>");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (bool)( inlft.pNum != inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<>");
      }
      break;
    case ybLogical:
      switch ( inrgt.pType ) {
        case ybLogical: return (bool)( inlft.pLogic != inrgt.pLogic );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"==");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybDate: return (bool)( *inlft.pDate != *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<>");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"<>",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator < (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( *inlft.pChar < *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (bool)( inlft.pNum < inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybDate: return (bool)( *inlft.pDate < *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"<",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator > (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( *inlft.pChar > *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),">");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (bool)( inlft.pNum > inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),">");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybDate: return (bool)( *inlft.pDate > *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),">");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],">",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator <= (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( *inlft.pChar <= *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<=");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (bool)( inlft.pNum <= inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<=");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybDate: return (bool)( *inlft.pDate <= *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),"<=");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],"<=",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

ybValue operator >= (const ybValue& inlft, const ybValue& inrgt) {
  switch ( inlft.pType ) {
    case ybCharacter:
      switch ( inrgt.pType ) {
        case ybCharacter: return (bool)( *inlft.pChar >= *inrgt.pChar );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),">=");
      }
      break;
    case ybNumeric:
      switch ( inrgt.pType ) {
        case ybNumeric: return (bool)( inlft.pNum >= inrgt.pNum );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),">=");
      }
      break;
    case ybDate:
      switch ( inrgt.pType ) {
        case ybDate: return (bool)( *inlft.pDate >= *inrgt.pDate );
        case ybUndefined: break;
        default: throw yBaseExc(YBstrings[YB_TYPINCOM],nameof(inlft.pType),nameof(inrgt.pType),">=");
      }
      break;
    case ybUndefined: break;
    default: throw yBaseExc(YBstrings[YB_OPNOWORK],">=",nameof(inlft.pType));
  } /* switch */
  return ybValue();
}

// -- Main element of all syntactic structures ----------------------

ybEc::ybEc() {
  row = yylineno;
  column = 0; // TODO: add column counting
}

void ybEc::error(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  fprintf(stderr, YBstrings[YB_ERROR], FileName, row);
  vfprintf(stderr,msg, ap);
  fputc('\n',stderr);
  va_end(ap);
}

// -- Getting informations about values -----------------------------

const char* nameof( ybType typ ) {
  switch( typ ) {
    case ybCharacter: return "Character";
    case ybNumeric: return "Numeric";
    case ybLogical: return "Logical";
    case ybDate: return "Date";
    default: return "Undefined";
  }
}

const char* nameof( ybState ste ) {
  char* names[] = { "function", "variable" };
  return names[ste];
}

// end of file

