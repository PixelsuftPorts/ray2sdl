@echo off
cls
gcc %* -Lraylib\lib -Iraylib\include -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -o test.exe && test.exe
