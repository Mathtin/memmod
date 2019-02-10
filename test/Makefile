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

CPP      = g++ #-D__DEBUG__
CC       = gcc #-D__DEBUG__
OBJ      = StrQueue.o
LINKOBJ  = StrQueue.o
BIN      = StrQueue.dll
CXXFLAGS = -std=c++11 -Wall -pedantic #-g3
CFLAGS   = -std=c99   -Wall -pedantic #-g3
DEF      = libStrQueue.def
STATIC   = libStrQueue.a

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	rm -rf $(OBJ) $(BIN)

# Object-file dependencies
StrQueue.o: StrQueue.c
	$(CC) -c $(CFLAGS) $< -o $@
	
$(BIN): $(OBJ)
	$(CC) -shared $(OBJ) -o $@ -Wl,--add-stdcall-alias
