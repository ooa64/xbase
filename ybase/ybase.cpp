/*  $Id: ybase.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    YBASE.CPP
    Implementation of the main program

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
#include <unistd.h>
#include "table.h"
#include "lang.h"

char *FileName;      // name of the input file
extern FILE *yyin;   // input file

int yyparse(void);   // syntactic analyzer

int version(void) {
  printf("xBase "VERSION", yBase "YB_VERSION" ("__DATE__" "__TIME__")\n");
  return 0;
}
int help(void) {
  printf("\
usage: ybase [-hv] file\n\
 -h       give this help\n\
 -v       display version number\n\
 file     file to be processed\n");
  return 0;
}

// main function of the whole interpreter
int main(int argc, char **argv) {

  int c;
  while ( (c = getopt(argc, argv, "hv")) != -1 ) {
    switch( c ) {
      case 'h':
        // print help
        version();
        help();
        return 0;
      case 'v':
        // print the yBase version number
        version();
        return 0;
      case '?':
        printf("Unknown option -'%c'\n", c);
        return 1;
    }
  }
  // read input filename
  if( argc > optind ) {
    FileName = NCstrdup(argv[optind]);
    // open input file
    yyin = fopen(argv[optind], "r");
    if (yyin == NULL) {
      // error while opening input file
      fprintf(stderr, YBstrings[YB_ERROPEN], FileName);
      fprintf(stderr, "\n");
      return 1;
    }
  } else {
    fprintf(stderr, "Input filename must be specified.\n");
    return 1;
  }

  try {
    // database initialization
    xbXBase base;
    YBdbftab = new ybDbfTab(base);
  
    // Initialization of symbol table
    YBsymtab = new ybSymTab();
  
    // Adding system constants
    struct {
      char* name;       // constant name
      ybType type;      // type
      double value;     // value
    } vars[] = {
      { "PI", ybNumeric, 3.14159265358979323846 },
      { "E", ybNumeric, 2.71828182845904523536 }
    };
    for (int ii = 0; ii < 2; ii++) {
      ybIdeVar** id = (ybIdeVar**)YBsymtab->search(vars[ii].name,ybVariable);
      if(*id == NULL) {
        *id = new ybIdeVar( vars[ii].name, 1 );
        (*id)->value = ybValue( vars[ii].value );
      } else fprintf(stderr,"Init error: Constant %s already exists.\n", vars[ii].name);
    }
  
    // Adding system functions
    struct {
      char* name;        // function name
      char* params;      // formal parameters
      ybFcePtr function; // pointer to the implementation
    } fces[] = {
      { "ABS", "N", YBabs },
      { "ALIAS", "n", YBalias },
      { "ASC", "C", YBasc },
      { "AT", "CC", YBat },
      { "CDOW", "D", YBcdow },
      { "CHR", "N", YBchr },
      { "CMONTH", "D", YBcmonth },
      { "CTOD", "C", YBctod },
      { "DATE", "", YBdate },
      { "DAY", "D", YBday },
      { "DOW", "D", YBdow },
      { "GETENV", "C", YBgetenv },
      { "IFF", "LCC", YBiff },
      { "IFF", "LNN", YBiff },
      { "IFF", "LLL", YBiff },
      { "IFF", "LDD", YBiff },
      { "INT", "N", YBint },
      { "ISALPHA", "C", YBisalpha },
      { "ISLOWER", "C", YBislower },
      { "ISUPPER", "C", YBisupper },
      { "LEN", "C", YBlen },
      { "LOG", "N", YBlog },
      { "MAX", "CC", YBmax },
      { "MAX", "NN", YBmax },
      { "MAX", "DD", YBmax },
      { "MIN", "CC", YBmin },
      { "MIN", "NN", YBmin },
      { "MIN", "DD", YBmin },
      { "MOD", "N", YBmod },
      { "MONTH", "D", YBmonth },
      { "OS", "", YBos },
      { "RECCOUNT", "", YBreccount },
      { "RECNO", "", YBrecno },
      { "ROUND", "NN", YBround },
      { "SELECT", "", YBselect },
      { "SQRT", "N", YBsqrt },
      { "STR", "Nnn", YBstr },
      { "TIME", "", YBtime },
      { "VAL", "C", YBval },
      { "VERSION", "", YBversion },
      { "YEAR", "D", YByear },
      { "", "", NULL } };
    for (int ii = 0; fces[ii].name[0] > 0; ii++) {
      ybIdeFun** id = (ybIdeFun**)YBsymtab->search(fces[ii].name,ybFunction);
      if(*id == NULL) *id = new ybIdeFun( fces[ii].name );
      (*id)->funpro = new ybFunPro( fces[ii].params, fces[ii].function, (*id)->funpro );
    }
  
    yyparse();
  
//    YBsymtab->dump();
    NCdelete( YBsymtab );
    NCdelete( YBdbftab );
  
    // close input files
    if( FileName!=NULL) {
      free(FileName);
      fclose(yyin);
    }
  }
  catch ( yBaseExc exc ) {
    fprintf( stderr, "Error: %s\n", exc.message );
    return 1;
  }
  catch ( ... ) {
    fprintf( stderr, "Error: unknown exception\n" );
    return 1;
  }

  return 0;
} // main

// end of file

