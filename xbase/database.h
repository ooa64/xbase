/*  $Id: database.h,v 1.5 2003/08/16 19:59:39 gkunkel Exp $

    Xbase project source code

    This file contains the definition of the xbDatabase class.

    Copyright (C) 2000 Derry Bryson (derry@techass.com)   

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact:

      Mail:

        Technology Associates, Inc.
        XBase Project
        1455 Deming Way #11
        Sparks, NV 89434
        USA

      Email:

        xbase@techass.com
        xdb-devel@lists.sourceforge.net
	xdb-user@lists.sourceforge.net

      See our website at:

        xdb.sourceforge.net
*/

#ifndef __XB_DATABASE_H__
#define __XB_DATABASE_H__

#include <xbase/xbconfig.h>
#include <xbase/xtypes.h>
#include <xbase/xdate.h>

#include <iostream>
#include <stdio.h>

#ifdef XB_INDEX_ANY

#define XBDB_MAXPATHLEN              256

#define XBDB_DBNAMELEN               100
#define XBDB_DBCOMPANY_LEN           100
#define XBDB_DBVERSION_LEN           10
#define XBDB_DBDATE_LEN              8
#define XBDB_DBXDBVERSION_LEN        10

#define XBDB_TABLEID_LEN             10
#define XBDB_TABLENAME_LEN           100
#define XBDB_DESC_LEN                100
#define XBDB_FLAG_LEN                1
#define XBDB_TABLEVERSION_LEN        1

#define XBDB_FIELDNUM_LEN            4
#define XBDB_FIELDNAME_LEN           10
#define XBDB_FIELDTYPE_LEN           1
#define XBDB_FIELDPROMPT_LEN         100
#define XBDB_FIELDMINMAX_LEN         17
#define XBDB_FIELDINPUTSPEC_LEN      50
#define XBDB_FIELDLENGTH_LEN         3
#define XBDB_FIELDDECIMALS_LEN       2

#define XBDB_INDEXNAME_LEN           50
#define XBDB_INDEXNUM_LEN            4
#define XBDB_EXPRESSION_LEN          254
#define XBDB_INDEXNODESIZE_LEN       5
#define XBDB_INDEXTYPE_LEN           1

#define XBDB_REPORTNUM_LEN           4
#define XBDB_REPORTTITLE_LEN         100
#define XBDB_REPORTKEY_LEN           100
#define XBDB_REPORTKEY_LEN           100
#define XBDB_FIELDLIST_LEN           50
#define XBDB_REPORTFORMAT_LEN        1
    
#define XBDB_TABLE_NUMINDEXES        2
#define XBDB_FIELD_NUMINDEXES        2
#define XBDB_INDEX_NUMINDEXES        2
#define XBDB_REPORT_NUMINDEXES       1

#define XBDB_INDEXTYPE_NDX           "D"
#define XBDB_INDEXTYPE_NTX           "T"

/*! \file database.h
*/

//! xbDatabase class
/*!
*/

class XBDLLEXPORT xbDatabase
{
  xbXBase
    *m_xbase;
    
  xbDbf
    *m_databaseDbf,
    *m_tableDbf,
    *m_fieldDbf,
    *m_indexDbf,
    *m_reportDbf;
    
  xbNdx
    *m_tableIndex[XBDB_TABLE_NUMINDEXES],
    *m_fieldIndex[XBDB_FIELD_NUMINDEXES],
    *m_indexIndex[XBDB_INDEX_NUMINDEXES],
    *m_reportIndex[XBDB_REPORT_NUMINDEXES];
    
  char
    m_path[XBDB_MAXPATHLEN];
    
  xbShort
    m_rc;
    
public:
  xbDatabase(xbXBase *xbase);
  ~xbDatabase();
  
  inline xbShort LastError(void) { return m_rc; }
  
  inline xbXBase *GetXBase(void) { return m_xbase; }

  inline xbDbf *GetDatabaseDbf(void) { return m_databaseDbf; }
  inline xbDbf *GetTableDbf(void) { return m_tableDbf; }
  inline xbDbf *GetFieldDbf(void) { return m_fieldDbf; }
  inline xbDbf *GetIndexDbf(void) { return m_indexDbf; }
  inline xbDbf *GetReportDbf(void) { return m_reportDbf; }

