/*  $Id: treexp.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TREEXP.CPP
    Implementation of the syntactic tree for expressions

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

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "treexp.h"
#include "table.h"
#include "lang.h"

// -- List of syntactic trees ---------------------------------------

ybExpLst::ybExpLst(ybExp *co, ybExpLst *dalsi) {
  expdta = co;
  expnxt = dalsi;
}

ybExpLst::~ybExpLst() {
  NCdelete(expdta);  // delete data
  NCdelete(expnxt);  // recursive delete for the whole list
}

// -- Number in the expression --------------------------------------

ybValue ybExpNum::run(void) {
  return numval;
}

// -- Generic variable in the expression ----------------------------

ybExpIde::ybExpIde(char *ident) {
  idedta = NCstrdup( ident );
}

ybExpIde::~ybExpIde() {
  // the variable content destroys destructor of the symbol table
  NCfree( idedta );
}

ybValue ybExpIde::run(void) {
  // Look for the variable identifier
  ybIdeVar **ide = (ybIdeVar**)YBsymtab->search(idedta,ybVariable);
  if( *ide != NULL ) return (*ide)->value;
  // Look for the database field
  ybDbf* dbf = YBdbftab->dbf();
  xbShort num = dbf->GetFieldNo(idedta);
  if( num > 0 )
    return dbf->GetField( num );

  // Identifier wasn't found
  error(YBstrings[YB_VARNDEF],idedta);
  return ybValue(); // NAN
}

// -- Database field in the expression ------------------------------

ybExpDbf::ybExpDbf(char *alias, char *field) {
  idedbf = NCstrdup( alias );
  idefld = NCstrdup( field );
}

ybExpDbf::~ybExpDbf() {
  NCfree( idedbf );
  NCfree( idefld );
}

ybValue ybExpDbf::run(void) {
  // Search the database
  ybDbf* dbf = YBdbftab->dbf(idedbf);
  // Look for the database field
  xbShort num = dbf->GetFieldNo(idefld);
  if( num > 0 )
    return dbf->GetField( num );

  // Database field wasn't found
  error(YBstrings[YB_FIENDEF],idedbf,idefld);
  return ybValue(); // NAN
}

// -- Unary operator in the expression ------------------------------

ybExpUno::ybExpUno(ybOper typ, ybExp *dolu) {
  unotyp = typ;
  unoexp = dolu;
}

ybExpUno::~ybExpUno() {
  NCdelete(unoexp);
}

ybValue ybExpUno::run(void) {
  try {
    ybValue inval = unoexp->run();
    switch ( unotyp ) {
      case addOp: return +inval;
      case subOp: return -inval;
      case notLop: return !inval;
      default: assert( 0 ); // internal crash, unknown unary operator
    }
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  return ybValue();
}

// -- Binary operator in the expression -----------------------------

ybExpBin::ybExpBin(ybOper typ, ybExp *vlevo, ybExp *vpravo) {
  bintyp = typ;
  binlft = vlevo;
  binrgt = vpravo;
}

ybExpBin::~ybExpBin() {
  NCdelete(binlft);
  NCdelete(binrgt);
}

ybValue ybExpBin::run(void) {
  try {
    ybValue inlft = binlft->run();
    ybValue inrgt = binrgt->run();
  
    switch (bintyp) {
      case addOp: return inlft + inrgt;
      case subOp: return inlft - inrgt;
      case mulOp: return inlft * inrgt;
      case divOp: return inlft / inrgt;
      case modOp: return inlft % inrgt;
      case andLop: return inlft && inrgt;
      case orLop: return inlft || inrgt;
      case equOp: return inlft == inrgt;
      case neqOp: return inlft != inrgt;
      case lthOp: return inlft < inrgt;
      case gthOp: return inlft > inrgt;
      case leqOp: return inlft <= inrgt;
      case geqOp: return inlft >= inrgt;
      case subsOp: return in( inlft, inrgt ); /* operator '$' */
      default: assert(0); // internal crash, unknown binary operator
    }
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  return ybValue();
}

// -- Function call in the expression -------------------------------

ybExpFce::ybExpFce(char *name, ybExpLst *params) {
  // Search the function identifier
  ybIdeFun **ide = (ybIdeFun**)YBsymtab->search(name,ybFunction);
  if( ( fcenam = *ide ) == NULL ) error(YBstrings[YB_FUNNDEF],name);

  fceprm = params;
}

ybExpFce::~ybExpFce() {
  // destroy parameters
  NCdelete(fceprm);
}

int ybExpFce::typecmp(char *formal, char *actual ) {
  if( formal==NULL || actual==NULL ) return -2; // failure

  while( *formal>0 || *actual>0 ) {
    if( *actual != toupper( *formal ) ) {
      if( *actual == ybUndefined ) return -1; // failure
      else if( *formal == 0 ) return 0; // too many parameters
      else if( isupper( *formal ) ) return 0; // type mismatch
      else formal++;
    } else {
      formal++;
      actual++;
    }
  }
  return 1; // OK
}

ybValue ybExpFce::run(void) {
  ybValue outval;

  if( fcenam!=NULL ) {
    // get actual parameters count
    int forcnt = 0;
    for( ybExpLst* exprs = fceprm; exprs != NULL; exprs = exprs->expnxt ) {
      forcnt++;
    }
    // evaluate all parameters into an array and create "type string"
    ybExpLst* exprs = fceprm;
    ybValue* inval = new ybValue[forcnt];
    char* intyp = (char*)malloc(forcnt+1);
    for( int ii = 0; ii < forcnt; ii++ ) {
      inval[ii] = exprs->expdta->run();
      intyp[ii] = inval[ii].type();
      exprs = exprs->expnxt;
    }
    intyp[forcnt] = 0;
    // find corresponding function prototype
    ybFunPro* proto = fcenam->funpro;
    while( proto != NULL ) {
      // compare actual and formal parameters
      int resu = typecmp(proto->proprm,intyp);

      if( resu < 0 ) {
        // failure -> NAN output
        break;
      } else if( resu == 1 ) {
        // function prototype was found, so create "input array"
        // value types in "input array" will match formal parameters
        ybValue* input = new ybValue[NCstrlen(proto->proprm)];
        unsigned iac = 0;
        unsigned ifo = 0;
        while( ifo<NCstrlen(proto->proprm) ) {
          while( intyp[iac] != toupper(proto->proprm[ifo]) ) ifo++;
          input[ifo++] = inval[iac++];
        }
        // run function
        try {
          outval = proto->proptr(input);
        }
        catch ( yBaseExc exc ) {
          error( exc.message );
        }
        // free "input array"
        if( input != NULL ) delete[] input;
        break;
      }
      // try it once more
      proto=proto->pronxt;
    } /* while */

    if( proto == NULL ) {
      error(YBstrings[YB_INVFUCAL],fcenam->name(),intyp); // Invalid function call
      fprintf(stderr,YBstrings[YB_POSCALL]); // Possible calls are
      for( ybFunPro *pro = fcenam->funpro; pro != NULL; pro = pro->pronxt ) {
        fprintf(stderr," %s(%s)",fcenam->name(),pro->proprm);
      }
      fprintf(stderr,".\n");
    }
    // free resources
    if( inval != NULL ) delete[] inval;
    NCfree(intyp);
  }
  return outval;
}

// end of file

