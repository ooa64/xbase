/*  $Id: database.cpp,v 1.4 2003/08/16 19:59:39 gkunkel Exp $

    Xbase project source code
   
    This file contains the implementation of the xbDatabase class.

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
	xdb-users@lists.sourceforge.net


      See our website at:

        xdb.sourceforge.net
*/

#include <xbase/xbconfig.h>
#include <xbase/xbase.h>

#ifdef HAVE_IO_H
#include <io.h>
#endif
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef __WIN32__
#include <unistd.h>
#endif

#include "xbexcept.h"
#include "database.h"

/*! \file database.cpp
*/

#define NOTOPENERROR    XB_INVALID_RECORD

#ifdef XB_INDEX_ANY

static xbSchema 
  DatabaseRec[] = 
  {
    { "NAME", XB_CHAR_FLD, XBDB_DBNAMELEN, 0 },
    { "DESC", XB_CHAR_FLD, XBDB_DESC_LEN, 0 },
    { "COMPANY", XB_CHAR_FLD, XBDB_DBCOMPANY_LEN, 0 },
    { "VERSION", XB_CHAR_FLD, XBDB_DBVERSION_LEN, 0 },
    { "CREATEDATE", XB_CHAR_FLD, XBDB_DBDATE_LEN, 0 },
    { "MODDATE", XB_CHAR_FLD, XBDB_DBDATE_LEN, 0 },
    { "XDBVERSION", XB_CHAR_FLD, XBDB_DBXDBVERSION_LEN, 0 },
    { "", 0, 0, 0 }
  };
  
#define XBDB_DATABASE_NAME            (xbShort)0
#define XBDB_DATABASE_DESC            1
#define XBDB_DATABASE_COMPANY         2
#define XBDB_DATABASE_VERSION         3
#define XBDB_DATABASE_CREATEDATE      4
#define XBDB_DATABASE_MODDATE         5
#define XBDB_DATABASE_XDBVERSION      6
  
static xbSchema 
  TableRec[] = 
  {
    { "TABLEID", XB_CHAR_FLD, XBDB_TABLEID_LEN, 0 },
    { "TABLENAME", XB_CHAR_FLD, XBDB_TABLENAME_LEN, 0 },
    { "DESC", XB_CHAR_FLD, XBDB_DESC_LEN, 0 },
    { "REALDELETE", XB_LOGICAL_FLD, XBDB_FLAG_LEN, 0 },
    { "VERSION", XB_CHAR_FLD, XBDB_TABLEVERSION_LEN, 0 },
    { "", 0, 0, 0 }
  };
  
static char
  *TableIndexExps[XBDB_TABLE_NUMINDEXES] =
  {
    "TABLEID",
    "TABLENAME"
  };
  
#define XBDB_TABLE_TABLEID            (xbShort)0
#define XBDB_TABLE_TABLENAME          1
#define XBDB_TABLE_DESC               2
#define XBDB_TABLE_REALDELETE         3
#define XBDB_TABLE_VERSION            4
  
static xbSchema
  FieldRec[] =
  {
    { "TABLEID", XB_CHAR_FLD, XBDB_TABLEID_LEN, 0 },
    { "FIELDNUM", XB_CHAR_FLD, XBDB_FIELDNUM_LEN, 0 },
    { "FIELDNAME", XB_CHAR_FLD, XBDB_FIELDNAME_LEN, 0 },
    { "TYPE", XB_CHAR_FLD, XBDB_FIELDTYPE_LEN, 0 },
    { "DESC", XB_CHAR_FLD, XBDB_DESC_LEN, 0 },
    { "PROMPT", XB_CHAR_FLD, XBDB_FIELDPROMPT_LEN, 0 },
    { "INPUTSPEC", XB_CHAR_FLD, XBDB_FIELDINPUTSPEC_LEN, 0 },
    { "MINVALUE", XB_CHAR_FLD, XBDB_FIELDMINMAX_LEN, 0 },
    { "MAXVALUE", XB_CHAR_FLD, XBDB_FIELDMINMAX_LEN, 0 },
    { "LENGTH", XB_CHAR_FLD, XBDB_FIELDLENGTH_LEN, 0 },
    { "DECIMALS", XB_CHAR_FLD, XBDB_FIELDDECIMALS_LEN, 0 },
    { "", 0, 0, 0 }
  };
  
static char
  *FieldIndexExps[XBDB_FIELD_NUMINDEXES] =
  {
    "TABLEID+FIELDNUM",
    "TABLEID+FIELDNAME"
  };
  
#define XBDB_FIELD_TABLEID           (xbShort)0
#define XBDB_FIELD_FIELDNUM          1
#define XBDB_FIELD_FIELDNAME         2
#define XBDB_FIELD_TYPE              3
#define XBDB_FIELD_DESC              4
#define XBDB_FIELD_PROMPT            5
#define XBDB_FIELD_INPUTSPEC         6
#define XBDB_FIELD_MINVALUE          7
#define XBDB_FIELD_MAXVALUE          8
#define XBDB_FIELD_LENGTH            9
#define XBDB_FIELD_DECIMALS          10
  
static xbSchema
  IndexRec[] =
  {
    { "TABLEID", XB_CHAR_FLD, XBDB_TABLEID_LEN, 0 },
    { "INDEXNUM", XB_CHAR_FLD, XBDB_INDEXNUM_LEN, 0 },
    { "INDEXNAME", XB_CHAR_FLD, XBDB_INDEXNAME_LEN, 0 },
    { "UNIQUE", XB_LOGICAL_FLD, XBDB_FLAG_LEN, 0 },
    { "EXP", XB_CHAR_FLD, XBDB_EXPRESSION_LEN, 0 },
    { "DESC", XB_CHAR_FLD, XBDB_DESC_LEN, 0 },
    { "NODESIZE", XB_CHAR_FLD, XBDB_INDEXNODESIZE_LEN, 0 },
    { "INDEXTYPE", XB_CHAR_FLD, XBDB_INDEXTYPE_LEN, 0 },
    { "", 0, 0, 0 }
  };
  
static char
  *IndexIndexExps[XBDB_INDEX_NUMINDEXES] =
  {
    "TABLEID+INDEXNUM",
    "TABLEID+INDEXNAME"
  };
    
#define XBDB_INDEX_TABLEID           (xbShort)0
#define XBDB_INDEX_INDEXNUM          1
#define XBDB_INDEX_INDEXNAME         2
#define XBDB_INDEX_UNIQUE            3
#define XBDB_INDEX_EXPRESSION        4
#define XBDB_INDEX_DESC              5
#define XBDB_INDEX_NODESIZE          6
#define XBDB_INDEX_TYPE              7

static xbSchema
  ReportRec[] =
  {
    { "TABLEID", XB_CHAR_FLD, XBDB_TABLEID_LEN, 0 },
    { "REPORTNUM", XB_CHAR_FLD, XBDB_REPORTNUM_LEN, 0 },
    { "TITLE", XB_CHAR_FLD, XBDB_REPORTTITLE_LEN, 0 },
    { "INDEXNUM", XB_CHAR_FLD, XBDB_INDEXNUM_LEN, 0 },
    { "MINKEY", XB_CHAR_FLD, XBDB_REPORTKEY_LEN, 0 },
    { "MAXKEY", XB_CHAR_FLD, XBDB_REPORTKEY_LEN, 0 },
    { "QUERY", XB_CHAR_FLD, XBDB_EXPRESSION_LEN, 0 },
    { "FIELDLIST", XB_CHAR_FLD, XBDB_FIELDLIST_LEN, 0 },
    { "FORMAT", XB_CHAR_FLD, XBDB_REPORTFORMAT_LEN, 0 },    
    { "", 0, 0, 0 }
  };
  
static char
  *ReportIndexExps[XBDB_REPORT_NUMINDEXES] =
  {
    "REPORTNUM"
  };
  
#define XBDB_REPORT_TABLEID             (xbShort)0
#define XBDB_REPORT_REPORTNUM           1
#define XBDB_REPORT_TITLE               2
#define XBDB_REPORT_INDEXNUM            3
#define XBDB_REPORT_MINKEY              4
#define XBDB_REPORT_MAXKEY              5
#define XBDB_REPORT_QUERY               6
#define XBDB_REPORT_FIELDLIST           7
#define XBDB_REPORT_FORMAT              8
    
