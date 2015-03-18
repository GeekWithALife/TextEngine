setlocal
cmake -G "MinGW Makefiles" .
mingw32-make clean
mingw32-make
endlocal
pause