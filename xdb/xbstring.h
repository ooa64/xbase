/* 

    Xbase project source code

    This file contains the Class definition for a xbString object.

    Copyright (C) 1997  StarTech, Gary A. Kunkel   
    email - xbase@startech.keller.tx.us
    www   - http://www.startech.keller.tx.us/xbase.html

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

    V 1.9.2  9/14/99    - Misc user supplied updates 
*/

#ifndef __XBSTRING_H__
#define __XBSTRING_H__

#include <xdb/xbconfig.h>
#include <stdlib.h>
#include <iostream.h>

/*! \file xbstring.h
*/

//! xbString class
/*!
*/

class XBDLLEXPORT xbString {
public:
  enum {npos = -1};

  xbString();
  xbString(size_t size);
  xbString(char c);
  xbString(const char *s);
  xbString(const char *s, size_t maxlen);
  xbString(const xbString &s);

  ~xbString();

  void ctor(const char *s);
  void ctor(const char *s, size_t maxlen);

  xbString &operator=(const xbString &s);
  xbString &operator=(const char *s);
  xbString &operator=(char c);
  
  bool isNull() const;
  bool isEmpty() const;
  size_t len() const;
  size_t length() const;

  void resize(size_t size);  
  xbString copy() const;
  
  xbString &sprintf(const char *format, ...);
  void setNum(long num);
  
  xbString& assign(const xbString& str, size_t pos = 0, int n = npos);
  char operator[](int n) { return data[n]; }
  char getCharacter( int n ) const { return data[n]; }
  operator const char *() const;
  xbString &operator+=(const char *s);
  xbString &operator+=(char c);
  xbString &operator-=(const char *s);
  void putAt(size_t pos, char c);

  const char *getData() const;
  const char *c_str() const;
  void toLowerCase();
  int pos(char c);
  int pos(const char* s);
  void trim();
  bool compare(char s);
  bool compare(const char *s);

  bool operator == ( const xbString& ) const;
  bool operator != ( const xbString& ) const;
  bool operator < ( const xbString& ) const;
  bool operator > ( const xbString& ) const;
  bool operator <= ( const xbString& ) const;
  bool operator >= ( const xbString& ) const;

  friend ostream& operator << ( ostream&, const xbString& );

  xbString &remove(size_t pos = 0, int n = npos);
  xbString mid(size_t pos = 0, int n = npos) const;

protected:
  char *data;
  size_t size;
  static const char * NullString;
};

bool operator==(const xbString &s1, const char *s2);
bool operator!=(const xbString &s1, const char *s2);
bool operator==(const xbString &s1, char s2);
bool operator!=(const xbString &s1, char s2);

xbString operator+(const xbString &s1, const xbString &s2);
xbString operator+(const xbString &s1, const char *s2);
xbString operator+(const char *s1, const xbString &s2);
xbString operator+(const xbString &s1, char c2);
xbString operator+(char c1, const xbString &s2);
xbString operator-(const xbString &s1, const xbString &s2);

#endif