//! Short description
/*!
  \param xbase
*/
xbDatabase::xbDatabase(xbXBase *xbase)
{
  int
    i;

  m_xbase = xbase;
  
  m_databaseDbf = 0;
  m_tableDbf = 0;
  m_fieldDbf = 0;
  m_indexDbf = 0;
  m_reportDbf = 0;
  
  for(i = 0; i < XBDB_TABLE_NUMINDEXES; i++)
    m_tableIndex[i] = 0;
  for(i = 0; i < XBDB_FIELD_NUMINDEXES; i++)
    m_fieldIndex[i] = 0;
  for(i = 0; i < XBDB_INDEX_NUMINDEXES; i++)
    m_indexIndex[i] = 0;
  for(i = 0; i < XBDB_REPORT_NUMINDEXES; i++)
    m_reportIndex[i] = 0;
  
  m_path[0] = 0;
  m_rc = 0;
}

//! Short description
/*!
*/
xbDatabase::~xbDatabase()
{
  if(m_databaseDbf)
    m_databaseDbf->CloseDatabase(1);
  if(m_tableDbf)
    m_tableDbf->CloseDatabase(1);
  if(m_fieldDbf)
    m_fieldDbf->CloseDatabase(1);
  if(m_indexDbf)
    m_indexDbf->CloseDatabase(1);
  if(m_reportDbf)
    m_reportDbf->CloseDatabase(1);
}

