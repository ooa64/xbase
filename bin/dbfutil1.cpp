/*  $Id: dbfutil1.cpp,v 1.5 2000/09/20 23:11:47 dbryson Exp $

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

       linux.techass.com


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
  cout << "For NDX indices, output placed in xbase.log" << endl;
  cout << "total nodes = " << NodeCnt << endl;
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
  cout << "Enter index name: ";
  cin >> Filename;
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
    cout << "Invalid index name. Needs file extension. index.ndx or index.ntx" << endl;
    return;
  }
  cout << "Return code " << rc << endl;
}    
/************************************************************************/
void MyClass::FindKey()
{
  char KeyBuf[101];
  memset( KeyBuf, 0x00, 101 );
  cout << "This function only works for character keys (so far).."<<endl;
  cout << "Enter search key:" << endl;
  cin >> KeyBuf;
  cout <<  "Return code " << ix->FindKey( KeyBuf ) << endl;
}
/************************************************************************/
void MyClass::GetNextKey()
{
  cout << "Return code " << ix->GetNextKey() << endl;
}
/************************************************************************/
void MyClass::GetPrevKey()
{
  cout << "Return code " << ix->GetPrevKey() << endl;
}
/************************************************************************/
void MyClass::GetFirstKey()
{
  cout << "Return code " << ix->GetFirstKey() << endl;
}
/************************************************************************/
void MyClass::GetLastKey()
{
  cout << "Return code " << ix->GetLastKey() << endl;
}
/************************************************************************/
void MyClass::CloseIndex()
{
  cout << "Return code " << ix->CloseIndex() << endl;
}
/************************************************************************/
/* stats */
void MyClass::FileStats()
{
  cout << "Number of records   = " << d.NoOfRecords()       << endl;
  cout << "Number of fields    = " << d.FieldCount()        << endl;
  cout << "Database Status     = " << d.GetDbfStatus()      << endl;
  cout << "Record Length       = " << d.GetRecordLen()      << endl;
  cout << "Memo Fields Present = " << d.MemoFieldsPresent() << endl;
  cout << "Current Record No   = " << d.GetCurRecNo()       << endl;
}  
/************************************************************************/
/* open database */
void MyClass::OpenFile()
{
  int rc;
  char filename[50];
  cout << "Enter database file name: ";
  cin >> filename;

  rc = d.OpenDatabase( filename );
  cout << "Return Code " << rc << endl;
}
/************************************************************************/
/* close database */
void MyClass::CloseFile()
{
  int rc;
  rc = d.CloseDatabase();
  cout << "Return Code " << rc << endl;
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

  cout << "Warning - check your results!!" << endl << endl;

  char filename[50];
  cout << "Enter database file name to convert to: ";
  cin >> filename;

  if(( rc = d2.OpenDatabase( filename )) != XB_NO_ERROR ){
    cout << "Error " << rc << " opening database" << endl;
    return;
  }

  rc = d.GetFirstRecord();
  FieldCnt = d.FieldCount();
  cout << "Database has " << FieldCnt << " fields to convert" << endl;
  while( rc == XB_NO_ERROR )
  {
    d2.BlankRecord();
    for( xbShort i = 0; i < FieldCnt; i++ )
    {
      if(( FieldNo = d2.GetFieldNo( d.GetFieldName( i ))) == -1 )
        cout << "Error converting field " << d.GetFieldName( i ) << endl;
      else 
      {
        if( d.GetFieldType( i ) != d2.GetFieldType( FieldNo ))
          cout << "Error - different field types " << d.GetFieldName(i);
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
           cout << "Memo fields not converted XB_MEMO_FIELDS not compiled in\n";
#endif
          }
          else{
            if( d.GetFieldType(i) == 'N' ){
                cout << "const code compile prob..." << endl;

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
  cout << "Number   Name        Type  Len  Decs" << endl;
  for( int i = 0; i < d.FieldCount(); i++ ){
    printf( "%d     %-10s         %c     %5d   %d\n", i,
              d.GetFieldName(i), d.GetFieldType(i), d.GetFieldLen(i),
              d.GetFieldDecimal(i) );
    if(((i+1)%15) == 0 ){
      cout << "Press a key to continue" << endl;
      getchar();
      cout << "Number   Name        Type  Len  Decs" << endl;
    }
  } 
  cout << "Press a key to continue" << endl;
  getchar();
}
/************************************************************************/
void MyClass::ShowFieldData()
{
  cout << "Name      Data" << endl;
  for( int i = 0; i < d.FieldCount(); i++ ){
    printf( "%-10s = %s\n", d.GetFieldName(i), d.GetStringField(i));
    if(((i+1)%15) == 0 ){
      cout << "Press a key to continue" << endl;
      getchar();
      cout << "Name      Data" << endl;
    }
  }
  cout << "Press a key to continue" << endl;
  getchar();
   
}
/************************************************************************/
void MyClass::UpdateFieldData()
{
  char dbuf[100];
  int rc, fn;

  cout  << "Enter Field Number to update (first field is 0): " << endl;
  cin   >> fn;
  cout  << "Enter Field Data" << endl;
  cin   >> dbuf;
  rc = d.PutField( fn, dbuf );
  
  cout << "Return Code " << rc << endl;
}
/************************************************************************/
/* open database */
void MyClass::ReindexDatabase()
{
  xbNdx n(&d);
  int rc;
  char filename[50];
  cout << "Enter index file name: ";
  cin >> filename;

  if(( rc = n.OpenIndex( filename )) != XB_NO_ERROR ){
    cout << "Error " << rc << " Opening index" << endl;
    return;
  }

  if(( rc = n.ReIndex()) != XB_NO_ERROR )
    cout << "Error " << rc << " Reindexing database" << endl;
}
/************************************************************************/
void MyClass::GetRecord()
{
  int rc;
  xbLong RecNo;
  cout << "Enter record number: ";
  cin >> RecNo;

  rc = d.GetRecord( RecNo );
  cout << "Return Code = " << rc << endl;
}
/************************************************************************/
void MyClass::UpdateMemoField()
{
#ifdef XB_MEMO_FIELDS
  xbShort fn,rc;
  xbLong len;
  char bufchar[2];
  char *buf;

  cout << "Enter Field Number: " << endl;
  cin >> fn;
  if( fn < 0 || fn > d.FieldCount()){
    cout << "Invalid Field Number" << endl;
    return;
  }
  if( d.GetFieldType( fn ) != 'M' ){
    cout << "Field " << fn << " is not a memo field" << endl;
    cout << "Field Name = " << d.GetFieldName( fn ) << " type= ";
    cout << d.GetFieldType( fn ) << endl;
    return;
  }
  cout << "Enter length of memo data:" << endl;
  cin >> len;
 
  cout << "Enter character to populate memo data:" << endl;
  cin >> bufchar;

  buf = new char[len];
  memset( buf, bufchar[0], len );

  rc = d.UpdateMemoData( fn, len, buf, F_SETLKW );
  cout << "Return Code = " << rc << endl;
#else
  cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
void MyClass::DeleteMemoField()
{
#ifdef XB_MEMO_FIELDS
  xbShort fn,rc;
  cout << "Enter Field Number: " << endl;
  cin >> fn;
  if( fn < 0 || fn > d.FieldCount()){
    cout << "Invalid Field Number" << endl;
    return;
  }
  if( d.GetFieldType( fn ) != 'M' ){
    cout << "Field " << fn << " is not a memo field" << endl;
    cout << "Field Name = " << d.GetFieldName( fn ) << " type= ";
    cout << d.GetFieldType( fn ) << endl;
    return;
  }
  rc = d.UpdateMemoData( fn, 0, 0, F_SETLKW );
  cout << "Return Code = " << rc << endl;
#else
  cout << "\nXB_MEMO_FIELDS is not compiled in\n";
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
  cout << "\nFuncion Return Code = " << rc << endl;
#else
  cout << "\nXBASE_DEBUG is not compiled in\n";
#endif
#else
  cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
void MyClass::GetMemoBlocks()
{
#ifdef XB_MEMO_FIELDS
  xbLong BlocksNeeded, Location, PrevNode;
  int rc;

  cout << "Enter number of blocks: " << endl;
  cin  >> BlocksNeeded;
  cout << "Enter starting location: " << endl;
  cin  >> Location;
  cout << "Enter previous node: " << endl;
  cin  >> PrevNode;
  rc = d.GetBlockSetFromChain( BlocksNeeded, Location, PrevNode );
  cout << "Return code = " << rc << endl;
#else
  cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
/************************************************************************/
void MyClass::FindMemoBlocks()
{
#ifdef XB_MEMO_FIELDS
  xbLong BlocksNeeded, Location, PrevNode;
  int rc;

  cout << "Enter number of blocks: " << endl;
  cin  >> BlocksNeeded;
  rc = d.FindBlockSetInChain( BlocksNeeded, 0, Location, PrevNode );
  cout << "Return code = " << rc << endl;
  cout << "Location = " << Location << endl;
  cout << "Previous Node = " << PrevNode << endl;
#else
  cout << "\nXB_MEMO_FIELDS is not compiled in\n";
#endif
}
#endif   // XBASE_DEBUG
/************************************************************************/
void MyClass::IndexMenu()
{
  int option = 0;

  while( option != 99 ) {
   cout << endl << endl << "NDX and NTX Index Menu" << endl;
   cout << "This program only supports one index at a time" << endl;
   cout << "1 - Open Index" << endl;
   cout << "2 - Find Key" << endl;
   cout << "3 - Get First Key" << endl;
   cout << "4 - Get Last Key" << endl;
   cout << "5 - Get Previous Key" << endl;
   cout << "6 - Get Next Key" << endl;
   cout << "7 - Close Index" << endl;
   
   cout << "99 - Exit Menu" << endl;
   cin >> option;
   switch( option ){
    case 1:  OpenIndex();  break;
    case 2:  FindKey(); break;
    case 3:  GetFirstKey();  break;
    case 4:  GetLastKey();  break;
    case 5:  GetPrevKey();  break;
    case 6:  GetNextKey();  break;
    case 7:  CloseIndex();     break;    
    case 99: break;
    default: cout << "Invalid option" << endl; break;
   }
  }
}
/************************************************************************/
#ifdef XBASE_DEBUG
void MyClass::DebugMenu()
{
  int option = 0;

  while( option != 99 ) {
   cout << endl << endl << "Debug Menu" << endl;
   cout << "1 - Dislay DBT Header" << endl;
   cout << "2 - Find Memo Blocks" << endl;
   cout << "3 - Get Memo Blocks" << endl;
   cout << "4 - Dump index node chains to file xbase.dmp" << endl;
   cout << "99 - Exit Menu" << endl;
   cin >> option;
   switch( option ){
    case 1:  DumpDbtHeader();  break;
    case 2:  FindMemoBlocks(); break;
    case 3:  GetMemoBlocks();  break;
    case 4:  DumpIndexNodes(); break;
    case 99: break;
    default: cout << "Invalid option" << endl; break;
   }
  }
}
#endif   //  XBASE_DEBUG
/************************************************************************/
void MyClass::PutRecord()
{
  xbLong l;
  cout << "Enter Record number for PutRecord()" << endl;
  cin >> l;
  int rc = d.PutRecord( l );
  cout << "Return Code " << rc << endl;
}
/************************************************************************/
void MyClass::FieldMenu()
{
  int option = 0;

  while( option != 99 ) {
   cout << endl << endl << "Field Menu" << endl;
   cout << "1  - Delete Memo Field" << endl;
   cout << "2  - Update Memo Field" << endl;
   cout << "3  - Show Field Info"   << endl;
   cout << "4  - Show Field Data"   << endl;
   cout << "5  - Update Field Data" << endl;
   cout << "99 - Exit Menu" << endl;
   cin >> option;
   switch( option ){
    case 1:  DeleteMemoField(); break;
    case 2:  UpdateMemoField(); break;
    case 3:  ShowFieldInfo();   break;
    case 4:  ShowFieldData();   break;
    case 5:  UpdateFieldData(); break;
    case 99: break;
    default: cout << "Function not available" << endl; break;
   }
 }
}
/************************************************************************/
void MyClass::RecordMenu()
{
  int option = 0;
  cout << "File Menu" << endl;
  while( option != 99 ) {
   cout << endl << endl << "Record Menu" << endl;
   cout << "1  - Get Record" << endl;
   cout << "2  - Blank Record" << endl;
   cout << "3  - Append Record" << endl;
   cout << "4  - Put Record" << endl;
   cout << "99 - Exit Menu" << endl;
   cin >> option;
   switch( option ){
    case 1:  GetRecord(); break;
    case 4:  PutRecord(); break;
    case 99: break;
    default: cout << "Invalid option" << endl; break;
   }
  }
}
/************************************************************************/
void MyClass::FileMenu()
{
  int option = 0;

  while( option != 99 ) {
   cout << endl << endl << "File Menu" << endl;
   cout << "1  - Open File" << endl;
   cout << "2  - Close File" << endl;
   cout << "3  - File Stats" << endl;
   cout << "4  - Convert Database" << endl; 
   cout << "99 - Exit Menu" << endl;
   cin  >> option;

   switch( option ){
    case 1:  OpenFile();  break;
    case 2:  CloseFile(); break;
    case 3:  FileStats(); break;
    case 4:  ConvertDatabase(); break;
    case 99: break;
    default: cout << "Invalid Option" << endl;
   }
 }
}
/************************************************************************/
void MyClass::MainMenu()
{
  int option = 0;
  cout << endl<< endl << "XBase Utility Program";
  while( option != 99 ) {
   cout << endl << endl << "Main Menu" << endl;
   cout << "1  - File Menu" << endl;
   cout << "2  - Record Menu" << endl; 
   cout << "3  - Field Menu" << endl;
   cout << "4  - Index Menu" << endl;
#ifdef XBASE_DEBUG
   cout << "5  - Debug Menu" << endl;
#endif
   cout << "99 - Exit" << endl;
   cin >> option;
   switch( option ){
     case 1:  FileMenu();   break;
     case 2:  RecordMenu(); break;
     case 3:  FieldMenu();  break;
     case 4:  IndexMenu();  break;
#ifdef XBASE_DEBUG
     case 5:  DebugMenu();  break;
#endif
     case 99: cout << "Bye!! - Thanks for using XBase" << endl; break;
     default: cout << "Invalid function" << endl; break;
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
