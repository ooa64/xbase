/*  $Id: trecom.h,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TRECOM.H
    Definition of the syntactic tree for commands

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

#ifndef _TRECOM_H_
#define _TRECOM_H_

#include "types.h"
#include "treexp.h"

// A syntactic tree entry
class ybCom : public ybEc {
public:
  ybCom();
  virtual ~ybCom();
  virtual void run(void) = 0;
  ybCom* connect(ybCom*);
protected:
  ybCom *comnxt;        // pointer to the next command
}; //ybCom

// Print command
class ybComEval : public ybCom {
public:
  ybComEval(ybExpLst*,int);
  virtual ~ybComEval();
  virtual void run(void);
protected:
  ybExpLst *comexp;	// pointer to the expression
  int comtyp;
}; //ybComEval

// Assignment to a variable or a database field
class ybComAsg : public ybCom {
public:
  ybComAsg(char*, char*, ybExp*);
  virtual ~ybComAsg();
  virtual void run(void);
protected:
  char* asgnam;         // identifier name
  char* asgdbf;         // database alias
  ybExp* asgrgt;	// expression
}; //ybComAsg

enum ybGoto {
  ybGotoTop,    // GOTO TOP
  ybGotoBottom, // GOTO BOTTOM
  ybGotoSet,    // GOTO <num>
  ybGotoMove    // SKIP <num>
};

// GO/GOTO/SKIP command
class ybComGoto : public ybCom {
public:
  ybComGoto(ybGoto, ybExp*);
  virtual ~ybComGoto();
  virtual void run(void);
protected:
  ybGoto gottyp;        // type of goto command
  ybExp* gotval;	// expression
}; //ybComGoto

// IF command
class ybComIf : public ybCom {
public:
  ybComIf(ybExp*,ybCom*,ybCom*);
  virtual ~ybComIf();
  virtual void run(void);
protected:
  ybExp *ifexp;		  // pointer to the expression
  ybCom *ifthen,*ifelse;  // pointer to the THEN and ELSE subtree
}; //ybComIf

// WHILE command
class ybComWhl : public ybCom {
public:
  ybComWhl(ybExp*, ybCom*);
  virtual ~ybComWhl();
  virtual void run(void);
protected:
  ybExp *whlexp;	// pointer to expression
  ybCom *whlcom;	// pointer to commands
}; //ybComWhl

// RETURN command
class ybComRet : public ybCom {
public:
  ybComRet(ybExp*);
  virtual ~ybComRet();
  virtual void run(void);
protected:
  ybExp *retexp;	// pointer to returned value
}; //ybComRet

// SELECT command
class ybComSel : public ybCom {
public:
  ybComSel(ybExp*);
  virtual ~ybComSel();
  virtual void run(void);
protected:
  ybExp* what;
}; //ybComSel

// USE command
class ybComUse : public ybCom {
public:
  ybComUse(ybExp*,ybExp*);
  virtual ~ybComUse();
  virtual void run(void);
protected:
  ybExp* usename;
  ybExp* usealias;
}; //ybComUse

# endif

// end of file

