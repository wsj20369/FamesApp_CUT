echo. > ltall.txt

lint  options.lnt -Isrc -I..\lib\include src\*.c    >> ltall.txt

notepad ltall.txt

