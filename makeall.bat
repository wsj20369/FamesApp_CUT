@echo off

REM  ±àÒëslc
CD   src
CALL MAKEFILE
CD   ..

REM ±àÒë×¢²á»ú
CD MAKE_SN
CALL MAKEFILE >> ..\t.txt
cd ..

REM  Á¬½Ó
make makefile


ECHO ALL OK. 
