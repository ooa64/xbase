# XBASE Version 2.1.2

XBase is an xbase (i.e. dBase, FoxPro, etc.) compatible C++ class library 
originally by Gary Kunkel and others (see the AUTHORS file).

XBASE is useful for accessing data in legacy dBase 3 and 4 database files as
well as a general light-weight database engine.  It includes support for
DBF (dBase version 3 and 4) data files, NDX and NTX indexes, and DBT 
(dBase version 3 and 4).  It supports file and record locking under *nix
OS's (we hope to fix the locking support under Win32 soon).


Project home page https://sourceforge.net/projects/xdb/

News:
    
    - 2020-11-05 Windows build system for MSVC with nmake.
                 32-bit library, examples, tools and tests - everything works.
                 
    - 2020-11-04 Sources imported using https://gitlab.com/esr/cvs-fast-export.
                 autoconf/automake files updated, ./configure&&make should work now
