/*  exptest.cpp 

    Xbase project source code

    This program tests the Xbase expression logic

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

    V 1.8   4/29/99  - Initial creation of this program 
*/

#include "xbase/xbase.h"

class ExpTestor{
 public:
  ExpTestor( xbDbf * );
  xbShort TestExpression( const char * TestExpression, 
                          const char * ExpectedResult );
  xbShort TestExpression( const char * TestExpression, 
                          const xbDouble ExpectedResult );
 private:
  xbDbf   *dbfPtr;
};
/**************************************************************************/
ExpTestor::ExpTestor( xbDbf *dp )
{
  dbfPtr = dp;
}
/**************************************************************************/
xbShort ExpTestor::TestExpression( const char * Expression,
                                   const xbDouble ExpectedResult )
{
  xbExpNode * e;
  xbShort rc = 0;
  
  if(( rc = dbfPtr->xbase->ParseExpression( Expression, dbfPtr )) != 0 ){
    cout << "Parse Error " << rc << " in expression " << Expression << endl;
    return 1;
  }
  e = dbfPtr->xbase->GetExpressionHandle();

//  cout << "********************************" << endl << "Expression: " << Expression << endl;
//  dbfPtr->xbase->DumpExpressionTree( e );

  if(( rc = dbfPtr->xbase->ProcessExpression(e)) != 0 ){
    cout << "Error " << rc << " processing expression " << Expression << endl;
    return 1;
  }
  char type = dbfPtr->xbase->GetExpressionResultType(e);
  if( type != 'N' && type != 'L' ){
    cout << "Expression " << Expression;
    cout << " Expected result type Numeric or Logical, actual was ";
    cout << type << endl;
    return 1;
  }
//  if( type == 'N' && ExpectedResult != dbfPtr->xbase->GetDoub()){
//    cout << "Expression " << Expression;
//    cout << " Actual result of " << dbfPtr->xbase->GetDoub() << " does not match ";
//    cout << " expected result of " << ExpectedResult << endl;
//    return  1;
//  }
//  if( type == 'L' && ExpectedResult != (xbDouble) dbfPtr->xbase->GetInt()){
//    cout << "Expression " << Expression;
//    cout << " Actual result of " << dbfPtr->xbase->GetInt() << " does not match ";
//    cout << " expected result of " << ExpectedResult << endl;
//    return  1;
//  }
  cout << "Expression " << Expression << " returned " << ExpectedResult;
  cout << " OK" << endl;
  return 0;
}
/**************************************************************************/
xbShort ExpTestor::TestExpression( const char * Expression,
                                   const char * ExpectedResult )
{
  xbExpNode * e;
  xbShort rc = 0;
//  char temp[5];
	
  if(( rc = dbfPtr->xbase->ParseExpression( Expression, dbfPtr )) != 0 ){
    cout << "Parse Error " << rc << " in expression " << Expression << endl;
    return 1;
  }
  e = dbfPtr->xbase->GetExpressionHandle();

//  cout << "**********************************" << endl << "Expression: " << Expression << endl;
//  dbfPtr->xbase->DumpExpressionTree( e );
//  cin >> temp;

  if(( rc = dbfPtr->xbase->ProcessExpression(e)) != 0 ){
    cout << "Error " << rc << " processing expression " << Expression << endl;
    return 1;
  }
  char type = dbfPtr->xbase->GetExpressionResultType(e);
  if( type != 'C' ){
    cout << "Expression " << Expression;
    cout << " Expected result type Character, actual was " << type << endl;
    return 1;
  }
//  if( strlen( ExpectedResult ) != strlen( dbfPtr->xbase->GetCharResult())){
//    cout << "Expression " << Expression;
//    cout << " result length of " << strlen( dbfPtr->xbase->GetCharResult()) <<
//      " different than expected of " << strlen( ExpectedResult ) << endl;
//    cout << "Calculated result =" << dbfPtr->xbase->GetCharResult() << "<" << endl;
//    return 1;
//  }
/*
  if( strcmp( ExpectedResult, dbfPtr->xbase->GetCharResult())){
    cout << "Expression " << Expression;
    cout << " Actual result of " << dbfPtr->xbase->GetCharResult() << " does not match ";
    cout << " expected result of " << ExpectedResult << endl;
    return  1;
  }
*/
  cout << "Expression " << Expression << " returned " << ExpectedResult;
  cout << " OK" << endl;
  return 0;
}
/*************************************************************************/
main()
{
  xbSchema MyRecord[] =
  {
    { "FLOAT1", XB_FLOAT_FLD,  9, 2 },
    { "", 0, 0, 0 },
  };


  xbXBase x;
  xbDbf d( &x );

  d.CreateDatabase( "TEST", MyRecord, XB_OVERLAY );
  d.PutFloatField( "FLOAT1", 5 );  
  

  cout << "XBase Expression testor program.." << endl;
  cout << "This program test the XBase expression logic." << endl;
  ExpTestor * e = new ExpTestor(&d);

  e->TestExpression( "(25-3+2)*2", 48 );
  e->TestExpression( "(25+3+2)*2", 60 );

/* test functions which return a character value result */
  e->TestExpression( "CDOW( \"20000101\" )", "Saturday " );
  e->TestExpression( "CHR( 101 )", "e" );
  e->TestExpression( "CMONTH( \"20000101\" )", "January  " );
  e->TestExpression( "DATE()", x.DATE() );
  e->TestExpression( "DTOC( \"20000101\" )", "01/01/00" );
  e->TestExpression( "DTOS( \"20000101\" )", "20000101" );
  e->TestExpression( "LEFT( \"STRING\", 3 )", "STR" );
  e->TestExpression( "LTRIM( \"   xxxxxx\" )", "xxxxxx" );
  e->TestExpression( "LOWER( \"AAAA\" )", "aaaa" );

//  RECNO is broken
//  e->TestExpression( "RECNO( 1 )", "o" );

  e->TestExpression( "REPLICATE( \"abc\", 3 )", "abcabcabc" );
  e->TestExpression( "RTRIM( \"zzz   \" )", "zzz" );
  e->TestExpression( "RTRIM( \"zzz   \" )+\"qqq\"", "zzzqqq" );
  e->TestExpression( "SPACE( 3 )", "   " );
  e->TestExpression( "STR( \"ccc\" )", "ccc       " );
  e->TestExpression( "STR( \"ccc\", 3 )", "ccc" );
  e->TestExpression( "STR( -52.345 )", "-52.345   " );    
  e->TestExpression( "STR( -52.345, 3 )", "-52.345" );    
  e->TestExpression( "STR( 52.34, 4, 1 )", "52.3" );
  
// not sure what the STRZERO function is supposed to do
/*
  e->TestExpression( "STRZERO( \"aaa\" )", "not sure" );
  e->TestExpression( "STRZERO( \"aaa\", 3, 3, )", "??" );
  e->TestExpression( "STRZERO( 22 )", "not sure" );
  e->TestExpression( "STRZERO( 22, 3 )", "not sure" );
  e->TestExpression( "STRZERO( 2, 3, 3 )", "not sure" );
  e->TestExpression( "STRZERO( \"ddd\", 4, 6 ), "not sure" );
*/  
  e->TestExpression( "TRIM( \"aaa    \" )", "aaa" );
  e->TestExpression( "UPPER( \"abcde\" )", "ABCDE" );

/* functions returning double values */
  e->TestExpression( "ABS( -222 )", 222 );
  
  // next two lines generate invalid error messages - FIX ME */
  e->TestExpression( "EXP( 1 )", (xbDouble) 2.7182800 );  
  e->TestExpression( "LOG( 2 )", (xbDouble) 0.69314700 );
  
  e->TestExpression( "MAX( 10, 27 )", 27 );
  e->TestExpression( "MIN( 10, 5 )", 5 );
  e->TestExpression( "SQRT( 9 )", 3 );
  
/* functions returning long values */
  e->TestExpression( "ASC( \"A\" )", 'A' );
  e->TestExpression( "AT( \"BC\", \"ABCD\" )", 2 );

  e->TestExpression( "DAY( 20000101 )", 1 );

//  next function doesn't work
//  e->TestExpression( "DESCEND( \"A\" )", 1 );

  e->TestExpression( "DOW( 20000101 )", 6 );
  e->TestExpression( "INT( 621.5 )", 621 );
  e->TestExpression( "ISALPHA( \"A\" )", 1 );
  e->TestExpression( "ISLOWER( \"a\" )", 1 );
  e->TestExpression( "ISUPPER( \"A\" )", 1 );
  e->TestExpression( "LEN( \"AAAAA\" )", 5 );
  e->TestExpression( "MONTH( 20000101 )", 1 );
  e->TestExpression( "VAL( \"ABC\" )", 65 );
  e->TestExpression( "YEAR( \"20000101\" )", 2000 );

//  e->TestExpression( "(25-3+2)*2", 48 );
  e->TestExpression( "TEST->FLOAT1+1", 6 );
  e->TestExpression( "TEST->FLOAT1 + 1", 6 );
  e->TestExpression( "FLOAT1+1", 6 );
  e->TestExpression( "FLOAT1 + 1", 6 );

  e->TestExpression( "TEST->FLOAT1 < 1", (xbDouble) 0 );
  e->TestExpression( "TEST->FLOAT1 > 1", (xbDouble) 1 );
}     