//! Short description
/*!
  \param path
*/
xbShort 
xbDatabase::Open(const char *path)
{
  xbShort
    i;
    
  char
    tableName[1024];

  strncpy(m_path, path, XBDB_MAXPATHLEN);

  m_databaseDbf = new xbDbf(m_xbase);
  if(m_databaseDbf)
  {
    sprintf(tableName, "%s/database.dbf", path);
    m_rc = m_databaseDbf->OpenDatabase(tableName);
    if(m_rc)
      goto Outtahere;
      
    m_databaseDbf->RealDeleteOn();
  }
  else
  {
    m_rc = XB_NO_MEMORY;
    goto Outtahere;
  }
      
  m_tableDbf = new xbDbf(m_xbase);
  if(m_tableDbf)
  {
    sprintf(tableName, "%s/table.dbf", path);
    m_rc = m_tableDbf->OpenDatabase(tableName);
    if(m_rc)
      goto Outtahere;
      
    m_tableDbf->RealDeleteOn();
      
    for(i = 0; i < XBDB_TABLE_NUMINDEXES; i++)
    {
      m_tableIndex[i] = new xbNdx(m_tableDbf);
      if(m_tableIndex[i])
      {
        sprintf(tableName, "%s/table%d.ndx", path, i);
        m_rc = m_tableIndex[i]->OpenIndex(tableName);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
  else
  {
    m_rc = XB_NO_MEMORY;
    goto Outtahere;
  }
      
  m_fieldDbf = new xbDbf(m_xbase);
  if(m_fieldDbf)
  {
    sprintf(tableName, "%s/field.dbf", path);
    m_rc = m_fieldDbf->OpenDatabase(tableName);
    if(m_rc)
      goto Outtahere;
      
    m_fieldDbf->RealDeleteOn();
    
    for(i = 0; i < XBDB_FIELD_NUMINDEXES; i++)
    {
      m_fieldIndex[i] = new xbNdx(m_fieldDbf);
      if(m_fieldIndex[i])
      {
        sprintf(tableName, "%s/field%d.ndx", path, i);
        m_rc = m_fieldIndex[i]->OpenIndex(tableName);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
      
  m_indexDbf = new xbDbf(m_xbase);
  if(m_indexDbf)
  {
    sprintf(tableName, "%s/index.dbf", path);
    m_rc = m_indexDbf->OpenDatabase(tableName);
    if(m_rc)
      goto Outtahere;
      
    m_indexDbf->RealDeleteOn();
    
    for(i = 0; i < XBDB_INDEX_NUMINDEXES; i++)
    {
      m_indexIndex[i] = new xbNdx(m_indexDbf);
      if(m_indexIndex[i])
      {
        sprintf(tableName, "%s/index%d.ndx", path, i);
        m_rc = m_indexIndex[i]->OpenIndex(tableName);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
      
  m_reportDbf = new xbDbf(m_xbase);
  if(m_reportDbf)
  {
    sprintf(tableName, "%s/report.dbf", path);
    m_rc = m_reportDbf->OpenDatabase(tableName);
    if(m_rc)
      goto Outtahere;
      
    m_reportDbf->RealDeleteOn();
    
    for(i = 0; i < XBDB_REPORT_NUMINDEXES; i++)
    {
      m_reportIndex[i] = new xbNdx(m_reportDbf);
      if(m_reportIndex[i])
      {
        sprintf(tableName, "%s/report%d.ndx", path, i);
        m_rc = m_reportIndex[i]->OpenIndex(tableName);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
      
Outtahere:
  if(m_rc)
    Close();
      
  return m_rc;
}

//! Short description
/*!
  \param path
  \param overlay
*/
xbShort 
xbDatabase::Create(const char *path, xbShort overlay)
{
  xbShort
    i;
    
  char
    tableName[1024];
    
  strncpy(m_path, path, XBDB_MAXPATHLEN);
      
  m_databaseDbf = new xbDbf(m_xbase);
  if(m_databaseDbf)
  {
    sprintf(tableName, "%s/database.dbf", path);
    m_rc = m_databaseDbf->CreateDatabase(tableName, DatabaseRec, overlay);
    if(m_rc)
      goto Outtahere;
      
    m_databaseDbf->RealDeleteOn();
    
    m_databaseDbf->BlankRecord();
    m_databaseDbf->PutField(XBDB_DATABASE_XDBVERSION, XDB_VERSION);
    m_rc = m_databaseDbf->AppendRecord();
    if(m_rc)
      goto Outtahere;
  }
  else
  {
    m_rc = XB_NO_MEMORY;
    goto Outtahere;
  }
      
  m_tableDbf = new xbDbf(m_xbase);
  if(m_tableDbf)
  {
    sprintf(tableName, "%s/table.dbf", path);
    m_rc = m_tableDbf->CreateDatabase(tableName, TableRec, overlay);
    if(m_rc)
      goto Outtahere;
      
    m_tableDbf->RealDeleteOn();
      
    for(i = 0; i < XBDB_TABLE_NUMINDEXES; i++)
    {
      m_tableIndex[i] = new xbNdx(m_tableDbf);
      if(m_tableIndex[i])
      {
        sprintf(tableName, "%s/table%d.ndx", path, i);
        m_rc = m_tableIndex[i]->CreateIndex(tableName, TableIndexExps[i], XB_UNIQUE, overlay);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
  else
  {
    m_rc = XB_NO_MEMORY;
    goto Outtahere;
  }
      
  m_fieldDbf = new xbDbf(m_xbase);
  if(m_fieldDbf)
  {
    sprintf(tableName, "%s/field.dbf", path);
    m_rc = m_fieldDbf->CreateDatabase(tableName, FieldRec, overlay);
    if(m_rc)
      goto Outtahere;
      
    m_fieldDbf->RealDeleteOn();
    
    for(i = 0; i < XBDB_FIELD_NUMINDEXES; i++)
    {
      m_fieldIndex[i] = new xbNdx(m_fieldDbf);
      if(m_fieldIndex[i])
      {
        sprintf(tableName, "%s/field%d.ndx", path, i);
        m_rc = m_fieldIndex[i]->CreateIndex(tableName, FieldIndexExps[i], XB_UNIQUE, overlay);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
      
  m_indexDbf = new xbDbf(m_xbase);
  if(m_indexDbf)
  {
    sprintf(tableName, "%s/index.dbf", path);
    m_rc = m_indexDbf->CreateDatabase(tableName, IndexRec, overlay);
    if(m_rc)
      goto Outtahere;
      
    m_indexDbf->RealDeleteOn();
    
    for(i = 0; i < XBDB_INDEX_NUMINDEXES; i++)
    {
      m_indexIndex[i] = new xbNdx(m_indexDbf);
      if(m_indexIndex[i])
      {
        sprintf(tableName, "%s/index%d.ndx", path, i);
        m_rc = m_indexIndex[i]->CreateIndex(tableName, IndexIndexExps[i], XB_UNIQUE, overlay);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
      
  m_reportDbf = new xbDbf(m_xbase);
  if(m_reportDbf)
  {
    sprintf(tableName, "%s/report.dbf", path);
    m_rc = m_reportDbf->CreateDatabase(tableName, ReportRec, overlay);
    if(m_rc)
      goto Outtahere;
      
    m_reportDbf->RealDeleteOn();
    
    for(i = 0; i < XBDB_REPORT_NUMINDEXES; i++)
    {
      m_reportIndex[i] = new xbNdx(m_reportDbf);
      if(m_reportIndex[i])
      {
        sprintf(tableName, "%s/report%d.ndx", path, i);
        m_rc = m_reportIndex[i]->CreateIndex(tableName, ReportIndexExps[i], XB_UNIQUE, overlay);
        if(m_rc)
          goto Outtahere;
      }
      else
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
    }
  }
      
Outtahere:
  if(m_rc)
    Close();
      
  return m_rc;
}

//! Short description
/*!
*/
void 
xbDatabase::Close(void)
{
  if(m_databaseDbf)
  {
    m_databaseDbf->CloseDatabase(1);
    delete m_databaseDbf;
    m_databaseDbf = 0;
  }
  if(m_tableDbf)
  {
    m_tableDbf->CloseDatabase(1);
    delete m_tableDbf;
    m_tableDbf = 0;
  }
  if(m_fieldDbf)
  {
    m_fieldDbf->CloseDatabase(1);
    delete m_fieldDbf;
    m_fieldDbf = 0;
  }
  if(m_indexDbf)
  {
    m_indexDbf->CloseDatabase(1);
    delete m_indexDbf;
    m_indexDbf = 0;
  }
  if(m_reportDbf)
  {
    m_reportDbf->CloseDatabase(1);
    delete m_reportDbf;
    m_reportDbf = 0;
  }
}

//
// Database stuff
//
//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseName(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_NAME, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseDesc(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_DESC, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseCompany(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_COMPANY, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseVersion(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_VERSION, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseCreateDate(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_CREATEDATE, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseModDate(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_MODDATE, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::GetDatabaseXDBVersion(char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
      m_rc = m_databaseDbf->GetField(XBDB_DATABASE_XDBVERSION, val);
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseName(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_NAME, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseDesc(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_DESC, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseCompany(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_COMPANY, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseVersion(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_VERSION, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseCreateDate(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_CREATEDATE, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseModDate(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_MODDATE, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//! Short description
/*!
  \param val
*/
xbShort 
xbDatabase::SetDatabaseXDBVersion(const char *val)
{
  m_rc = NOTOPENERROR;
  
  if(m_databaseDbf)
  {
    m_rc = m_databaseDbf->GetFirstRecord();
    if(!m_rc)
    {
      m_databaseDbf->PutField(XBDB_DATABASE_XDBVERSION, val);
      m_rc = m_databaseDbf->PutRecord();
    }
  }
  
  return m_rc;
}

//
// Table stuff
//
//! Short description
/*!
*/
xbShort 
xbDatabase::GetNumTables(void)
{
  if(m_tableDbf)
    return m_tableDbf->NoOfRecords();
  return 0;
}
  
//! Short description
/*!
  \param tableNum
  \param tableId
*/
xbShort 
xbDatabase::GetTableId(xbShort tableNum, char *tableId)
{
  xbShort
    i = 0;
    
  m_rc = NOTOPENERROR;
    
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->GetFirstKey();
    while(i < tableNum && !m_rc)
    {
      i++;
      m_rc = m_tableIndex[0]->GetNextKey();
    }
    
    if(!m_rc)
    {
      m_rc = 0;
      m_tableDbf->GetField(XBDB_TABLE_TABLEID, tableId);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param tableName
*/
xbShort 
xbDatabase::GetTableFileName(const char *tableId, char *tableName)
{
  m_rc = NOTOPENERROR;
    
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_tableDbf->GetField(XBDB_TABLE_TABLENAME, tableName);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param desc
*/
xbShort 
xbDatabase::GetTableDesc(const char *tableId, char *desc)
{
  m_rc = NOTOPENERROR;
    
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_tableDbf->GetField(XBDB_TABLE_DESC, desc);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param realDelete
*/
xbShort 
xbDatabase::GetTableRealDelete(const char *tableId, bool *realDelete)
{
  char
    value[2];
    
  m_rc = NOTOPENERROR;
    
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_tableDbf->GetField(XBDB_TABLE_REALDELETE, value);
      if(value[0] == 'y' || value[0] == 'Y' || 
         value[0] == 't' || value[0] == 'T')
        *realDelete = 1;
      else
        *realDelete = 0;
    }
  }
  return m_rc;    
}
  
//! Short description
/*!
  \param tableId
  \param version
*/
xbShort 
xbDatabase::GetTableVersion(const char *tableId, xbShort *version)
{
  char
    value[2];
    
  m_rc = NOTOPENERROR;
    
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_tableDbf->GetField(XBDB_TABLE_VERSION, value);
      *version = atoi(value);
    }
  }
  return m_rc;    
}
  
//! Short description
/*!
  \param tableId
  \param tableName
  \param desc
  \param version
  \param realDelete
*/
xbShort 
xbDatabase::AddTable(const char *tableId, const char *tableName, const char *desc, 
                    xbShort version, bool realDelete)
{
  m_rc = NOTOPENERROR;
    
  char
    value[2];
    
  if(m_tableDbf)
  {
    if((m_rc = m_tableDbf->PutField(XBDB_TABLE_TABLEID, tableId)) != 0)
      goto Outtahere;
    if((m_rc = m_tableDbf->PutField(XBDB_TABLE_TABLENAME, tableName)) != 0)
      goto Outtahere;
    if((m_rc = m_tableDbf->PutField(XBDB_TABLE_DESC, desc)) != 0)
      goto Outtahere;
    if((m_rc = m_tableDbf->PutField(XBDB_TABLE_REALDELETE, realDelete ? "T" : "F")) != 0)
      goto Outtahere;
    sprintf(value, "%1.1d", version);
    if((m_rc = m_tableDbf->PutField(XBDB_TABLE_VERSION, value)) != 0)
      goto Outtahere;
      
    m_rc = m_tableDbf->AppendRecord();
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
*/
xbShort 
xbDatabase::RemoveTable(const char *tableId)
{
  xbShort
    i,
    num;
    
  m_rc = NOTOPENERROR;
    
  if(m_tableDbf)
  {
    //
    //  Remove the files
    //
    RemoveTableFiles(tableId);
    
    //
    //  Remove the fields
    //
    num = GetNumFields(tableId);
    for(i = 0; !m_rc && i < num; i++)
      m_rc = RemoveField(tableId, i);
      
    //
    //  Remove the indexes
    //
    num = GetNumIndexes(tableId);
    for(i = num - 1; !m_rc && i >= 0; i--)
      m_rc = RemoveIndex(tableId, i);
      
    //
    //  Remove the table record
    //
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
      m_rc = m_tableDbf->DeleteRecord();
  }
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param tableName
  \param desc
  \param version
  \param realDelete
*/
xbShort 
xbDatabase::ChangeTable(const char *tableId, const char *tableName, 
                       const char *desc, xbShort version, bool realDelete)
{
  m_rc = NOTOPENERROR;
    
  char
    value[2];
    
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      if((m_rc = m_tableDbf->PutField(XBDB_TABLE_TABLENAME, tableName)) != 0)
        goto Outtahere;
      if((m_rc = m_tableDbf->PutField(XBDB_TABLE_DESC, desc)) != 0)
        goto Outtahere;
      if((m_rc = m_tableDbf->PutField(XBDB_TABLE_REALDELETE, realDelete ? "T" : "F")) != 0)
        goto Outtahere;
      sprintf(value, "%1.1d", version);
      if((m_rc = m_tableDbf->PutField(XBDB_TABLE_VERSION, value)) != 0)
        goto Outtahere;
    }
    
    m_rc = m_tableDbf->PutRecord();
  }
  
Outtahere:
  return m_rc;
}
    
//! Short description
/*!
  \param tableId
*/
bool 
xbDatabase::TableExists(const char *tableId)
{
  char
    tableName[XBDB_TABLENAME_LEN + 1],
    tStr[1024];
    
  if(m_tableDbf)
  {
    m_tableDbf->GetField(XBDB_TABLE_TABLENAME, tableName);
    rtrim(tableName);
    MakeDbfName(tableName, tStr);
#ifdef __WIN32__
    if(!access(tStr, 0))
#else
    if(!access(tStr, F_OK))
#endif
      return 1;
  }
  return 0;
}

//! Short description
/*!
  \param tableId
*/
void 
xbDatabase::RemoveTableFiles(const char *tableId)
{
  xbShort
    i,
    num;
    
  char
    tableName[XBDB_TABLENAME_LEN + 1],
    tStr[1024];
    
  m_tableDbf->GetField(XBDB_TABLE_TABLENAME, tableName);
  
  //
  //  Remove the table file
  //
  rtrim(tableName);
  MakeDbfName(tableName, tStr);
  unlink(tStr);
    
  //
  //  Remove the memo files
  //
  sprintf(tStr, "%s/%s.dbt", m_path, tableName);
  unlink(tStr);
  sprintf(tStr, "%s/%s.DBT", m_path, tableName);
  unlink(tStr);
    
  //
  //  Remove the index files
  //
  num = GetNumIndexes(tableId);
  for(i = 0; i < num; i++)
  {
    MakeIndexName(tableName, i, ".ndx", tStr);
    unlink(tStr);
    MakeIndexName(tableName, i, ".ntx", tStr);
    unlink(tStr);
  }
}

//
// Field stuff
//
//! Short description
/*!
  \param tableId
*/
xbShort 
xbDatabase::GetNumFields(const char *tableId)
{
  xbShort
    count = 0;
    
  char
    tStr[XBDB_TABLEID_LEN + XBDB_FIELDNUM_LEN + 1];
    
  m_rc = NOTOPENERROR;
    
  if(m_fieldDbf)
  {
    m_rc = m_fieldIndex[0]->FindKey(tableId);
    while(!m_rc || m_rc == XB_FOUND)
    {
      m_fieldDbf->GetField(XBDB_FIELD_TABLEID, tStr);
      if(!strncmp(tStr, tableId, strlen(tableId)))
        count++;
      else
        break;
      m_rc = m_fieldIndex[0]->GetNextKey();
    }
  }
  m_rc = 0;
  return count;
}
  
//! Short description
/*!
  \param tableId
  \param fieldName
*/
xbShort
xbDatabase::LookupField(const char *tableId, const char *fieldName)
{
  char
    key[XBDB_TABLEID_LEN + XBDB_FIELDNAME_LEN + 1];
    
  sprintf(key, "%-*.*s%-*.*s", XBDB_TABLEID_LEN, XBDB_TABLEID_LEN, tableId, 
          XBDB_FIELDNAME_LEN, XBDB_FIELDNAME_LEN, fieldName);
  return m_fieldIndex[1]->FindKey(key);
}

//! Short description
/*!
  \param tableId
  \param fieldNum
*/
xbShort
xbDatabase::LookupField(const char *tableId, xbShort fieldNum)
{
  char
    key[XBDB_TABLEID_LEN + XBDB_FIELDNUM_LEN + 1];
    
  sprintf(key, "%-*.*s%*d", XBDB_TABLEID_LEN, XBDB_TABLEID_LEN, tableId, 
          XBDB_FIELDNUM_LEN, fieldNum);
  return m_fieldIndex[0]->FindKey(key);
}
  
//! Short description
/*!
  \param tableId
  \param fieldName
  \param fieldNum
*/
xbShort 
xbDatabase::GetFieldNum(const char *tableId, const char *fieldName, 
                        xbShort *fieldNum)
{
  char
    tStr[10];

  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_FIELDNUM, tStr);
      *fieldNum = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param type
*/
xbShort 
xbDatabase::GetFieldType(const char *tableId, const char *fieldName, char *type)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_TYPE, type);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param desc
*/
xbShort 
xbDatabase::GetFieldDesc(const char *tableId, const char *fieldName, char *desc)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_DESC, desc);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param prompt
*/
xbShort 
xbDatabase::GetFieldPrompt(const char *tableId, const char *fieldName, char *prompt)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_PROMPT, prompt);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param inputSpec
*/
xbShort 
xbDatabase::GetFieldInputSpec(const char *tableId, const char *fieldName, char *inputSpec)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_INPUTSPEC, inputSpec);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param minValue
*/
xbShort 
xbDatabase::GetFieldMinValue(const char *tableId, const char *fieldName, char *minValue)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_MINVALUE, minValue);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param maxValue
*/
xbShort 
xbDatabase::GetFieldMaxValue(const char *tableId, const char *fieldName, char *maxValue)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_MINVALUE, maxValue);
    }
  }
  return m_rc;    
}
  
//! Short description
/*!
  \param tableId
  \param fieldName
  \param length
*/
xbShort 
xbDatabase::GetFieldLength(const char *tableId, const char *fieldName, 
                      xbShort *length)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_LENGTH, tStr);
      *length = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldName
  \param decimals
*/
xbShort 
xbDatabase::GetFieldDecimals(const char *tableId, const char *fieldName, 
                        xbShort *decimals)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_DECIMALS, tStr);
      *decimals = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param fieldName
*/
xbShort 
xbDatabase::GetFieldName(const char *tableId, xbShort fieldNum, char *fieldName)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_FIELDNAME, fieldName);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param type
*/
xbShort 
xbDatabase::GetFieldType(const char *tableId, xbShort fieldNum, char *type)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_TYPE, type);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param desc
*/
xbShort 
xbDatabase::GetFieldDesc(const char *tableId, xbShort fieldNum, char *desc)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_DESC, desc);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param prompt
*/
xbShort 
xbDatabase::GetFieldPrompt(const char *tableId, xbShort fieldNum, char *prompt)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_PROMPT, prompt);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param inputSpec
*/
xbShort 
xbDatabase::GetFieldInputSpec(const char *tableId, xbShort fieldNum, char *inputSpec)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_INPUTSPEC, inputSpec);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param minValue
*/
xbShort 
xbDatabase::GetFieldMinValue(const char *tableId, xbShort fieldNum, char *minValue)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_MINVALUE, minValue);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param maxValue
*/
xbShort 
xbDatabase::GetFieldMaxValue(const char *tableId, xbShort fieldNum, char *maxValue)
{
  m_rc = NOTOPENERROR;

  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_MAXVALUE, maxValue);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param length
*/
xbShort 
xbDatabase::GetFieldLength(const char *tableId, xbShort fieldNum, 
                      xbShort *length)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_LENGTH, tStr);
      *length = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param decimals
*/
xbShort 
xbDatabase::GetFieldDecimals(const char *tableId, xbShort fieldNum, 
                        xbShort *decimals)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_fieldDbf->GetField(XBDB_FIELD_DECIMALS, tStr);
      *decimals = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param fieldName
  \param type
  \param desc
  \param prompt
  \param inputSpec
  \param minValue
  \param maxValue
  \param length
  \param decimals
*/
xbShort 
xbDatabase::AddField(const char *tableId, xbShort fieldNum, const char *fieldName, 
                     const char type, const char *desc, const char *prompt,
                     const char *inputSpec, const char *minValue, 
                     const char *maxValue, xbShort length, xbShort decimals)
{
  char
    tStr[XBDB_FIELDNUM_LEN + 1];
    
  m_rc = NOTOPENERROR;
    
  if(m_fieldDbf)
  {
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_TABLEID, tableId)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_FIELDNUM_LEN, fieldNum);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_FIELDNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_FIELDNAME, fieldName)) != 0)
      goto Outtahere;
    sprintf(tStr, "%c", type);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_TYPE, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_DESC, desc)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_PROMPT, prompt)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_INPUTSPEC, inputSpec)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_MINVALUE, minValue)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_MAXVALUE, maxValue)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_FIELDLENGTH_LEN, length);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_LENGTH, tStr)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_FIELDDECIMALS_LEN, decimals);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_DECIMALS, tStr)) != 0)
      goto Outtahere;
      
    m_rc = m_fieldDbf->AppendRecord();
  }
  
