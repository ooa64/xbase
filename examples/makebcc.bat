
rem This batch file builds the xbase sample programs
rem using the borland 5.5 compiler

del *.tds
del *.exe
del *.obj

bcc32 -I.. -L../xbase sample1.cpp xbase.lib
bcc32 -I.. -L../xbase sample2.cpp xbase.lib
bcc32 -I.. -L../xbase sample3.cpp xbase.lib
bcc32 -I.. -L../xbase sample4.cpp xbase.lib
bcc32 -I.. -L../xbase sample5.cpp xbase.lib
bcc32 -I.. -L../xbase exfilter.cpp xbase.lib
bcc32 -I.. -L../xbase string.cpp xbase.lib

