/*  $Id: lang.cpp,v 1.1 2000/06/07 00:03:49 dbryson Exp $

    yBase project source code

    LANG.CPP
    Language dependent implementations

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

#include"lang.h"

#if defined ( LANG_EN )

const char* YBstrings[] = {
  "Error %s %d: ",
  "Error while opening '%s'.",
  "Variable '%s' was not defined.",
  "Function '%s' is not defined.",
  "Alias '%s' was not defined.",
  "Database '%s' has no field '%s'.",
  "Unary '%s' doesn't work with type '%s'.",
  "Operator '%s' doesn't work with type '%s'.",
  "Command '%s' doesn't work with type '%s'.",
  "Types '%s' and '%s' aren't for '%s' compatible.",
  "Division by zero.",
  "Constant '%s' can't be changed.",
  "Invalid function call %s(%s).",
  "Possible calls are",
  "Type '%s' expected, but type '%s' found.",
  "Workspace '%i' doesn't exist.",
  "All workspaces are occupied, no free one.",
  "Can't assign '%s' value to '%s' field '%s'.",
};

#elif defined ( LANG_CZ )

const char* YBstrings[] = {
  "Chyba %s %d: ",
  "Soubor '%s' nelze otevý¡t.",
  "PromØnn  '%s' nebyla definov na.",
  "Funkce '%s' nebyla definov na.",
  "Alias '%s' nebyl definov n.",
  "Datab ze '%s' nem  z znam '%s'.",
  "Un rn¡ oper tor '%s' nepracuje s typem '%s'.",
  "Oper tor '%s' nepracuje s typem '%s'.",
  "Pý¡kaz '%s' nepracuje s typem '%s'.",
  "Typy '%s' a '%s' nejsou pro '%s' kompatibiln¡.",
  "DØlen¡ nulou.",
  "Konstantu '%s' nelze zmØnit.",
  "Chybn‚ vol n¡ funkce %s(%s).",
  "Pý¡pustn  vol n¡ jsou",
  "OŸek v m typ '%s', byl vçak zad n typ '%s'.",
  "Pracovn¡ oblast '%i' neexistuje.",
  "Vçechny pracovn¡ oblasti jsou obsazeny, § dn‚ voln‚.",
  "Hodnotu '%s' nelze pýiýadit '%s' z znamu '%s'.",
};

#else

#error None language defined, define some LANG_xx

#endif

// end of file