Outtahere:
  return m_rc;
}
                    
//! Short description
/*!
  \param tableId
  \param fieldName
*/
xbShort 
xbDatabase::RemoveField(const char *tableId, const char *fieldName)
{
  xbShort
    i,
    fieldNum,
    numFields;
    
  char
    tStr[XBDB_FIELDNUM_LEN + 1];
    
  m_rc = NOTOPENERROR;
    
  if(m_fieldDbf)
  {
    numFields = GetNumFields(tableId);
    
    //
    //  Lookup and remove the field
    //
    m_rc = LookupField(tableId, fieldName);
    if(m_rc == XB_FOUND)
    {
      GetFieldNum(tableId, fieldName, &fieldNum);
      m_rc = m_fieldDbf->DeleteRecord();
    }
      
    //
    //  Renumber fields
    //
    for(i = fieldNum + 1; !m_rc && i < numFields; i++)
    {
      m_rc = LookupField(tableId, i);
      if(m_rc == XB_FOUND)
      {
        sprintf(tStr, "%*d", XBDB_FIELDNUM_LEN, i - 1);
        if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_FIELDNUM, tStr)) != 0)
          goto Outtahere;
      }
      m_rc = m_fieldDbf->PutRecord();
    }
    if(i == numFields)
      m_rc = 0;
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param fieldNum
*/
xbShort 
xbDatabase::RemoveField(const char *tableId, xbShort fieldNum)
{
  xbShort
    i,
    numFields;
    
  char
    tStr[XBDB_FIELDNUM_LEN + 1];
    
  m_rc = NOTOPENERROR;
    
  if(m_fieldDbf)
  {
    numFields = GetNumFields(tableId);
    
    //
    //  Lookup and remove the field
    //
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc == XB_FOUND)
      m_rc = m_fieldDbf->DeleteRecord();
      
    //
    //  Renumber fields
    //
    for(i = fieldNum + 1; !m_rc && i < numFields; i++)
    {
      m_rc = LookupField(tableId, i);
      if(m_rc == XB_FOUND)
      {
        sprintf(tStr, "%*d", XBDB_FIELDNUM_LEN, i - 1);
        if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_FIELDNUM, tStr)) != 0)
          goto Outtahere;
      }
      m_rc = m_fieldDbf->PutRecord();
    }
    if(i == numFields)
      m_rc = 0;
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param fieldNum
  \param fieldName
  \param type
  \param desc
  \param prompt
  \param inputSpec
  \param minValue
  \param maxValue
  \param length
  \param decimals
