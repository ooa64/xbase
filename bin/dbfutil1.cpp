/*  $Id: dbfutil1.cpp,v 1.7 2002/12/17 03:03:55 dbryson Exp $

    dbf utility program

    This is a generic XBase menu program for executing XBase functions


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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact:

      Mail:
  
        Technology Associates, Inc.
        XBase Project
        1455 Deming Way #11
        Sparks, NV  89434
        USA

     Email:
       
       xbase@techass.com

     Website:

       xdb.sourceforge.net


*/

#include <xbase/xbase.h>

xbXBase x;
xbDbf d( &x );
xbIndex *ix;


class MyClass {
  public:
    MyClass() {};
    void OpenFile();
    void CloseFile();
    void ConvertDatabase();
    void ReindexDatabase();
    void DeleteMemoField();
    void UpdateMemoField();
    void ShowFieldInfo();
    void ShowFieldData();
    void UpdateFieldData();
    void PutRecord();

    void GetRecord();

    void DumpDbtHeader();
    void BlankRecord();
   
    void FindMemoBlocks();
    void GetMemoBlocks();
 
    void FileStats();
    void MainMenu();
    void FileMenu();
    void RecordMenu();
    void FieldMenu();
    void IndexMenu();
    void DebugMenu();
    
    void OpenIndex();
    void CloseIndex();
    void FindKey();
    void GetFirstKey();
    void GetLastKey();
    void GetNextKey();
    void GetPrevKey();
    void Reindex();
    void DumpIndexNodes();
};
/************************************************************************/
void MyClass::DumpIndexNodes()
{
  xbLong NodeCnt = ix->GetTotalNodes();
  std::cout << "For NDX indices, output placed in xbase.log" << std::endl;
  std::cout << "total nodes = " << NodeCnt << std::endl;
  ix->DumpHdrNode();
  if( NodeCnt > 1 )
    for( xbLong l = 1L; l <= NodeCnt-1; l++ )
      ix->DumpNodeRec( l );
}
/************************************************************************/
void MyClass::OpenIndex()
{
  int rc = 0;
  char Filename[51];
  std::cout << "Enter index name: ";
  std::cin >> Filename;
  if( strstr( Filename, "NDX" ) || strstr( Filename, "ndx" )){
    ix = new xbNdx( &d );
    rc = ix->OpenIndex( Filename );
  }
  else if( strstr( Filename, "NTX" ) || strstr( Filename, "ntx" )){
    ix = new xbNtx( &d );
    rc = ix->OpenIndex( Filename );
  }
  else
  {
    std::cout
      << "Invalid index name. Needs file extension. index.ndx or index.ntx"
      << std::endl;
    return;
  }
  std::cout << "Return code " << rc << std::endl;
}    
/************************************************************************/
void MyClass::FindKey()
{
  char KeyBuf[101];
  memset( KeyBuf, 0x00, 101 );
  std::cout << "This function only works for character keys (so far).."
            << std::endl;
  std::cout << "Enter search key:" << std::endl;
  std::cin >> KeyBuf;
  std::cout <<  "Return code " << ix->FindKey( KeyBuf ) << std::endl;
}
/************************************************************************/
void MyClass::GetNextKey()
{
  std::cout << "Return code " << ix->GetNextKey() << std::endl;
}
/************************************************************************/
void MyClass::GetPrevKey()
{
  std::cout << "Return code " << ix->GetPrevKey() << std::endl;
}
/************************************************************************/
void MyClass::GetFirstKey()
{
  std::cout << "Return code " << ix->GetFirstKey() << std::endl;
}
/************************************************************************/
void MyClass::GetLastKey()
{
  std::cout << "Return code " << ix->GetLastKey() << std::endl;
}
/************************************************************************/
void MyClass::CloseIndex()
{
  std::cout << "Return code " << ix->CloseIndex() << std::endl;
}
/************************************************************************/
/* stats */
void MyClass::FileStats()
{
  std::cout << "Number of records   = " << d.NoOfRecords()       << std::endl;
  std::cout << "Number of fields    = " << d.FieldCount()        << std::endl;
  std::cout << "Database Status     = " << d.GetDbfStatus()      << std::endl;
  std::cout << "Record Length       = " << d.GetRecordLen()      << std::endl;
  std::cout << "Memo Fields Present = " << d.MemoFieldsPresent() << std::endl;
  std::cout << "Current Record No   = " << d.GetCurRecNo()       << std::endl;
}  
/************************************************************************/
/* open database */
void MyClass::OpenFile()
{
  int rc;
  char filename[50];
  std::cout << "Enter database file name: ";
  std::cin >> filename;

  rc = d.OpenDatabase( filename );
  std::cout << "Return Code " << rc << std::endl;
}
/************************************************************************/
/* close database */
void MyClass::CloseFile()
{
  int rc;
  rc = d.CloseDatabase();
  std::cout << "Return Code " << rc << std::endl;
}
/************************************************************************/
/* convefrt database */
void MyClass::ConvertDatabase()
{
  int     rc;
  xbShort FieldNo;
  xbLong  len, FieldCnt;
  xbDbf   d2( &x );
  char    *MemoFieldBuf = NULL;
  xbLong  MemoFieldLen = 0L;

  std::cout << "Warning - check your results!!" << std::endl << std::endl;

  char filename[50];
  std::cout << "Enter database file name to convert to: ";
  std::cin >> filename;

  if(( rc = d2.OpenDatabase( filename )) != XB_NO_ERROR ){
    std::cout << "Error " << rc << " opening database" << std::endl;
    return;
  }

  rc = d.GetFirstRecord();
  FieldCnt = d.FieldCount();
  std::cout << "Database has " << FieldCnt << " fields to convert"
            << std::endl;
  while( rc == XB_NO_ERROR )
  {
    d2.BlankRecord();
    for( xbShort i = 0; i < FieldCnt; i++ )
    {
      if(( FieldNo = d2.GetFieldNo( d.GetFieldName( i ))) == -1 )
        std::cout << "Error converting field " << d.GetFieldName( i )
                  << std::endl;
      else 
      {
        if( d.GetFieldType( i ) != d2.GetFieldType( FieldNo ))
          std::cout << "Error - different field types " << d.GetFieldName(i);
        else
        {
          if( d.GetFieldType( i ) == 'M' )
          {
#ifdef XB_MEMO_FIELDS
            len = d.GetMemoFieldLen( i ); 
            if( len > MemoFieldLen )
            {
              if( MemoFieldLen > 0 ) 
                delete MemoFieldBuf;
              MemoFieldBuf = new char[len];
              MemoFieldLen = len;
            }
            d.GetMemoField( i, len, MemoFieldBuf, F_SETLKW );              
            d2.UpdateMemoData( FieldNo, len, MemoFieldBuf, F_SETLKW );
#else
           std::cout
             << "Memo fields not converted XB_MEMO_FIELDS not compiled in\n";
#endif
          }
          else{
            if( d.GetFieldType(i) == 'N' ){
                std::cout << "const code compile prob..." << std::endl;

//                d2.PutField( FieldNo, x.LTRIM( d.GetField( i )));
            }
            else
             d2.PutField( FieldNo, d.GetField(i));
          }
        } 
      }
    }
    rc = d.GetNextRecord();
    d2.AppendRecord();
  }
  if( MemoFieldLen > 0 ) 
    delete MemoFieldBuf;
  d2.CloseDatabase();
}
/************************************************************************/
void MyClass::ShowFieldInfo()
{
  std::cout << "Number   Name        Type  Len  Decs" << std::endl;
  for( int i = 0; i < d.FieldCount(); i++ ){
    printf( "%d     %-10s         %c     %5d   %d\n", i,
              d.GetFieldName(i), d.GetFieldType(i), d.GetFieldLen(i),
              d.GetFieldDecimal(i) );
    if(((i+1)%15) == 0 ){
      std::cout << "Press a key to continue" << std::endl;
      getchar();
      std::cout << "Number   Name        Type  Len  Decs" << std::endl;
    }
  } 
  std::cout << "Press a key to continue" << std::endl;
  getchar();
}
/************************************************************************/
void MyClass::ShowFieldData()
{
  std::cout << "Name      Data" << std::endl;
  for( int i = 0; i < d.FieldCount(); i++ ){
    printf( "%-10s = %s\n", d.GetFieldName(i), d.GetStringField(i));
    if(((i+1)%15) == 0 ){
      std::cout << "Press a key to continue" << std::endl;
      getchar();
      std::cout << "Name      Data" << std::endl;
    }
  }
  std::cout << "Press a key to continue" << std::endl;
  getchar();
   
}
/************************************************************************/
void MyClass::UpdateFieldData()
{
  char dbuf[100];
  int rc, fn;

  std::cout  << "Enter Field Number to update (first field is 0): "
             << std::endl;
  std::cin   >> fn;
  std::cout  << "Enter Field Data" << std::endl;
  std::cin   >> dbuf;
  rc = d.PutField( fn, dbuf );
  
  std::cout << "Return Code " << rc << std::endl;
}
/************************************************************************/
/* open database */
void MyClass::ReindexDatabase()
{
  xbNdx n(&d);
  int rc;
  char filename[50];
  std::cout << "Enter index file name: ";
  std::cin >> filename;

  if(( rc = n.OpenIndex( filename )) != XB_NO_ERROR ){
    std::cout << "Error " << rc << " Opening index" << std::endl;
    return;
  }

  if(( rc = n.ReIndex()) != XB_NO_ERROR )
    std::cout << "Error " << rc << " Reindexing database" << std::endl;
}
/************************************************************************/
void MyClass::GetRecord()
{
  int rc;
  xbLong RecNo;
  std::cout << "Enter record number: ";
  std::cin >> RecNo;

  rc = d.GetRecord( RecNo );
  std::cout << "Return Code = " << rc << std::endl;
}
/************************************************************************/
void MyClass::UpdateMemoField()
{
#ifdef XB_MEMO_FIELDS
  xbShort fn,rc;
  xbLong len;
  char bufchar[2];
  char *buf;

  std::cout << "Enter Field Number: " << std::endl;
  std::cin >> fn;
  if( fn < 0 || fn > d.FieldCount()){
    std::cout << "Invalid Field Number" << std::endl;
    return;
  }
  if( d.GetFieldType( fn ) != 'M' ){
    std::cout << "Field " << fn << " is not a memo field" << std::endl;
    std::cout << "Field Name = " << d.GetFieldName( fn ) << " type= ";
    std::cout << d.GetFieldType( fn ) << std::endl;
    return;
  }
  std::cout << "Enter length of memo data:" << std::endl;
  std::cin >> len;
 
  std::cout << "Enter character to populate memo data:" << std::endl;
  std::cin >> bufchar;

  buf = new char[len];
  memset( buf, bufchar[0], len );

  rc = d.UpdateMemoData( fn, len, buf, F_SETLKW );
  std::cout << "Return Code = " << rc << std::endl;
#else
  std::cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
void MyClass::DeleteMemoField()
{
#ifdef XB_MEMO_FIELDS
  xbShort fn,rc;
  std::cout << "Enter Field Number: " << std::endl;
  std::cin >> fn;
  if( fn < 0 || fn > d.FieldCount()){
    std::cout << "Invalid Field Number" << std::endl;
    return;
  }
  if( d.GetFieldType( fn ) != 'M' ){
    std::cout << "Field " << fn << " is not a memo field" << std::endl;
    std::cout << "Field Name = " << d.GetFieldName( fn ) << " type= ";
    std::cout << d.GetFieldType( fn ) << std::endl;
    return;
  }
  rc = d.UpdateMemoData( fn, 0, 0, F_SETLKW );
  std::cout << "Return Code = " << rc << std::endl;
#else
  std::cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
#ifdef XBASE_DEBUG
void MyClass::DumpDbtHeader()
{
  int rc = 0;

#ifdef XB_MEMO_FIELDS
#ifdef XBASE_DEBUG
/* FIXME?	Gary, do you mean header or free page list? */

/* - This was a hack to peek in the file for debugging purposes
     probably not to useful for anything else - Gary */

  rc = d.DumpMemoFreeChain();
  std::cout << "\nFuncion Return Code = " << rc << std::endl;
#else
  std::cout << "\nXBASE_DEBUG is not compiled in\n";
#endif
#else
  std::cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
void MyClass::GetMemoBlocks()
{
#ifdef XB_MEMO_FIELDS
  xbLong BlocksNeeded, Location, PrevNode;
  int rc;

  std::cout << "Enter number of blocks: " << std::endl;
  std::cin  >> BlocksNeeded;
  std::cout << "Enter starting location: " << std::endl;
  std::cin  >> Location;
  std::cout << "Enter previous node: " << std::endl;
  std::cin  >> PrevNode;
  rc = d.GetBlockSetFromChain( BlocksNeeded, Location, PrevNode );
  std::cout << "Return code = " << rc << std::endl;
#else
  std::cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
void MyClass::FindMemoBlocks()
{
#ifdef XB_MEMO_FIELDS
  xbLong BlocksNeeded, Location, PrevNode;
  int rc;

  std::cout << "Enter number of blocks: " << std::endl;
  std::cin  >> BlocksNeeded;
  rc = d.FindBlockSetInChain( BlocksNeeded, 0, Location, PrevNode );
  std::cout << "Return code = " << rc << std::endl;
  std::cout << "Location = " << Location << std::endl;
  std::cout << "Previous Node = " << PrevNode << std::endl;
#else
  std::cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
#endif   // XBASE_DEBUG
/************************************************************************/
void MyClass::IndexMenu()
{
  int option = 0;

  while( option != 99 ) {
   std::cout << std::endl << std::endl
             << "NDX and NTX Index Menu" << std::endl;
   std::cout << "This program only supports one index at a time" << std::endl;
   std::cout << "1 - Open Index" << std::endl;
   std::cout << "2 - Find Key" << std::endl;
   std::cout << "3 - Get First Key" << std::endl;
   std::cout << "4 - Get Last Key" << std::endl;
   std::cout << "5 - Get Previous Key" << std::endl;
   std::cout << "6 - Get Next Key" << std::endl;
   std::cout << "7 - Close Index" << std::endl;
   
   std::cout << "99 - Exit Menu" << std::endl;
   std::cin >> option;
   switch( option ){
    case 1:  OpenIndex();  break;
    case 2:  FindKey(); break;
    case 3:  GetFirstKey();  break;
    case 4:  GetLastKey();  break;
    case 5:  GetPrevKey();  break;
    case 6:  GetNextKey();  break;
    case 7:  CloseIndex();     break;    
    case 99: break;
    default: std::cout << "Invalid option" << std::endl; break;
   }
  }
}
/************************************************************************/
#ifdef XBASE_DEBUG
void MyClass::DebugMenu()
{
  int option = 0;

  while( option != 99 ) {
   std::cout << std::endl << std::endl << "Debug Menu" << std::endl;
   std::cout << "1 - Dislay DBT Header" << std::endl;
   std::cout << "2 - Find Memo Blocks" << std::endl;
   std::cout << "3 - Get Memo Blocks" << std::endl;
   std::cout << "4 - Dump index node chains to file xbase.dmp" << std::endl;
   std::cout << "99 - Exit Menu" << std::endl;
   std::cin >> option;
   switch( option ){
    case 1:  DumpDbtHeader();  break;
    case 2:  FindMemoBlocks(); break;
    case 3:  GetMemoBlocks();  break;
    case 4:  DumpIndexNodes(); break;
    case 99: break;
    default: std::cout << "Invalid option" << std::endl; break;
   }
  }
}
#endif   //  XBASE_DEBUG
/************************************************************************/
void MyClass::PutRecord()
{
  xbLong l;
  std::cout << "Enter Record number for PutRecord()" << std::endl;
  std::cin >> l;
  int rc = d.PutRecord( l );
  std::cout << "Return Code " << rc << std::endl;
}
/************************************************************************/
void MyClass::FieldMenu()
{
  int option = 0;

  while( option != 99 ) {
   std::cout << std::endl << std::endl << "Field Menu" << std::endl;
   std::cout << "1  - Delete Memo Field" << std::endl;
   std::cout << "2  - Update Memo Field" << std::endl;
   std::cout << "3  - Show Field Info"   << std::endl;
   std::cout << "4  - Show Field Data"   << std::endl;
   std::cout << "5  - Update Field Data" << std::endl;
   std::cout << "99 - Exit Menu" << std::endl;
   std::cin >> option;
   switch( option ){
    case 1:  DeleteMemoField(); break;
    case 2:  UpdateMemoField(); break;
    case 3:  ShowFieldInfo();   break;
    case 4:  ShowFieldData();   break;
    case 5:  UpdateFieldData(); break;
    case 99: break;
    default: std::cout << "Function not available" << std::endl; break;
   }
 }
}
/************************************************************************/
void MyClass::RecordMenu()
{
  int option = 0;
  std::cout << "File Menu" << std::endl;
  while( option != 99 ) {
   std::cout << std::endl << std::endl << "Record Menu" << std::endl;
   std::cout << "1  - Get Record" << std::endl;
   std::cout << "2  - Blank Record" << std::endl;
   std::cout << "3  - Append Record" << std::endl;
   std::cout << "4  - Put Record" << std::endl;
   std::cout << "99 - Exit Menu" << std::endl;
   std::cin >> option;
   switch( option ){
    case 1:  GetRecord(); break;
    case 4:  PutRecord(); break;
    case 99: break;
    default: std::cout << "Invalid option" << std::endl; break;
   }
  }
}
/************************************************************************/
void MyClass::FileMenu()
{
  int option = 0;

  while( option != 99 ) {
   std::cout << std::endl << std::endl << "File Menu" << std::endl;
   std::cout << "1  - Open File" << std::endl;
   std::cout << "2  - Close File" << std::endl;
   std::cout << "3  - File Stats" << std::endl;
   std::cout << "4  - Convert Database" << std::endl; 
   std::cout << "99 - Exit Menu" << std::endl;
   std::cin  >> option;

   switch( option ){
    case 1:  OpenFile();  break;
    case 2:  CloseFile(); break;
    case 3:  FileStats(); break;
    case 4:  ConvertDatabase(); break;
    case 99: break;
    default: std::cout << "Invalid Option" << std::endl;
   }
 }
}
/************************************************************************/
void MyClass::MainMenu()
{
  int option = 0;
  std::cout << std::endl<< std::endl << "XBase Utility Program";
  while( option != 99 ) {
   std::cout << std::endl << std::endl << "Main Menu" << std::endl;
   std::cout << "1  - File Menu" << std::endl;
   std::cout << "2  - Record Menu" << std::endl; 
   std::cout << "3  - Field Menu" << std::endl;
   std::cout << "4  - Index Menu" << std::endl;
#ifdef XBASE_DEBUG
   std::cout << "5  - Debug Menu" << std::endl;
#endif
   std::cout << "99 - Exit" << std::endl;
   std::cin >> option;
   switch( option ){
     case 1:  FileMenu();   break;
     case 2:  RecordMenu(); break;
     case 3:  FieldMenu();  break;
     case 4:  IndexMenu();  break;
#ifdef XBASE_DEBUG
     case 5:  DebugMenu();  break;
#endif
     case 99: std::cout << "Bye!! - Thanks for using XBase"
                        << std::endl; break;
     default: std::cout << "Invalid function" << std::endl; break;
   }
  }
}
/************************************************************************/
int main(int, char**)
{
  MyClass m;
  m.MainMenu();
    return 0;
}
