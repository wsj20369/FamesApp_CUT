@echo off

REM  ����slc
CD   src
CALL MAKEFILE > ..\t.txt
CD   ..

REM ����ע���
CD MAKE_SN
CALL MAKEFILE >> ..\t.txt
cd ..

REM  ����
make makefile

ECHO.
ECHO ALL OK. 

notepad t.txt