*/
xbShort 
xbDatabase::ChangeField(const char *tableId, xbShort fieldNum, const char *fieldName, 
                        const char type, const char *desc, const char *prompt,
                        const char *inputSpec, const char *minValue, 
                        const char *maxValue, xbShort length, xbShort decimals)
{
  m_rc = NOTOPENERROR;
    
  char
    tStr[10];
    
  if(m_fieldDbf)
  {
    m_rc = LookupField(tableId, fieldNum);
    if(m_rc != XB_FOUND)
      goto Outtahere;
    
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_TABLEID, tableId)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_FIELDNUM_LEN, fieldNum);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_FIELDNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_FIELDNAME, fieldName)) != 0)
      goto Outtahere;
    sprintf(tStr, "%c", type);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_TYPE, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_DESC, desc)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_PROMPT, prompt)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_INPUTSPEC, inputSpec)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_MINVALUE, minValue)) != 0)
      goto Outtahere;
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_MAXVALUE, maxValue)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_FIELDLENGTH_LEN, length);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_LENGTH, tStr)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_FIELDDECIMALS_LEN, decimals);
    if((m_rc = m_fieldDbf->PutField(XBDB_FIELD_DECIMALS, tStr)) != 0)
      goto Outtahere;
      
    m_rc = m_fieldDbf->PutRecord();
  }
  
Outtahere:
  return m_rc;
}
  
//
//  Index stuff
//
//! Short description
/*!
  \param tableId
*/
xbShort 
xbDatabase::GetNumIndexes(const char *tableId)
{
  xbShort
    count = 0;
    
  char
    tStr[XBDB_TABLEID_LEN + 1];
    
  m_rc = NOTOPENERROR;
    
  if(m_indexDbf)
  {
    m_rc = m_indexIndex[0]->FindKey(tableId);
    while(!m_rc || m_rc == XB_FOUND)
    {
      m_indexDbf->GetField(XBDB_INDEX_TABLEID, tStr);
      if(!strncmp(tStr, tableId, strlen(tableId)))
        count++;
      else
        break;
      m_rc = m_indexIndex[0]->GetNextKey();
    }
  }
  m_rc = 0;
  return count;
}

//! Short description
/*!
  \param tableId
  \param indexName
*/
xbShort
xbDatabase::LookupIndex(const char *tableId, const char *indexName)
{
  char
    key[XBDB_TABLEID_LEN + XBDB_INDEXNAME_LEN + 1];
    
  sprintf(key, "%-*.*s%-*.*s", XBDB_TABLEID_LEN, XBDB_TABLEID_LEN, tableId, 
          XBDB_INDEXNAME_LEN, XBDB_INDEXNAME_LEN, indexName);
  return m_indexIndex[1]->FindKey(key);
}

//! Short description
/*!
  \param tableId
  \param indexNum
*/
xbShort
xbDatabase::LookupIndex(const char *tableId, xbShort indexNum)
{
  char
    key[XBDB_TABLEID_LEN + XBDB_INDEXNUM_LEN + 1];
    
  sprintf(key, "%-*.*s%*d", XBDB_TABLEID_LEN, XBDB_TABLEID_LEN, tableId, 
          XBDB_INDEXNUM_LEN, indexNum);
  return m_indexIndex[0]->FindKey(key);
}
  
//! Short description
/*!
  \param tableId
  \param indexName
  \param num
*/
xbShort 
xbDatabase::GetIndexNum(const char *tableId, const char *indexName, xbShort *num)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_INDEXNUM, tStr);
      *num = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexName
  \param unique
*/
xbShort 
xbDatabase::GetIndexUnique(const char *tableId, const char *indexName, bool *unique)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_UNIQUE, tStr);
      *unique = tStr[0] == 'y' || tStr[0] == 'Y';
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexName
  \param expression
*/
xbShort 
xbDatabase::GetIndexExpression(const char *tableId, const char *indexName, char *expression)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_EXPRESSION, expression);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexName
  \param desc
*/
xbShort 
xbDatabase::GetIndexDesc(const char *tableId, const char *indexName, char *desc)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_DESC, desc);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexName
  \param nodeSize
*/
xbShort 
xbDatabase::GetIndexNodeSize(const char *tableId, const char *indexName, xbShort *nodeSize)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_NODESIZE, tStr);
      *nodeSize = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexName
  \param type
*/
xbShort 
xbDatabase::GetIndexType(const char *tableId, const char *indexName, char *type)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_TYPE, type);
    }
  }
  return m_rc;    
}
  
//! Short description
/*!
  \param tableId
  \param indexNum
  \param name
*/
xbShort 
xbDatabase::GetIndexName(const char *tableId, xbShort indexNum, char *name)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_INDEXNAME, name);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param unique
*/
xbShort 
xbDatabase::GetIndexUnique(const char *tableId, xbShort indexNum, bool *unique)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_UNIQUE, tStr);
      *unique = tStr[0] == 'y' || tStr[0] == 'Y';
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param expression
*/
xbShort 
xbDatabase::GetIndexExpression(const char *tableId, xbShort indexNum, char *expression)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_EXPRESSION, expression);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param desc
*/
xbShort 
xbDatabase::GetIndexDesc(const char *tableId, xbShort indexNum, char *desc)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_DESC, desc);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param nodeSize
*/
xbShort 
xbDatabase::GetIndexNodeSize(const char *tableId, xbShort indexNum, xbShort *nodeSize)
{
  m_rc = NOTOPENERROR;

  char
    tStr[10];
        
  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_NODESIZE, tStr);
      *nodeSize = atoi(tStr);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param type
*/
xbShort 
xbDatabase::GetIndexType(const char *tableId, xbShort indexNum, char *type)
{
  m_rc = NOTOPENERROR;

  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_indexDbf->GetField(XBDB_INDEX_TYPE, type);
    }
  }
  return m_rc;    
}
  
