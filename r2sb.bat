@echo off
cls
gcc %* src\*.c -Isrc\include -ISDL2\include -LSDL2\lib -lSDL2 -lSDL2main -Wall -o test.exe && test.exe