  inline xbNdx *GetTableIndex(xbShort indexNum) { return m_tableIndex[indexNum]; }
  inline xbNdx *GetFieldIndex(xbShort indexNum) { return m_fieldIndex[indexNum]; }
  inline xbNdx *GetIndexIndex(xbShort indexNum) { return m_indexIndex[indexNum]; }
  inline xbNdx *GetReportIndex(xbShort indexNum) { return m_reportIndex[indexNum]; }
      
  xbShort Open(const char *path);
  xbShort Create(const char *path, xbShort overlay = XB_DONTOVERLAY);
  void Close(void);
  const char *GetPath(void) { return m_path; }
  
  //
  //  Database stuff
  //
  xbShort GetDatabaseName(char *val);
  xbShort GetDatabaseDesc(char *val);
  xbShort GetDatabaseCompany(char *val);
  xbShort GetDatabaseVersion(char *val);
  xbShort GetDatabaseCreateDate(char *val);
  xbShort GetDatabaseModDate(char *val);
  xbShort GetDatabaseXDBVersion(char *val);

  xbShort SetDatabaseName(const char *val);
  xbShort SetDatabaseDesc(const char *dval);
  xbShort SetDatabaseCompany(const char *val);
  xbShort SetDatabaseVersion(const char *val);
  xbShort SetDatabaseCreateDate(const char *val);
  xbShort SetDatabaseModDate(const char *val);
  xbShort SetDatabaseXDBVersion(const char *val);
  
  //
  // Table stuff
  //
  xbShort GetNumTables(void);
  
  xbShort GetTableId(xbShort tableNum, char *tableId);
  xbShort GetTableFileName(const char *tableId, char *tableName);
  xbShort GetTableDesc(const char *tableId, char *desc);
  xbShort GetTableRealDelete(const char *tableId, bool *realDelete);
  xbShort GetTableVersion(const char *tableId, xbShort *version);
  
  xbShort AddTable(const char *tableId, const char *tableName, const char *desc, xbShort version = 3, bool realDelete = 0);
  xbShort RemoveTable(const char *tableId);
  xbShort ChangeTable(const char *tableId, const char *tableName, const char *desc, xbShort version = 3, bool realDelete = 0);
  bool TableExists(const char *tableId);
  void RemoveTableFiles(const char *tableId);
    
  //
  // Field stuff
  //
  xbShort GetNumFields(const char *tableId);
  xbShort LookupField(const char *tableId, const char *fieldName);
  xbShort LookupField(const char *tableId, xbShort fieldNum);
  
  xbShort GetFieldNum(const char *tableId, const char *fieldName, xbShort *fieldNum);
  xbShort GetFieldType(const char *tableId, const char *fieldName, char *type);
  xbShort GetFieldDesc(const char *tableId, const char *fieldName, char *desc);
  xbShort GetFieldPrompt(const char *tableId, const char *fieldName, char *prompt);
  xbShort GetFieldInputSpec(const char *tableId, const char *fieldName, char *inputSpec);
  xbShort GetFieldMinValue(const char *tableId, const char *fieldName, char *minValue);
  xbShort GetFieldMaxValue(const char *tableId, const char *fieldName, char *maxValue);  
  xbShort GetFieldLength(const char *tableId, const char *fieldName, xbShort *length);
  xbShort GetFieldDecimals(const char *tableId, const char *fieldName, xbShort *decimals);
  
  xbShort GetFieldName(const char *tableId, xbShort fieldNum, char *fieldName);
  xbShort GetFieldType(const char *tableId, xbShort fieldNum, char *type);
  xbShort GetFieldDesc(const char *tableId, xbShort fieldNum, char *desc);
  xbShort GetFieldPrompt(const char *tableId, xbShort fieldNum, char *prompt);
  xbShort GetFieldInputSpec(const char *tableId, xbShort fieldNum, char *inputSpec);
  xbShort GetFieldMinValue(const char *tableId, xbShort fieldNum, char *minValue);
  xbShort GetFieldMaxValue(const char *tableId, xbShort fieldNum, char *maxValue);
  xbShort GetFieldLength(const char *tableId, xbShort fieldNum, xbShort *length);
  xbShort GetFieldDecimals(const char *tableId, xbShort fieldNum, xbShort *decimals);

  xbShort AddField(const char *tableId, xbShort fieldNum, const char *fieldName, 
                   const char type, const char *desc, const char *prompt,
                   const char *inputSpec, const char *minValue, 
                   const char *maxValue, xbShort length, xbShort decimals);
  xbShort RemoveField(const char *tableId, const char *fieldName);
  xbShort RemoveField(const char *tableId, xbShort fieldNum);
  xbShort ChangeField(const char *tableId, xbShort fieldNum, const char *fieldName, 
                      const char type, const char *desc, const char *prompt,
                      const char *inputSpec, const char *minValue, 
                      const char *maxValue, xbShort length, xbShort decimals);
  