//! Short description
/*!
  \param tableId
  \param indexNum
  \param indexName
  \param unique
  \param expression
  \param desc
  \param nodeSize
  \param type
*/
xbShort 
xbDatabase::AddIndex(const char *tableId, xbShort indexNum, const char *indexName,
                     bool unique, const char *expression, const char *desc,
                     xbShort nodeSize, const char *type)
{
  char
    tStr[10];
    
  m_rc = NOTOPENERROR;
    
  if(m_indexDbf)
  {
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_TABLEID, tableId)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_INDEXNUM_LEN, indexNum);
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_INDEXNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_INDEXNAME, indexName)) != 0)
      goto Outtahere;
    strcpy(tStr, unique ? "Y" : "N");
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_UNIQUE, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_EXPRESSION, expression)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_DESC, desc)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_INDEXNODESIZE_LEN, nodeSize);
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_NODESIZE, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_TYPE, type)) != 0)
      goto Outtahere;
      
    m_rc = m_indexDbf->AppendRecord();
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param indexNum
*/
xbShort 
xbDatabase::RemoveIndex(const char *tableId, xbShort indexNum)
{
  xbShort
    i,
    numIndexes;
    
  char
    tStr[XBDB_INDEXNUM_LEN + 1],
    tableName[XBDB_TABLENAME_LEN + 1],
    indexType[XBDB_INDEXTYPE_LEN + 1],
    oldName[1024],
    newName[1024];
    
  m_rc = NOTOPENERROR;
    
  if(m_indexDbf)
  {
    numIndexes = GetNumIndexes(tableId);
    GetTableFileName(tableId, tableName);
    rtrim(tableName);
    
    //
    //  Lookup and remove the index record.
    //
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc == XB_FOUND)
    {
      GetIndexType(tableId, indexNum, indexType);
      m_rc = m_indexDbf->DeleteRecord();
    }
    
    //
    //  Remove the actual index file
    //
    if(!strcmp(indexType, XBDB_INDEXTYPE_NDX))
      MakeIndexName(tableName, indexNum, ".ndx", oldName);
    else
      MakeIndexName(tableName, indexNum, ".ntx", oldName);
    unlink(oldName);
    
    //
    //  Renumber and rename indexes after the one deleted
    //
    for(i = indexNum + 1; (!m_rc || m_rc == XB_FOUND) && i < numIndexes; i++)
    {
      GetIndexType(tableId, i, indexType);
      
      m_rc = LookupIndex(tableId, i);
      if(m_rc == XB_FOUND)
      {
        sprintf(tStr, "%*d", XBDB_INDEXNUM_LEN, i - 1);
        if((m_rc = m_indexDbf->PutField(XBDB_INDEX_INDEXNUM, tStr)) != 0)
          goto Outtahere;
        m_rc = m_indexDbf->PutRecord();
      
        if(!strcmp(indexType, XBDB_INDEXTYPE_NDX))
        {
          MakeIndexName(tableName, i, ".ndx", oldName);
          MakeIndexName(tableName, i - 1, ".ndx", newName);
        }
        else
        {
          MakeIndexName(tableName, i, ".ntx", oldName);
          MakeIndexName(tableName, i - 1, ".ntx", newName);
        }
        rename(oldName, newName);
      }
    }
    if(i != numIndexes)
      m_rc = 0;
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param indexName
*/
xbShort 
xbDatabase::RemoveIndex(const char *tableId, const char *indexName)
{
  xbShort
    i,
    numIndexes,
    indexNum;
    
  char
    tStr[XBDB_INDEXNUM_LEN + 1],
    tableName[XBDB_TABLENAME_LEN + 1],
    indexType[XBDB_INDEXTYPE_LEN + 1],
    oldName[1024],
    newName[1024];
    
  m_rc = NOTOPENERROR;
    
  if(m_indexDbf)
  {
    numIndexes = GetNumIndexes(tableId);
    GetTableFileName(tableId, tableName);
    rtrim(tableName);
    
    //
    //  Lookup and remove the index record.
    //
    m_rc = LookupIndex(tableId, indexName);
    if(m_rc == XB_FOUND)
    {
      GetIndexNum(tableId, indexName, &indexNum);
      GetIndexType(tableId, indexNum, indexType);
      m_rc = m_indexDbf->DeleteRecord();
    }
    
    //
    //  Remove the actual index file
    //
    if(!strcmp(indexType, XBDB_INDEXTYPE_NDX))
      MakeIndexName(tableName, indexNum, ".ndx", oldName);
    else
      MakeIndexName(tableName, indexNum, ".ntx", oldName);
    unlink(oldName);
    
    //
    //  Renumber and rename indexes after the one deleted
    //
    for(i = indexNum + 1; (!m_rc || m_rc == XB_FOUND) && i < numIndexes; i++)
    {
      GetIndexType(tableId, i, indexType);
      
      m_rc = LookupIndex(tableId, i);
      if(m_rc == XB_FOUND)
      {
        sprintf(tStr, "%*d", XBDB_INDEXNUM_LEN, i - 1);
        if((m_rc = m_indexDbf->PutField(XBDB_INDEX_INDEXNUM, tStr)) != 0)
          goto Outtahere;
        m_rc = m_indexDbf->PutRecord();
      
        if(!strcmp(indexType, XBDB_INDEXTYPE_NDX))
        {
          MakeIndexName(tableName, i, ".ndx", oldName);
          MakeIndexName(tableName, i - 1, ".ndx", newName);
        }
        else
        {
          MakeIndexName(tableName, i, ".ntx", oldName);
          MakeIndexName(tableName, i - 1, ".ntx", newName);
        }
        rename(oldName, newName);
      }
    }
    if(i != numIndexes)
      m_rc = 0;
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param indexName
  \param unique
  \param expression
  \param desc
  \param nodeSize
  \param type
*/
xbShort 
xbDatabase::ChangeIndex(const char *tableId, xbShort indexNum, const char *indexName,
                        bool unique, const char *expression, const char *desc,
                        xbShort nodeSize, const char *type)
{
  char
    tStr[100];
    
  m_rc = NOTOPENERROR;
    
  if(m_indexDbf)
  {
    m_rc = LookupIndex(tableId, indexNum);
    if(m_rc != XB_FOUND)
      goto Outtahere;
    
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_TABLEID, tableId)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_INDEXNUM_LEN, indexNum);
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_INDEXNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_INDEXNAME, indexName)) != 0)
      goto Outtahere;
    strcpy(tStr, unique ? "Y" : "N");
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_UNIQUE, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_EXPRESSION, expression)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_DESC, desc)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_INDEXNODESIZE_LEN, nodeSize);
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_NODESIZE, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_indexDbf->PutField(XBDB_INDEX_TYPE, type)) != 0)
      goto Outtahere;
      
    m_rc = m_indexDbf->PutRecord();
  }
  
Outtahere:
  return m_rc;
}
                      
//! Short description
/*!
  \param tableId
  \param indexNum
*/
bool 
xbDatabase::IndexExists(const char *tableId, xbShort indexNum)
{
  char
    tableName[XBDB_TABLENAME_LEN + 1],
    tStr[1024];
    
  if((m_rc = GetTableFileName(tableId, tableName)) != 0)
    goto Outtahere;
  if((m_rc = GetIndexType(tableId, indexNum, tStr)) != 0)
    goto Outtahere;
  if(!strcmp(tStr, XBDB_INDEXTYPE_NDX))
  {
    MakeIndexName(tableName, indexNum, ".ndx", tStr);
#ifdef __WIN32__
    if(!access(tStr, 0))
#else
    if(!access(tStr, F_OK))
#endif
      return 1;
  }
  else
  {
    MakeIndexName(tableName, indexNum, ".ntx", tStr);
#ifdef __WIN32__
    if(!access(tStr, 0))
#else
    if(!access(tStr, F_OK))
#endif
      return 1;
  }
Outtahere:
  return 0;
}

//! Short description
/*!
  \param tableId
  \param indexNum
*/
void 
xbDatabase::RemoveIndexFile(const char *tableId, xbShort indexNum)
{
  char
    tableName[XBDB_TABLENAME_LEN + 1],
    tStr[1024];
    
  GetTableFileName(tableId, tableName);
  rtrim(tableName);
  
  //
  //  Remove the index file
  //
  MakeIndexName(tableName, indexNum, ".ndx", tStr);
  unlink(tStr);
  MakeIndexName(tableName, indexNum, ".ntx", tStr);
  unlink(tStr);
}

//
//  Report stuff
//
//! Short description
/*!
*/
xbShort 
xbDatabase::GetNumReports(void)
{
  m_rc = NOTOPENERROR;
    
  if(m_reportDbf)
  {
    m_rc = 0;
    return m_reportDbf->NoOfRecords();
  }
  
  return -1;
}

