COMPILER    = tcc -c -1 -B -S -ml -w -Ic:\tc\include
LINKER      = tlink /3
STD_OBJ     = c:\tc\lib\c0l.obj
STD_LIB     = c:\tc\lib\cxc.lib+..\lib\os.lib
ALL_LIB     = src\cut.lib
ALL_LIST    = src\cut.lib


EXE_FILE    = cut\cut.exe

$(EXE_FILE): $(ALL_LIST)
	$(LINKER) $(STD_OBJ),$(EXE_FILE),,$(STD_LIB)+$(ALL_LIB);


MAKE_SN     = make_sn\make_sn.exe

$(MAKE_SN): make_sn\make_sn.obj
	$(LINKER) $(STD_OBJ)+make_sn\make_sn.obj,$(MAKE_SN),,$(STD_LIB);

