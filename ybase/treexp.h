/*  $Id: treexp.h,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    TREEXP.H
    Definition of the syntactic tree for expressions

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

#ifndef _TREEXP_H_
#define _TREEXP_H_

#include "types.h"

// A syntactic tree entry
class ybExp : public ybEc {
public:
  virtual ~ybExp() { };
  virtual ybValue run(void) = 0;
}; //ybExp

// List of syntactic trees
class ybExpLst {
public:
  ybExpLst(ybExp*,ybExpLst*);
  ~ybExpLst();

  ybExp *expdta;	// pointer to the syntactic tree
  ybExpLst *expnxt;	// next tree
}; //ybExpLst

// Number in the expression
class ybExpNum : public ybExp {
public:
  ybExpNum(xbString Cvalue) : numval(Cvalue) { }
  ybExpNum(double Nvalue) : numval(Nvalue) { }
  ybExpNum(bool Lvalue) : numval(Lvalue) { }
  ybExpNum(xbDate Dvalue) : numval(Dvalue) { }
  virtual ~ybExpNum() { }
  virtual ybValue run(void);
protected:
  ybValue numval;
}; //ybExpNum

// Generic variable in the expression
class ybExpIde : public ybExp {
public:
  ybExpIde(char*);
  virtual ~ybExpIde();
  virtual ybValue run(void);
protected:
  char *idedta;
}; //ybExpIde

// Database field in the expression
class ybExpDbf : public ybExp {
public:
  ybExpDbf(char*,char*);
  virtual ~ybExpDbf();
  virtual ybValue run(void);
protected:
  char *idedbf;
  char *idefld;
}; //ybExpDbf

// Unary operator in the expression
class ybExpUno : public ybExp {
public:
  ybExpUno(ybOper,ybExp*);
  virtual ~ybExpUno();
  virtual ybValue run(void);
protected:
  ybOper unotyp;	// operator type
  ybExp *unoexp;	// pointer to the subtree
}; //ybExpUno

// Binary operator in the expression
class ybExpBin : public ybExp {
public:
  ybExpBin(ybOper,ybExp*,ybExp*);
  virtual ~ybExpBin();
  virtual ybValue run(void);
protected:
  ybOper bintyp;	// operator type
  ybExp *binlft;        // pointer to the left subtree
  ybExp *binrgt;	// pointer to the right subtree
}; //ybExpBin

// Function call in the expression
class ybExpFce : public ybExp {
public:
  ybExpFce(char*,ybExpLst*);
  virtual ~ybExpFce();
  virtual ybValue run(void);
protected:
  int typecmp(char*,char*);

  ybIdeFun* fcenam;
  ybExpLst* fceprm;
}; //ybExpFce

# endif

// end of file

