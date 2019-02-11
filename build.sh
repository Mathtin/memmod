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

MEMMOD_VER=1.1.0

PY2_32_MINGW=/mingw32/bin/python2
PY2_64_MINGW=/mingw64/bin/python2

PY3_32_MINGW=/mingw32/bin/python3
PY3_64_MINGW=/mingw64/bin/python3

PYDIR2_32_WIN=/c/Python27/
PYDIR2_64_WIN=/c/Python27amd64/

PYDIR3_32_WIN=/c/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/Shared/Python36_86
PYDIR3_64_WIN=/c/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/Shared/Python36_64

WD=$PWD

echo "[*] Cleaning directory"
git clean -xdf > /dev/null

if test $# -gt 0 && test $1 = "clean"; then
    exit
fi

echo "[*] Making additionoal directories"
mkdir obj && mkdir dist && mkdir dist/x64 && mkdir dist/Win32 || exit

echo "[!] Building memmod $MEMMOD_VER"

echo "[*] Building unit tests"
cd tests
make > /dev/null || exit
cd ..

# Set library version
echo "[*] setup.py.in > setup.py"
sed -e "s/%MEMMOD_VER%/'$MEMMOD_VER'/g" setup.py.in > setup.py || exit

echo "[*] Building python2 lib for mingw32"
$PY2_32_MINGW setup.py bdist_wheel > /dev/null || exit

echo "[*] Building python3 lib for mingw32"
$PY3_32_MINGW setup.py bdist_wheel > /dev/null || exit

mv dist/memmod* dist/Win32 || exit
echo "[!] Wheels available in dist/Win32"

rm -rf build &> /dev/null

echo "[*] Building python2 lib for mingw64"
$PY2_64_MINGW setup.py bdist_wheel > /dev/null || exit

echo "[*] Building python3 lib for mingw64"
$PY3_64_MINGW setup.py bdist_wheel > /dev/null || exit

mv dist/memmod* dist/x64
echo "[!] Wheels available in dist/x64"

echo "[*] Building source dist package"
python setup.py sdist > /dev/null || exit
echo "[!] Source dist available in dist"

rm -rf build &> /dev/null

OLDPATH=$PATH
PATH=$PYDIR2_32_WIN:$PYDIR2_32_WIN/Scripts:$PATH
echo "[*] Building python2 lib for win32"
python.exe setup.py bdist_wheel &> /dev/null || exit
PATH=$OLDPATH

OLDPATH=$PATH
PATH=$PYDIR2_64_WIN:$PYDIR2_64_WIN/Scripts:$PATH
echo "[*] Building python2 lib for x64"
python.exe setup.py bdist_wheel &> /dev/null || exit
PATH=$OLDPATH

OLDPATH=$PATH
PATH=$PYDIR3_32_WIN:$PYDIR3_32_WIN/Scripts:$PATH
echo "[*] Building python3 lib for win32"
python.exe setup.py bdist_wheel &> /dev/null || exit
PATH=$OLDPATH

OLDPATH=$PATH
PATH=$PYDIR3_64_WIN:$PYDIR3_64_WIN/Scripts:$PATH
echo "[*] Building python3 lib for x64"
python.exe setup.py bdist_wheel &> /dev/null || exit
PATH=$OLDPATH
