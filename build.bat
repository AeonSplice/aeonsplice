@echo off
SETLOCAL EnableDelayedExpansion
set myvar=C:\MinGW\bin\c++.exe -o "%cd%\aeonsplice.exe" -static -fpermissive
for /f "delims=" %%P in ('dir /b *.cpp') do (
  SET "CppName=%%~nxP"
  REM echo "!CppName:~0,2!" <- echos first two characters of each file name
  echo "!CppName!"
  SET tempStr=GEN !CppName! 
  SET myvar=!myvar! "%%P"
)
set cmd=!myvar!
%cmd%
pause
exit