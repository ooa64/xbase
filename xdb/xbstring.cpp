/* 
    Xbase project source code

    This file contains the xbString object methods

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

    V 1.9.2  9/14/99    - misc user supplied updates
*/

#include <xbase/xbconfig.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#include <xdb/xbstring.h>
#include <xdb/xbexcept.h>

const char * xbString::NullString = "";

xbString::xbString() {
  ctor(NULL);
}

xbString::xbString(size_t size) {
  data = (char *)malloc(size);
  this->size = size;
}

xbString::xbString(char c) {
  ctor(NULL);
  *this = c;
}

xbString::xbString(const char *s) {
  ctor(s);
}

xbString::xbString(const xbString &s) {
  ctor((const char *)s);
}

xbString::xbString(const char *s, size_t maxlen) {
  size_t len = strlen(s);
  
  if (maxlen<len)
    maxlen = len;

  size = maxlen+1;
  data = (char *)malloc(size);
  strncpy(data, s, maxlen);
	data[maxlen] = 0;
}

xbString::~xbString() {
  if (data != NULL)
    free(data);
}

void xbString::ctor(const char *s) {
  if (s == NULL) {
    data = NULL;
    size =0;
    return;
  }

  size = strlen(s)+1;

  data = (char *)malloc(size);
  strcpy(data, s);
}

void xbString::ctor(const char *s,size_t maxlen) {

  if (s == NULL) {
    data = NULL;
    size =0;
    return;
  }

  size_t len = strlen(s);
  
  if (maxlen<len)
    maxlen = len;

  size = maxlen+1;
  data = (char *)malloc(size);
  strncpy(data, s, maxlen);
	data[maxlen] = 0;
}

xbString &xbString::operator=(char c) {
  if (data != NULL)
    free(data);

  data = (char *)malloc(2);
  data[0] = c;
  data[1] = 0;

  size = 2;

  return (*this);
}

xbString &xbString::operator=(const xbString &s) {
  if (data != NULL)
    free(data);

  const char *sd = s;
  if (sd == NULL) {
    data = NULL;
    size = 0;
    return (*this);
  }

  data = (char *)malloc(strlen(s)+1);
  strcpy(data, s);

  size = strlen(data)+1;
  
  return (*this);
}

xbString &xbString::operator=(const char *s) {
  if (data != NULL)
    free(data);

	if (s == NULL) {
		data = NULL;
		size = 0;
		return (*this);
	}

  data = (char *)malloc(strlen(s)+1);
  strcpy(data, s);

  size = strlen(data)+1;
  
  return (*this);
}

void xbString::resize(size_t size) {
  data = (char *)realloc(data, size);
  if (size>0)
    data[size-1] = 0;
  this->size = size;
}
     
bool xbString::isNull() const {
  return (data == NULL);
}

bool xbString::isEmpty() const {
  if (data == NULL)
    return true;
  if (data[0] == 0)
    return true;
  return false;
}

size_t xbString::len() const {
  return ((data == NULL)?0:strlen(data));
}

size_t xbString::length() const {
  return len();
}

xbString xbString::copy() const {
  return (*this);
}

xbString &xbString::sprintf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);

  if (size < 256)
    resize(256);              // make string big enough

#ifdef HAVE_VSNPRINTF
  if (vsnprintf(data, size, format, ap) == -1)
    data[size-1] = 0;
#else
#  if HAVE_VSPRINTF
  vsprintf(data, format, ap);
#  else
#    if
#      error "You have neither vsprintf nor vsnprintf!!!"
#    endif
#  endif
#endif

  resize(strlen(data)+1);               // truncate
  va_end(ap);
  return (*this);
}

xbString::operator const char *() const {
//  return data;
  return (data != NULL) ? data : NullString;
}

