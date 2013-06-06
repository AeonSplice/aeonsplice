@echo off
set cmd=C:\MinGW\bin\c++.exe -x c++ -o "%cd%\aeonsplice.exe" -static "%cd%\main.cpp" "aeonwindow.cpp" -Wl,glew32.dll,glfw.dll
%cmd%
pause
exit