@echo off

rem call C:\Program Files\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat

echo SDK:
echo %WindowsSdkDir%
if not exist "%WindowsSdkDir%\lib\kernel32.lib" goto sdkerror

rem fix tailing slash
if "%WindowsSdkDir:~-1%" == "\" set WindowsSdkDir=%WindowsSdkDir:~0,-1%

echo Compiler:
cl > nul
if errorlevel 1 goto clerror

rem defaults: DEBUG=0 STATIC=1 MSVCRT=1
if not "%DEBUG%" == ""  set OPTS=%OPTS% DEBUG=%DEBUG%
if not "%MSVCRT%" == "" set OPTS=%OPTS% MSVCRT=%MSVCRT%
if not "%STATIC%" == "" set OPTS=%OPTS% STATIC_BUILD=%STATIC%

rem supress warning C4996 on fopen, strcpy, etc.
set CFLAGS=%CFLAGS% /D _CRT_SECURE_NO_WARNINGS
rem supress warning C4530
set CFLAGS=%CFLAGS% /D _HAS_EXCEPTIONS=0
:set CFLAGS=%CFLAGS% /EHsc

nmake -nologo -f makefile.vc %OPTS% CFLAGS="%CFLAGS%" SDKINSTALLDIR="%WindowsSdkDir%" %*

if not errorlevel 1 goto ok
echo ERROR: MAKE RRROR
goto end

:sdkerror
echo ERROR: SDK NOT FOUND
goto end

:clerror
echo ERROR: COMPILER NOT FOUND
goto end

:ok
echo OK

:end
