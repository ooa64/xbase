#include <xdb/xbase.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef XB_INDEX_ANY
xbIndex::xbIndex(xbDbf * pdbf)
{
  index          = this;
  dbf            = pdbf;
  ExpressionTree = NULL;
  indexfp        = NULL;
  IndexStatus    = 0;
  CurDbfRec      = 0L;
  KeyBuf         = NULL;
  KeyBuf2        = NULL;
  CurLockCount   = 0;
  CurLockType    = -1;
}
#endif // XB_INDEX_ANY

