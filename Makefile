###################################################
#........../\./\...___......|\.|..../...\.........#
#........./..|..\/\.|.|_|._.|.\|....|.c.|.........#
#......../....../--\|.|.|.|i|..|....\.../.........#
#        Mathtin (c)                              #
###################################################
#   Author: Daniel [Mathtin] Shiko                #
#   Copyright (c) 2017 <wdaniil@mail.ru>          #
#   This file is released under the MPL license.  #
###################################################
# Project: memmod

CPP_32      = i686-w64-mingw32-g++.exe -g -D_DEBUG
CC_32       = i686-w64-mingw32-gcc.exe -g -D_DEBUG
OBJ_32      = obj/memmod-32.o obj/MemoryModule-32.o
LINKOBJ_32_37 = $(OBJ_32) -lpython3.7m.dll
LINKOBJ_32_27 = $(OBJ_32) -lpython2.7.dll

CPP_64      = x86_64-w64-mingw32-g++.exe -g -D_DEBUG
CC_64       = x86_64-w64-mingw32-gcc.exe -g -D_DEBUG
OBJ_64      = obj/memmod-64.o obj/MemoryModule-64.o
LINKOBJ_64_37 = $(OBJ_64) -lpython3.7m.dll
LINKOBJ_64_27 = $(OBJ_64) -lpython2.7.dll

LIBS_32     = -L"/mingw32/lib"
INCS_32     = -I"/mingw32/include/python3.7m"

LIBS_64     = -L"/mingw64/lib"
INCS_64     = -I"/mingw64/include/python3.7m"

CFLAGS_32   = $(INCS_32)    -std=c11   -Wall -pedantic #-Ofast
CFLAGS_64   = $(INCS_64)    -std=c11   -Wall -pedantic #-Ofast

CXXINCS  = 
CXXFLAGS = $(CXXINCS) -std=c++17 -Wall -pedantic #-Ofast
RM       = rm -f

BIN       = _memmod-cpython
BIN_37 = $(BIN)-37m.dll
BIN_27 = $(BIN)-27.dll

.PHONY: all all-before all-after clean clean-custom lib32-37 lib32-27 lib64-37 lib64-27

all: all-before lib64-37 all-after

clean: clean-custom
	$(RM) $(OBJ_32) $(OBJ_64) $(BIN_37) $(BIN_27) 

obj/memmod-64.o: src/memmod.c src/memmod.h
	$(CC_64) -c $< -o $@ $(CFLAGS_64)

obj/MemoryModule-64.o: src/MemoryModule.c src/MemoryModule.h
	$(CC_64) -c $< -o $@ $(CFLAGS_64)

obj/memmod-32.o: src/memmod.c src/memmod.h
	$(CC_32) -c $< -o $@ $(CFLAGS_32)

obj/MemoryModule-32.o: src/MemoryModule.c src/MemoryModule.h
	$(CC_32) -c $< -o $@ $(CFLAGS_32)

lib64-37: $(OBJ_64)
	$(CC_64) -shared $(LINKOBJ_64_37) $(LIBS_64) -o $(BIN_37) -Wl,--add-stdcall-alias

lib64-27: $(OBJ_64)
	$(CC_64) -shared $(LINKOBJ_64_27) $(LIBS_64) -o $(BIN_27) -Wl,--add-stdcall-alias

lib32-37: $(OBJ_32)
	$(CC_32) -shared $(LINKOBJ_32_37) $(LIBS_32) -o $(BIN_37) -Wl,--add-stdcall-alias

lib32-27: $(OBJ_32)
	$(CC_32) -shared $(LINKOBJ_32_27) $(LIBS_32) -o $(BIN_27) -Wl,--add-stdcall-alias

