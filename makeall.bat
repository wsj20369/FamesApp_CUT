@echo off

REM  ����slc
CD   src
CALL MAKEFILE
CD   ..

REM ����ע���
CD MAKE_SN
CALL MAKEFILE >> ..\t.txt
cd ..

REM  ����
make makefile


ECHO ALL OK. 