  //
  //  Index stuff
  //
  xbShort GetNumIndexes(const char *tableId);
  xbShort LookupIndex(const char *tableId, const char *indexName);
  xbShort LookupIndex(const char *tableId, xbShort indexNum);
  
  xbShort GetIndexNum(const char *tableId, const char *indexName, xbShort *num);
  xbShort GetIndexUnique(const char *tableId, const char *indexName, bool *unique);
  xbShort GetIndexExpression(const char *tableId, const char *indexName, char *expression);
  xbShort GetIndexDesc(const char *tableId, const char *indexName, char *desc);
  xbShort GetIndexNodeSize(const char *tableId, const char *indexName, xbShort *nodeSize);
  xbShort GetIndexType(const char *tableId, const char *indexName, char *type);
  
  xbShort GetIndexName(const char *tableId, xbShort indexNum, char *name);
  xbShort GetIndexUnique(const char *tableId, xbShort indexNum, bool *unique);
  xbShort GetIndexExpression(const char *tableId, xbShort indexNum, char *expression);
  xbShort GetIndexDesc(const char *tableId, xbShort indexNum, char *desc);
  xbShort GetIndexNodeSize(const char *tableId, xbShort indexNum, xbShort *nodeSize);
  xbShort GetIndexType(const char *tableId, xbShort indexNum, char *type);
  
  xbShort AddIndex(const char *tableId, xbShort indexNum, const char *indexName,
                   bool unique, const char *expression, const char *desc,
                   xbShort nodeSize, const char *type);
  xbShort RemoveIndex(const char *tableId, xbShort indexNum);
  xbShort RemoveIndex(const char *tableId, const char *indexName);
  xbShort ChangeIndex(const char *tableId, xbShort indexNum, const char *indexName,
                      bool unique, const char *expression, const char *desc,
                      xbShort nodeSize, const char *type);
  bool IndexExists(const char *tableId, xbShort indexNum);
  void RemoveIndexFile(const char *tableId, xbShort indexNum);

  //
  //  Report stuff
  //
  xbShort GetNumReports(void);
  xbShort LookupReport(xbShort reportNum);

  xbShort GetReportTableId(xbShort reportNum, char *tableId);
  xbShort GetReportTitle(xbShort reportNum, char *title);
  xbShort GetReportIndexNum(xbShort reportNum, xbShort *indexNum);
  xbShort GetReportMinKey(xbShort reportNum, char *minKey);
  xbShort GetReportMaxKey(xbShort reportNum, char *maxKey);
  xbShort GetReportQuery(xbShort reportNum, char *query);
  xbShort GetReportFieldList(xbShort reportNum, char *fieldList);
  xbShort GetReportFormat(xbShort reportNum, char *format);  

  xbShort AddReport(const char *tableId, xbShort reportNum, const char *title,
                   xbShort indexNum, const char *minKey, const char *maxKey,
                   const char *query, const char *fieldList,
                   const char *format);
  xbShort RemoveReport(xbShort reportNum);
  xbShort ChangeReport(const char *tableId, xbShort reportNum, const char *title,
                       xbShort indexNum, const char *minKey, const char *maxKey,
                       const char *query, const char *fieldList,
                       const char *format);
                        
  //
  //  Dbf stuff
  //
  xbDbf *OpenDbf(const char *tableId);
  xbDbf *CreateDbf(const char *tableId, xbShort overlay = XB_DONTOVERLAY);
  
  //
  // Misc
  //
  xbShort Pack(const char *tableId = 0,
               void (*packStatusFunc)(xbLong itemNum, xbLong numItems) = 0,
               void (*indexStatusFunc)(xbLong itemNum, xbLong numItems) = 0);
  xbShort ReIndex(const char *tableId = 0, xbShort indexNum = -1, 
                  void (*statusFunc)(xbLong itemNum, xbLong numItems) = 0);
  xbShort Zap(const char *tableId = 0);
  
private:
  char *ltrim(char *s);
  char *rtrim(char *s);

  char *MakeDbfName(const char *tableName, char *buf);  
  char *MakeIndexName(const char *tableName, xbShort indexNum, const char *suffix,
                      char *buf);
};

#endif // XB_INDEX_ANY

#endif  // __XB_DATABASE_H__
