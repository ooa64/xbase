%{
/*  $Id: parser.y,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    PARSER.Y
    Implementation of the syntactic analyzer (for the BISON generator)

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

#include "trecom.h"

int yylex();
int yyterminate();
extern int nerrors;
extern char *FileName;

%}
%union {
  double greal;     // generic double
  int gint;         // generic integer
  long glong;       // generic long
  char *gstr;	    // generic string
  ybOper tope;	    // operator
  ybExp *exp;	    // expression
  ybExpLst *explst; // list of expressions
  ybCom *com;	    // command
}

%term <greal> RNUM
%term <gint>  EVAL BNUM
%term <glong> LNUM
%term <gstr>  ID DNUM SNUM
%term <tope>  NOTOP ADDOP MULOP RELOP EQUOP ANDOP OROP
%term         NL DET
%term         ALIAS BOTTOM DO ELSE ENDDO ENDIF GOTO IF SELECT SKIP TOP
%term         WHILE USE

%left     OROP
%left     ANDOP
%right    NOTOP
%nonassoc EQUOP RELOP
%left     ADDOP
%left     MULOP
%right    UNOP

%type  <com>    stmt stmts
%type  <exp>    expr usealias
%type  <explst> explst exprs

%start    program
%%

program:
   stmts { 
     if( $1!=NULL ) {
       $1->run();
       delete($1);
     }
   }
;

stmts: /* statements */
   stmt NL stmts { 
     $$ = ($1!=NULL ? $1->connect($3) : $3);
   }
 | /* e */ { 
     $$ = NULL;
   }
;

stmt: /* command */
   EVAL explst { $$ = new ybComEval($2, $1); }
 | ID EQUOP expr { $$ = new ybComAsg($1, NULL, $3); NCfree($1); }
 | ID DET ID EQUOP expr { $$ = new ybComAsg($1, $3, $5); NCfree($1); NCfree($3); }
 | IF expr NL stmts ENDIF { $$ = new ybComIf($2, $4, NULL); }
 | IF expr NL stmts ELSE NL stmts ENDIF { $$ = new ybComIf($2, $4, $7); }
 | DO WHILE expr NL stmts ENDDO { $$ = new ybComWhl($3, $5); }
 | SELECT expr { $$ = new ybComSel( $2 ); }
 | GOTO TOP { $$ = new ybComGoto(ybGotoTop, NULL); }
 | GOTO BOTTOM { $$ = new ybComGoto(ybGotoBottom, NULL); }
 | GOTO expr { $$ = new ybComGoto(ybGotoSet, $2); }
 | SKIP expr { $$ = new ybComGoto(ybGotoMove, $2); }
 | USE expr usealias { $$ = new ybComUse( $2, $3 ); }
 | error { $$ = NULL; }
 | /* e */ { $$ = NULL; }
;

usealias: /* USE ... ALIAS */
   ALIAS expr { $$ = $2; }
 | error { $$ = NULL; }
 | /* e */ { $$ = NULL; }
;

expr: /* expression */
   expr ADDOP expr { $$ = new ybExpBin($2, $1, $3); }
 | expr MULOP expr { $$ = new ybExpBin($2, $1, $3); }
 | expr ANDOP expr { $$ = new ybExpBin($2, $1, $3); }
 | expr OROP expr { $$ = new ybExpBin($2, $1, $3); }
 | expr RELOP expr { $$ = new ybExpBin($2, $1, $3); }
 | expr EQUOP expr { $$ = new ybExpBin($2, $1, $3); }
 | ADDOP expr %prec UNOP { $$ = new ybExpUno($1, $2); }
 | NOTOP expr { $$ = new ybExpUno($1, $2); }
 | '(' expr ')' { $$ = $2; }
 | BNUM { $$ = new ybExpNum( bool($1) ); }
 | DNUM { $$ = new ybExpNum( xbDate($1) ); NCfree($1); }
 | LNUM { $$ = new ybExpNum( double($1) ); }
 | RNUM { $$ = new ybExpNum( double($1) ); }
 | SNUM { $$ = new ybExpNum( xbString($1) ); NCfree($1); }
 | ID { $$ = new ybExpIde($1); NCfree($1); }
 | ID DET ID { $$ = new ybExpDbf($1,$3); NCfree($1); NCfree($3); }
 | ID '(' explst ')' { $$ = new ybExpFce($1, $3); NCfree($1); }
 | SELECT '(' explst ')' { $$ = new ybExpFce("SELECT", $3); }
 | ALIAS '(' explst ')' { $$ = new ybExpFce("ALIAS", $3); }
;

explst: /* list of expressions */
   exprs { $$ = $1; }
 | /* e */ { $$ = NULL; }
;

exprs: /* comma delimited expressions */
   expr ',' exprs { $$ = new ybExpLst($1, $3); }
 | expr { $$ = new ybExpLst($1, NULL); }
;

%%