//! Short description
/*!
  \param reportNum
*/
xbShort 
xbDatabase::LookupReport(xbShort reportNum)
{
  char
    key[XBDB_REPORTNUM_LEN + 1];
    
  sprintf(key, "%*d", XBDB_REPORTNUM_LEN, reportNum);
  return m_reportIndex[0]->FindKey(key);
}

//! Short description
/*!
  \param reportNum
  \param tableId
*/
xbShort 
xbDatabase::GetReportTableId(xbShort reportNum, char *tableId)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_TABLEID, tableId);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param reportNum
  \param title
*/
xbShort 
xbDatabase::GetReportTitle(xbShort reportNum, char *title)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_TITLE, title);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param reportNum
  \param indexNum
*/
xbShort 
xbDatabase::GetReportIndexNum(xbShort reportNum, xbShort *indexNum)
{
  char
    tStr[XBDB_INDEXNUM_LEN + 1];
    
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_INDEXNUM, tStr);
      *indexNum = (xbShort)atoi(tStr);
    }
  }
  return m_rc;
}

//! Short description
/*!
  \param reportNum
  \param minKey
*/
xbShort
xbDatabase::GetReportMinKey(xbShort reportNum, char *minKey)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_MINKEY, minKey);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param reportNum
  \param maxKey
*/
xbShort 
xbDatabase::GetReportMaxKey(xbShort reportNum, char *maxKey)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_MAXKEY, maxKey);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param reportNum
  \param query
*/
xbShort 
xbDatabase::GetReportQuery(xbShort reportNum, char *query)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_QUERY, query);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param reportNum
  \param fieldList
*/
xbShort 
xbDatabase::GetReportFieldList(xbShort reportNum, char *fieldList)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_FIELDLIST, fieldList);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param reportNum
  \param format
*/
xbShort 
xbDatabase::GetReportFormat(xbShort reportNum, char *format)
{
  m_rc = NOTOPENERROR;

  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = 0;
      m_reportDbf->GetField(XBDB_REPORT_FORMAT, format);
    }
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
  \param reportNum
  \param title
  \param indexNum
  \param minKey
  \param maxKey
  \param query
  \param fieldList
  \param format
*/
xbShort 
xbDatabase::AddReport(const char *tableId, xbShort reportNum, const char *title,
                      xbShort indexNum, const char *minKey, const char *maxKey,
                      const char *query, const char *fieldList,
                      const char *format)
{
  char
    tStr[10];
    
  m_rc = NOTOPENERROR;
    
  if(m_reportDbf)
  {
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_TABLEID, tableId)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_REPORTNUM_LEN, reportNum);
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_REPORTNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_TITLE, title)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_INDEXNUM_LEN, indexNum);
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_INDEXNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_MINKEY, minKey)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_MAXKEY, maxKey)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_QUERY, query)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_FIELDLIST, fieldList)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_FORMAT, format)) != 0)
      goto Outtahere;
      
    m_rc = m_reportDbf->AppendRecord();
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param reportNum
*/
xbShort 
xbDatabase::RemoveReport(xbShort reportNum)
{
  xbShort
    i,
    numReports;
    
  char
    tStr[XBDB_REPORTNUM_LEN + 1];

  m_rc = NOTOPENERROR;
    
  if(m_reportDbf)
  {
    numReports = GetNumReports();
    
    //
    //  Lookup and remove the report record.
    //
    m_rc = LookupReport(reportNum);
    if(m_rc == XB_FOUND)
    {
      m_rc = m_reportDbf->DeleteRecord();
    }
    
    //
    //  Renumber reports after the one deleted
    //
    for(i = reportNum + 1; (!m_rc || m_rc == XB_FOUND) && i < numReports; i++)
    {
      m_rc = LookupReport(i);
      if(m_rc == XB_FOUND)
      {
        sprintf(tStr, "%*d", XBDB_REPORTNUM_LEN, i - 1);
        if((m_rc = m_reportDbf->PutField(XBDB_REPORT_REPORTNUM, tStr)) != 0)
          goto Outtahere;
        m_rc = m_reportDbf->PutRecord();
      }
    }
    if(i == numReports)
      m_rc = 0;
  }
  
Outtahere:
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param reportNum
  \param title
  \param indexNum
  \param minKey
  \param maxKey
  \param query
  \param fieldList
  \param format
*/
xbShort 
xbDatabase::ChangeReport(const char *tableId, xbShort reportNum, const char *title,
                         xbShort indexNum, const char *minKey, const char *maxKey,
                         const char *query, const char *fieldList,
                         const char *format)
{
  char
    tStr[10];
    
  m_rc = NOTOPENERROR;
    
  if(m_reportDbf)
  {
    m_rc = LookupReport(reportNum);
    if(m_rc != XB_FOUND)
      goto Outtahere;
    
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_TABLEID, tableId)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_TITLE, title)) != 0)
      goto Outtahere;
    sprintf(tStr, "%*d", XBDB_INDEXNUM_LEN, indexNum);
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_INDEXNUM, tStr)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_MINKEY, minKey)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_MAXKEY, maxKey)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_QUERY, query)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_FIELDLIST, fieldList)) != 0)
      goto Outtahere;
    if((m_rc = m_reportDbf->PutField(XBDB_REPORT_FORMAT, format)) != 0)
      goto Outtahere;
      
    m_rc = m_reportDbf->PutRecord();
  }
  
Outtahere:
  return m_rc;
}                         
                        
//
//  Dbf stuff
//
//! Short description
/*!
  \param tableId
*/
xbDbf *
xbDatabase::OpenDbf(const char *tableId)
{
  char
    tableName[XBDB_TABLENAME_LEN + 1],
    expression[XBDB_EXPRESSION_LEN + 1],
    tStr[1024];
    
  xbShort
    i,
    numIndexes;
    
  xbDbf
    *dbf = 0;

  xbNdx
    *ndx;
    
#ifdef XB_INDEX_NTX
  xbNtx 
    *ntx;
#endif // XB_INDEX_NTX
    
  bool
    unique;
    
  m_rc = NOTOPENERROR;
  
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      dbf = new xbDbf(m_xbase);
      if(!dbf)
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
      m_tableDbf->GetField(XBDB_TABLE_TABLENAME, tableName);
      rtrim(tableName);
      MakeDbfName(tableName, tStr);
      if((m_rc = dbf->OpenDatabase(tStr)) != 0)
        goto Outtahere;
        
      m_tableDbf->GetField(XBDB_TABLE_REALDELETE, tStr);
      if(tStr[0] == 'T')
        dbf->RealDeleteOn();
      else
        dbf->RealDeleteOff();
                
      numIndexes = GetNumIndexes(tableId);
      for(i = 0; i < numIndexes; i++)
      {
        if((m_rc = GetIndexType(tableId, i, tStr)) != 0)
          goto Outtahere;
        if((m_rc = GetIndexExpression(tableId, i, expression)) != 0)
          goto Outtahere;
        if((m_rc = GetIndexUnique(tableId, i, &unique)) != 0)
          goto Outtahere;
        if(!strcmp(tStr, XBDB_INDEXTYPE_NDX))
        {
          ndx = new xbNdx(dbf);
          if(!ndx)
          {
            m_rc = XB_NO_MEMORY;
            goto Outtahere;
          }
          MakeIndexName(tableName, i, ".ndx", tStr);
          if((m_rc = ndx->OpenIndex(tStr)) != 0)
          {
            delete ndx;
            ndx = new xbNdx(dbf);
            if(!ndx)
            {
              m_rc = XB_NO_MEMORY;
              goto Outtahere;
            }
            m_rc = ndx->CreateIndex(tStr, expression, 
                                  unique ? XB_UNIQUE : XB_NOT_UNIQUE, XB_OVERLAY);
          }
        }
        else
        {
#ifdef XB_INDEX_NTX
          ntx = new xbNtx(dbf);
          if(!ntx)
          {
            m_rc = XB_NO_MEMORY;
            goto Outtahere;
          }
          MakeIndexName(tableName, i, ".ntx", tStr);
          if((m_rc = ntx->OpenIndex(tStr)) != 0)
          {
            delete ntx;
            ntx = new xbNtx(dbf);
            if(!ntx)
            {
              m_rc = XB_NO_MEMORY;
              goto Outtahere;
            }
            m_rc = ntx->CreateIndex(tStr, expression, 
                                  unique ? XB_UNIQUE : XB_NOT_UNIQUE, XB_OVERLAY);
          }
#else
          m_rc = XB_OPEN_ERROR;
#endif // XB_INDEX_NTX
        }
        if(m_rc)
          goto Outtahere;
      }
    }
  }
  
