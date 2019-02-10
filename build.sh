# Load dll libraries directly from memory
# -*- coding: utf-8 -*-
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

rm -rf build/
rm -rf dist/
rm -rf memmod.egg-info/

mkdir dist && mkdir dist/x64 && mkdir dist/Win32

/mingw32/bin/python2 setup.py bdist_wheel || exit

/mingw32/bin/python3 setup.py bdist_wheel || exit

mv dist/memmod* dist/Win32

/mingw64/bin/python2 setup.py bdist_wheel || exit

/mingw64/bin/python3 setup.py bdist_wheel || exit

mv dist/memmod* dist/x64

python setup.py sdist || exit
