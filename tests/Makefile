# Project: StrQueue DLL Library
###################################################
#........../\./\...___......|\.|..../...\.........#
#........./..|..\/\.|.|_|._.|.\|....|.c.|.........#
#......../....../--\|.|.|.|i|..|....\.../.........#
#        Mathtin (c)                              #
###################################################
#   Author: Daniel [Mathtin] Shiko                #
#   Copyright (c) 2019 <wdaniil@mail.ru>          #
#   This file is released under the MPL license.  #
###################################################

OBJ_DIR     = ../obj

CC_32       = i686-w64-mingw32-gcc.exe
OBJ_32      = $(OBJ_DIR)/StrQueue-32.o
LINKOBJ_32  = $(OBJ_32)

CC_64       = x86_64-w64-mingw32-gcc.exe
OBJ_64      = $(OBJ_DIR)/StrQueue-64.o
LINKOBJ_64  = $(OBJ_64)

LIBS_32     =
INCS_32     =
LIBS_64     =
INCS_64     =

CFLAGS_32   = $(INCS_32) -std=c11 -Wall -pedantic
CFLAGS_64   = $(INCS_64) -std=c11 -Wall -pedantic

BIN_32      = StrQueue-win32.dll
BIN_64      = StrQueue-x86_64.dll

CFLAGS      = -std=c99   -Wall -pedantic #-g3

.PHONY: all all-before all-after clean clean-custom lib32 lib64

all: all-before lib32 lib64 all-after

clean: clean-custom
	rm -rf $(OBJ_32) $(OBJ_64) $(BIN_32) $(BIN_64)

lib32: $(BIN_32)

lib64: $(BIN_64)

$(BIN_32): $(OBJ_32)
	$(CC_32) -shared $(LINKOBJ_32) -o $@

$(BIN_64): $(OBJ_64)
	$(CC_64) -shared $(LINKOBJ_64) -o $@

# Object-file dependencies
$(OBJ_DIR)/StrQueue-32.o: StrQueue.c
	$(CC_32) -c $(CFLAGS_32) $< -o $@

$(OBJ_DIR)/StrQueue-64.o: StrQueue.c
	$(CC_64) -c $(CFLAGS_64) $< -o $@

