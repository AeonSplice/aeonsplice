@echo off
SETLOCAL EnableDelayedExpansion
set myvar=C:\MinGW\bin\c++.exe -o "%cd%\aeonsplice.exe" -static -fpermissive
for /f "delims=" %%P in ('dir /b *.cpp') do (
  SET "sPDFName=%%~nxP"
  REM echo "!sPDFName:~0,2!" <- echos first two characters of each file name
  SET tempStr=GEN !sPDFName! 
  SET myvar=!myvar! "%%P"
)
set cmd=!myvar!
%cmd%
pause
exit