xbString &xbString::operator-=(const char *s) {
  if( s == NULL ) return (*this);
  int len = strlen(s);
  int oldlen = this->len();

  data = (char *)realloc(data, oldlen+len+1);
  if( oldlen == 0 ) data[0] = 0;

  // looking for an occurence of space in the first string
  char *lftspc = strchr(data,' ');
  if( lftspc==NULL ) { // left string has no spaces
    strcat(data,s);
  } else { // left string has one or more spaces
    int numspc = strlen(lftspc);
    strcpy(lftspc,s);
    while( numspc-- > 0 ) strcat(lftspc," ");
  }

  size += len;
  return (*this);
}

xbString &xbString::operator+=(const char *s) {
  if (s == NULL)
    return (*this);
  int len = strlen(s);
  int oldlen = this->len();

  data = (char *)realloc(data, oldlen+len+1);
	if (oldlen == 0)
		data[0] = 0;
  strcat(data, s);

  size += len;
  return (*this);
}

xbString &xbString::operator+=(char c) {
  int len = 1;
  int oldlen = this->len();

  data = (char *)realloc(data, oldlen+len+1);
  data[oldlen] = c;
  data[oldlen+1] = 0;

  size++;

  return (*this);
}

const char *xbString::getData() const {
  return (data != NULL) ? data : NullString;
}

const char *xbString::c_str() const {
  return data;
}

void xbString::toLowerCase() {
  int len = this->len();
  for (int i=0;i<len;i++)
    data[i] = (char)tolower(data[i]);
}

int xbString::pos(char c) {
  if (data == NULL)
    return (-1);

  const char *p = strchr(data, c);

  if (p == NULL)
    return (-1);

  return p-data;
}

int xbString::pos(const char* s) {
  if (data == NULL)
    return (-1);

  const char *p = strstr(data, s);

  if (p == NULL)
    return (-1);

  return p-data;
}

void xbString::setNum(long num) {
  sprintf("%ld", num);
}

bool operator==(const xbString &s1, const char *s2) {
  if (s2 == NULL) {
    if (s1.getData() == NULL)
      return true;
    return false;
  }

	if ((s2[0] == 0) && s1.getData() == NULL)
		return true;

  if (s1.getData() == NULL)
    return false;

  return (strcmp(s1, s2) == 0);
}

bool operator!=(const xbString &s1, const char *s2) {
  if (s2 == NULL) {
    if (s1.getData() == NULL)
      return false;
    return true;
  }

	if ((s2[0] == 0) && s1.getData() == NULL)
		return false;

  if (s1.getData() == NULL)
    return true;

  return (strcmp(s1, s2) != 0);
}

bool xbString::operator==( const xbString &s2 ) const {
  if( data == NULL || data[0] == 0 ) {
    if( s2.data == NULL || s2.data[0] == 0 ) return true; // NULL == NULL
    return false; // NULL == !NULL
  } else {
    if( s2.data == NULL || s2.data[0] == 0 ) return false; // !NULL == NULL
    return strcmp(data,s2.data) == 0; //!NULL == !NULL
  }
}

bool xbString::operator!=( const xbString &s2 ) const {
  if( data == NULL || data[0] == 0 ) {
    if( s2.data == NULL || s2.data[0] == 0 ) return false; // NULL != NULL
    return true; // NULL != !NULL
  } else {
    if( s2.data == NULL || s2.data[0] == 0 ) return true; // !NULL != NULL
    return strcmp(data,s2.data) != 0; //!NULL != !NULL
  }
}

bool xbString::operator< ( const xbString &s2 ) const {
  if( data == NULL || data[0] == 0 ) {
    if( s2.data == NULL || s2.data[0] == 0 ) return false; // NULL < NULL
    return true; // NULL < !NULL
  } else {
    if( s2.data == NULL || s2.data[0] == 0 ) return false; // !NULL < NULL
    return strcmp(data,s2.data) < 0; //!NULL < !NULL
  }
}

bool xbString::operator> ( const xbString &s2 ) const {
  if( data == NULL || data[0] == 0 ) {
    if( s2.data == NULL || s2.data[0] == 0 ) return false; // NULL > NULL
    return false; // NULL > !NULL
  } else {
    if( s2.data == NULL || s2.data[0] == 0 ) return true; // !NULL > NULL
    return strcmp(data,s2.data) > 0; //!NULL > !NULL
  }
}

