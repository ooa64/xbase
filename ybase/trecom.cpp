/*  $Id: trecom.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TRECOM.CPP
    Implementation of the syntactic tree for commands

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
#include "trecom.h"
#include "table.h"
#include "lang.h"

// -- A syntactic tree entry ----------------------------------------

ybCom::ybCom() {
  comnxt = NULL;
}

ybCom::~ybCom() {
  NCdelete(comnxt);
}

// join after this block of commands given block
ybCom* ybCom::connect(ybCom* dalsi) {
  // if this block has more commands, join till the end
  if (comnxt!=NULL) comnxt->connect(dalsi);
    else comnxt = dalsi;
  return this;
}

// -- Print command -------------------------------------------------

ybComEval::ybComEval(ybExpLst *exprs, int type) {
  comexp = exprs;
  comtyp = type;
}

ybComEval::~ybComEval() {
  NCdelete(comexp);
}

void ybComEval::run(void) {
  try {
    if( comtyp==1 ) cout << endl;
    ybExpLst *exprs = comexp;
      while( exprs!=NULL ) {
      ybValue value = exprs->expdta->run();
      cout << value;
      exprs = exprs->expnxt;
    }
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- Assignment to a variable or a database field ------------------

ybComAsg::ybComAsg(char* innam, char* indbf, ybExp* inrgt) {
  asgnam = NCstrdup(innam);
  asgdbf = NCstrdup(indbf);
  asgrgt = inrgt;
}

ybComAsg::~ybComAsg() {
  NCfree(asgnam);
  NCfree(asgdbf);
  NCdelete(asgrgt);
}

void ybComAsg::run(void) {
  try {
    if( asgdbf == NULL ) {
      // look for the variable identifier
      ybIdeVar **ide = (ybIdeVar**)YBsymtab->search(asgnam, ybVariable);
      if( *ide != NULL ) {
        // Variable identifier found
        if( (**ide).constant ) {
          // constants can't be changed
          error(YBstrings[YB_CONSNOCH],(**ide).name());
        } else {
          // Assign value to variable
          if( asgrgt!=NULL) (**ide).value = asgrgt->run();
          else (**ide).value = ybValue(); // NAN
        }
      } else {
        // Variable not found -> look for the database field
        ybDbf* dbf = YBdbftab->dbf();
        xbShort num = dbf->GetFieldNo(asgnam);
        if( num > 0 ) {
          // Assign value to database field
          ybValue outval = asgrgt->run();
          if( asgrgt!=NULL) dbf->PutField( num, outval );
        } else {
          // No variable, no database field -> create new variable
          *ide = new ybIdeVar(asgnam, 0);
          // Assign value to variable
          if( asgrgt!=NULL) (**ide).value = asgrgt->run();
          else (**ide).value = ybValue(); // NAN
        }
      } /* if */
    } else {
      // Search the database
      ybDbf* dbf = YBdbftab->dbf(asgdbf);
      // Look for the database field
      xbShort num = dbf->GetFieldNo(asgnam);
      if( num > 0 ) {
        // Assign value to database field
        ybValue outval = asgrgt->run();
        if( asgrgt!=NULL) dbf->PutField( num, outval );
      } else {
        // Database field wasn't found
        error(YBstrings[YB_FIENDEF],asgdbf,asgnam);
      }
    }
  } /* try */
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- GOTO command --------------------------------------------------

ybComGoto::ybComGoto(ybGoto intyp, ybExp* inval) {
  gottyp = intyp;
  gotval = inval;
}

ybComGoto::~ybComGoto() {
  NCdelete( gotval );
}

void ybComGoto::run(void) {
  try {
    // get database
    ybDbf* dbf = YBdbftab->dbf();
    // process command
    switch( gottyp ) {
      case ybGotoTop:
        dbf->GetFirstRecord();
        break;
      case ybGotoBottom:
        dbf->GetLastRecord();
        break;
      case ybGotoSet: {
        ybValue inval = gotval->run();
        switch( inval.type() ) {
          case ybNumeric: {
            long posit = inval;
            dbf->GetRecord( posit );
            break;
          }
          case ybUndefined: break;
          default: error( YBstrings[YB_COMNOWORK],"GOTO",nameof(inval.type()) );
        }
        break;
      }
      case ybGotoMove: {
        ybValue inval = gotval->run();
        switch( inval.type() ) {
          case ybNumeric: {
            long posit = inval;
            posit += dbf->GetCurRecNo();
            dbf->GetRecord( posit );
            break;
          }
          case ybUndefined: break;
          default: error( YBstrings[YB_COMNOWORK],"SKIP",nameof(inval.type()) );
        }
        break;
      }
      default: assert(0);
    }
  } /* try */
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- IF command ----------------------------------------------------

ybComIf::ybComIf(ybExp *vyraz, ybCom *pak, ybCom *jinak) {
  ifexp = vyraz;
  ifthen = pak;
  ifelse = jinak;
}

ybComIf::~ybComIf() {
  NCdelete(ifexp);
  NCdelete(ifthen);
  NCdelete(ifelse);
}

void ybComIf::run(void) {
  try {
    ybValue inval = ifexp->run();
    if( inval.type() != ybLogical )
      throw yBaseExc(YBstrings[YB_EXPETYP],"Logical",nameof(inval.type()));

    if( (bool)inval ) {
      if( ifthen != NULL ) ifthen->run();
    } else {
      if( ifelse != NULL ) ifelse->run();
    }
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- WHILE command -------------------------------------------------

ybComWhl::ybComWhl(ybExp *vyraz, ybCom *delej) {
  whlexp = vyraz;
  whlcom = delej;
}

ybComWhl::~ybComWhl() {
  NCdelete(whlexp);
  NCdelete(whlcom);
}

void ybComWhl::run(void) {
  try {
    do {
      // get logical value
      ybValue inval = whlexp->run();
      if( inval.type() != ybLogical )
        throw yBaseExc(YBstrings[YB_EXPETYP],"Logical",nameof(inval.type()));

      if( whlcom == NULL ) break; // command is empty
      if( !(bool)inval ) break; // condition is FALSE
      // run commands
      whlcom->run();
    } while( 1 );
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- RETURN command ------------------------------------------------

ybComRet::ybComRet(ybExp *vyraz) {
  retexp = vyraz;
}

ybComRet::~ybComRet() {
  NCdelete(retexp);
}

void ybComRet::run(void) {
  try {
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- SELECT command ------------------------------------------------

ybComSel::ybComSel(ybExp* inwhat) {
  what = inwhat;
}

ybComSel::~ybComSel() {
  NCdelete( what );
}

void ybComSel::run(void) {
  try {
    ybValue area = what->run();
    YBdbftab->select( area );
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// -- USE command ---------------------------------------------------

ybComUse::ybComUse(ybExp *inname, ybExp *inalias) {
  usename = inname;
  usealias = inalias;
}

ybComUse::~ybComUse() {
  NCdelete( usename );
  NCdelete( usealias );
}

void ybComUse::run(void) {
  try {
    ybValue inname = usename->run();
    ybValue inalias = (usealias!=NULL?usealias->run():ybValue());
    YBdbftab->dbf()->OpenDatabase(inname,inalias);
    // Warning: followin functiong call under DOS causes error -127
    // to fix it turn LOCKING off
    YBdbftab->dbf()->GetFirstRecord();
  }
  catch ( yBaseExc exc ) {
    error( exc.message );
  }
  // next command
  if( comnxt!=NULL ) comnxt->run();
}

// end of file

