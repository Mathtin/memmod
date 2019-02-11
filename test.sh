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

PIP2_32_MINGW=/mingw32/bin/pip2
PIP2_64_MINGW=/mingw64/bin/pip2

PIP3_32_MINGW=/mingw32/bin/pip3
PIP3_64_MINGW=/mingw64/bin/pip3

PYDIR2_32_WIN=/c/Python27/
PYDIR2_64_WIN=/c/Python27amd64/

PYDIR3_32_WIN=/c/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/Shared/Python36_86
PYDIR3_64_WIN=/c/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/Shared/Python36_64

WD=$PWD
cd dist/Win32/

echo "[*] Testing python2 lib for mingw32"
$PIP2_32_MINGW uninstall -y memmod-$MEMMOD_VER-cp27-cp27m-mingw.whl &> /dev/null
$PIP2_32_MINGW install      memmod-$MEMMOD_VER-cp27-cp27m-mingw.whl > /dev/null
$PY2_32_MINGW -c 'from memmod.tests.test_memmod import StrQueueTest;import unittest; unittest.main()' > /dev/null || exit
$PIP2_32_MINGW uninstall -y memmod-$MEMMOD_VER-cp27-cp27m-mingw.whl &> /dev/null

echo "[*] Testing python3 lib for mingw32"
$PIP3_32_MINGW uninstall -y memmod-$MEMMOD_VER-cp37-cp37m-mingw.whl &> /dev/null
$PIP3_32_MINGW install      memmod-$MEMMOD_VER-cp37-cp37m-mingw.whl > /dev/null
$PY3_32_MINGW -c 'from memmod.tests.test_memmod import StrQueueTest;import unittest; unittest.main()' > /dev/null || exit
$PIP3_32_MINGW uninstall -y memmod-$MEMMOD_VER-cp37-cp37m-mingw.whl &> /dev/null

cd $WD
cd dist/x64/

echo "[*] Testing python2 lib for mingw64"
$PIP2_64_MINGW uninstall -y memmod-$MEMMOD_VER-cp27-cp27m-mingw.whl &> /dev/null
$PIP2_64_MINGW install      memmod-$MEMMOD_VER-cp27-cp27m-mingw.whl > /dev/null
$PY2_64_MINGW -c 'from memmod.tests.test_memmod import StrQueueTest;import unittest; unittest.main()' > /dev/null || exit
$PIP2_64_MINGW uninstall -y memmod-$MEMMOD_VER-cp27-cp27m-mingw.whl &> /dev/null

echo "[*] Testing python3 lib for mingw64"
$PIP3_64_MINGW uninstall -y memmod-$MEMMOD_VER-cp37-cp37m-mingw.whl &> /dev/null
$PIP3_64_MINGW install      memmod-$MEMMOD_VER-cp37-cp37m-mingw.whl > /dev/null
$PY3_64_MINGW -c 'from memmod.tests.test_memmod import StrQueueTest;import unittest; unittest.main()' > /dev/null || exit
$PIP3_64_MINGW uninstall -y memmod-$MEMMOD_VER-cp37-cp37m-mingw.whl &> /dev/null

cd $WD
cd dist

OLDPATH=$PATH
PATH=$PYDIR3_64_WIN:$PYDIR3_64_WIN/Scripts:$PATH
echo "[*] Testing python3 lib for x64"
python.exe -m pip uninstall -y memmod-$MEMMOD_VER-cp36-cp36m-win_amd64.whl &> /dev/null
python.exe -m pip install      memmod-$MEMMOD_VER-cp36-cp36m-win_amd64.whl > /dev/null
python.exe -c 'from memmod.tests.test_memmod import StrQueueTest;import unittest; unittest.main()' > /dev/null || exit
python.exe -m pip uninstall -y memmod-$MEMMOD_VER-cp36-cp36m-win_amd64.whl &> /dev/null
PATH=$OLDPATH
exit

OLDPATH=$PATH
PATH=$PYDIR2_32_WIN:$PYDIR2_32_WIN/Scripts:$PATH
echo "[*] Testing python2 lib for win32"
python.exe -m pip uninstall -y memmod-$MEMMOD_VER-cp27-cp27m-win32.whl &> /dev/null
python.exe -m pip install      memmod-$MEMMOD_VER-cp27-cp27m-win32.whl > /dev/null
python.exe -c 'from memmod.tests.test_memmod import StrQueueTest;import unittest; unittest.main()' > /dev/null || exit
python.exe -m pip uninstall -y memmod-$MEMMOD_VER-cp27-cp27m-win32.whl &> /dev/null
PATH=$OLDPATH

exit

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