bool xbString::operator<=( const xbString &s2 ) const {
  if( data == NULL || data[0] == 0 ) {
    if( s2.data == NULL || s2.data[0] == 0 ) return true; // NULL <= NULL
    return true; // NULL <= !NULL
  } else {
    if( s2.data == NULL || s2.data[0] == 0 ) return false; // !NULL <= NULL
    return strcmp(data,s2.data) <= 0; //!NULL <= !NULL
  }
}

bool xbString::operator>=( const xbString &s2 ) const {
  if( data == NULL || data[0] == 0 ) {
    if( s2.data == NULL || s2.data[0] == 0 ) return true; // NULL >= NULL
    return false; // NULL >= !NULL
  } else {
    if( s2.data == NULL || s2.data[0] == 0 ) return true; // !NULL >= NULL
    return strcmp(data,s2.data) >= 0; //!NULL >= !NULL
  }
}

ostream& operator << ( ostream& os, const xbString& xbs ) {
  return os << xbs.data;
}

xbString operator-(const xbString &s1, const xbString &s2) {
	xbString tmp(s1.getData());
	tmp -= s2;
	return tmp;
}

xbString operator+(const xbString &s1, const xbString &s2) {
	xbString tmp(s1.getData());
	tmp += s2;
	return tmp;
}

xbString operator+(const xbString &s1, const char *s2) {
	xbString tmp(s1.getData());
	tmp += s2;
	return tmp;
}

xbString operator+(const char *s1, const xbString &s2) {
	xbString tmp(s1);
	tmp += s2;
	return tmp;
}

xbString operator+(const xbString &s1, char c2) {
	xbString tmp(s1.getData());
	tmp += c2;
	return tmp;
}

xbString operator+(char c1, const xbString &s2) {
	xbString tmp(c1);
	tmp += s2;
	return tmp;
}

void xbString::putAt(size_t pos, char c) {
	if (pos>len())
		return;

	data[pos] = c;
}

xbString& xbString::assign(const xbString& str, size_t pos, int n) {
	if (data != NULL)
		free(data);

	if (str.len() <= pos) {
		size = 0;
		return (*this);
	}

	if (str.len() < pos+n) {
		n = str.len()-pos;
	}

	const char *d = str;
		
	if (n == -1) {
//        data = (char *)malloc(str.len()-pos+1); ms win/nt bug fix
          data = (char *)calloc(str.len()-pos+1, sizeof( char ));
	  strcpy(data, d+pos);
	  size = str.len()-pos+1;
	} else {
//	  data = (char *)malloc(n);  ms win/nt bug fix
          data = (char *)calloc( n+1, sizeof(char));
	  strncpy(data, d+pos, n);
	  data[n] = '\0';
	  size = n+1;
	}

	return (*this);
}

void xbString::trim() {
  int l = len()-1;

	for (;;) {
		if (data[l] != ' ')
			break;
		data[l] = 0;
		if (l == 0)
			break;
		l--;
	}
}

xbString &xbString::remove(size_t pos, int n) {
  if (data == NULL)
    return (*this);
  if (data[0] == 0)
    return (*this);

  size_t l = len();

  if (pos>l)
    return (*this);
  if (n == 0)
    return (*this);
  if (n > int(l-pos))
    n = l-pos;
  if (n<0)
    n = l-pos;
  memcpy(data+pos, data+pos+n, l-pos-n+1);

  return (*this);
}

xbString xbString::mid(size_t pos, int n) const {
  if (data == NULL)
    return (*this);
  if (data[0] == 0)
    return (*this);

  size_t l = len();

  if (pos>l)
    return (*this);
  if (n == 0)
    return (*this);
  if (n > int(l-pos))
    n = l-pos;
  if (n<0)
    n = l-pos;

  xbString s;
  s.data = (char *)malloc(n+1);
  strncpy(s.data, data+pos, n);
  s.data[n] = 0;

  return s;
}