Outtahere:
  if(m_rc && dbf)
  {
    dbf->CloseDatabase(1);
    delete dbf;
    dbf = 0;
  }    
  
  return dbf;
}

//! Short description
/*!
  \param tableId
  \param overlay
*/
xbDbf *
xbDatabase::CreateDbf(const char *tableId, xbShort overlay)
{
  char
    tableName[XBDB_TABLENAME_LEN + 1],
    fieldName[XBDB_FIELDNAME_LEN + 1],
    fieldType[XBDB_FIELDTYPE_LEN + 1],
    expression[XBDB_EXPRESSION_LEN + 1],
    indexType[XBDB_INDEXTYPE_LEN + 1],
    tStr[1024];
    
  xbDbf
    *dbf = 0;
    
  xbNdx
    *ndx;
    
#ifdef XB_INDEX_NTX
  xbNtx 
    *ntx;
#endif // XB_INDEX_NTX
    
  xbShort
    i,
    numFields,
    numIndexes,
    fieldLength,
    fieldDecimals;
    
  bool
    unique;
    
  xbSchema
    *schema = 0;
    
  m_rc = NOTOPENERROR;
  
  if(m_tableDbf)
  {
    m_rc = m_tableIndex[0]->FindKey(tableId);
    if(m_rc == XB_FOUND)
    {
      //
      // Create the schema
      //
      if((numFields = GetNumFields(tableId)) == 0)
      {
        m_rc = XB_INVALID_FIELD;
        goto Outtahere;
      }
      
      schema = (xbSchema *)calloc(1, sizeof(xbSchema) * (numFields + 1));
      if(!schema)
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
      
      for(i = 0; i < numFields; i++)
      {
        if((m_rc = GetFieldName(tableId, i, fieldName)) != 0)
          goto Outtahere;
        if((m_rc = GetFieldType(tableId, i, fieldType)) != 0)
          goto Outtahere;
        if((m_rc = GetFieldLength(tableId, i, &fieldLength)) != 0)
          goto Outtahere;
        if((m_rc = GetFieldDecimals(tableId, i, &fieldDecimals)) != 0)
          goto Outtahere;
        strcpy(schema[i].FieldName, rtrim(fieldName));
        schema[i].Type = fieldType[0];
        schema[i].FieldLen = fieldLength;
        schema[i].NoOfDecs = fieldDecimals;
      }
      
      //
      //  Create the database
      //
      dbf = new xbDbf(m_xbase);
      if(!dbf)
      {
        m_rc = XB_NO_MEMORY;
        goto Outtahere;
      }
      m_tableDbf->GetField(XBDB_TABLE_TABLENAME, tableName);
      rtrim(tableName);
      MakeDbfName(tableName, tStr);
      if((m_rc = dbf->CreateDatabase(tStr, schema, overlay)) != 0)
        goto Outtahere;
        
      m_tableDbf->GetField(XBDB_TABLE_REALDELETE, tStr);
      if(tStr[0] == 'T')
        dbf->RealDeleteOn();
      else
        dbf->RealDeleteOff();
        
      //
      //  Create indexes
      //
      numIndexes = GetNumIndexes(tableId);
      for(i = 0; i < numIndexes; i++)
      {
        if((m_rc = GetIndexType(tableId, i, indexType)) != 0)
          goto Outtahere;
        if((m_rc = GetIndexExpression(tableId, i, expression)) != 0)
          goto Outtahere;
        if((m_rc = GetIndexUnique(tableId, i, &unique)) != 0)
          goto Outtahere;
          
        if(!strcmp(indexType, XBDB_INDEXTYPE_NDX))
        {
          ndx = new xbNdx(dbf);
          if(!ndx)
          {
            m_rc = XB_NO_MEMORY;
            goto Outtahere;
          }
          rtrim(expression);
          MakeIndexName(tableName, i, ".ndx", tStr);
          m_rc = ndx->CreateIndex(tStr, expression, 
                                unique ? XB_UNIQUE : XB_NOT_UNIQUE, overlay);
        }
        else
        {
#ifdef XB_INDEX_NTX
          ntx = new xbNtx(dbf);
          if(!ntx)
          {
            m_rc = XB_NO_MEMORY;
            goto Outtahere;
          }
          rtrim(expression);
          MakeIndexName(tableName, i, ".ntx", tStr);
          m_rc = ntx->CreateIndex(tStr, expression, 
                                unique ? XB_UNIQUE : XB_NOT_UNIQUE, overlay);
        if(m_rc)
          goto Outtahere;
#else
        m_rc = XB_OPEN_ERROR;
#endif // XB_INDEX_NTX
        }
      }
    }
  }
  
Outtahere:
  if(m_rc && dbf)
  {
    dbf->CloseDatabase(1);
    delete dbf;
    dbf = 0;
  }    
  if(schema)
    free(schema);
  
  return dbf;
}
  
//
// Misc
//
//! Short description
/*!
  \param tableId
  \param packStatusFunc
  \param indexStatusFunc
*/
xbShort 
xbDatabase::Pack(const char *tableId,
                 void (*packStatusFunc)(xbLong itemNum, xbLong numItems),
                 void (*indexStatusFunc)(xbLong itemNum, xbLong numItems))
{
  xbDbf
    *dbf = 0;
    
  dbf = OpenDbf(tableId);
  if(dbf)
  {
    m_rc = dbf->PackDatabase(F_SETLKW, packStatusFunc, indexStatusFunc);
    dbf->CloseDatabase(1);
    delete dbf;
  }
  else
    m_rc = LastError();
    
  return m_rc;
}

//! Short description
/*!
  \param tableId
  \param indexNum
  \param statusFunc
*/
xbShort 
xbDatabase::ReIndex(const char *tableId, xbShort indexNum,
                    void (*statusFunc)(xbLong itemNum, xbLong numItems))
{
  xbShort
    i,
    startIndexNum,
    endIndexNum;
    
  xbDbf
    *dbf = 0;
    
  xbIndex
    *index = 0;
    
  if(indexNum == -1)
  {
    startIndexNum = 0;
    endIndexNum = GetNumIndexes(tableId);
  }
  else
  {
    startIndexNum = indexNum;
    endIndexNum = indexNum + 1;
  }
    
  dbf = OpenDbf(tableId);
  if(dbf)
  {
    m_rc = 0;
    for(i = startIndexNum; !m_rc && i < endIndexNum; i++)
    {
      index = dbf->GetIndex(i);
      if(index)
        m_rc = index->ReIndex(statusFunc);
    }
    dbf->CloseDatabase(1);
    delete dbf;
  }
  return m_rc;    
}

//! Short description
/*!
  \param tableId
*/
xbShort 
xbDatabase::Zap(const char *tableId)
{
  xbDbf
    *dbf = 0;

  RemoveTableFiles(tableId);    
  if(m_rc)
    return m_rc;
    
  dbf = CreateDbf(tableId, 1);
  if(dbf)
  {
    dbf->CloseDatabase(1);
    delete dbf;
    m_rc = 0;
  }
  else
    m_rc = LastError();
    
  return m_rc;
}

//! Short description
/*!
  \param s
*/
char *
xbDatabase::ltrim(char *s)
{
  char
    *p,
    *q;
    
  for(p = s; *p && isspace(*p); p++)
    ;
  for(q = s; *p; p++, q++)
    *q = *p;
  *q = 0;
    
  return s;
}

//! Short description
/*!
  \param s
*/
char *
xbDatabase::rtrim(char *s)
{
  char
    *p;

  for(p = s + (strlen(s)); p >= s && (!*p || isspace(*p)); p--)
    *p = 0;

  return s;
}

//! Short description
/*!
  \param tableName, buf
*/
char *
xbDatabase::MakeDbfName(const char *tableName, char *buf)
{
  sprintf(buf, "%s/%s", m_path, tableName);
  return buf;
}

//! Short description
/*!
  \param tableName
  \param indexNum
  \param suffix
  \param buf
*/
char *
xbDatabase::MakeIndexName(const char *tableName, xbShort indexNum, 
                          const char *suffix, char *buf)
{
  sprintf(buf, "%s/%s%d%s", m_path, tableName, indexNum, suffix);
  return buf;
}

#endif // XB_INDEX_ANY
