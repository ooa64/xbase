
rem This batch file builds the xbase utility programs
rem using the borland 5.5 compiler

del *.tds
del *.exe
del *.obj

bcc32 -I.. -L../xbase checkndx.cpp xbase.lib
bcc32 -I.. -L../xbase copydbf.cpp  xbase.lib
bcc32 -I.. -L../xbase dbfutil1.cpp xbase.lib
bcc32 -I.. -L../xbase dbfxtrct.cpp xbase.lib
bcc32 -I.. -L../xbase deletall.cpp xbase.lib
bcc32 -I.. -L../xbase dumpdbt.cpp  xbase.lib
bcc32 -I.. -L../xbase dumphdr.cpp  xbase.lib
bcc32 -I.. -L../xbase dumprecs.cpp xbase.lib
bcc32 -I.. -L../xbase packdbf.cpp  xbase.lib
bcc32 -I.. -L../xbase reindex.cpp  xbase.lib
bcc32 -I.. -L../xbase undelall.cpp xbase.lib
bcc32 -I.. -L../xbase zap.cpp      xbase.lib

