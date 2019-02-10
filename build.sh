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
rm -rf memmod-1.0/

mkdir dist && mkdir dist/x64 && mkdir dist/Win32

/mingw32/bin/python2 setup.py bdist_wheel || exit

/mingw32/bin/python3 setup.py bdist_wheel || exit

mv dist/memmod* dist/Win32

rm -rf build/
rm -rf memmod.egg-info/
rm -rf memmod-1.0/

/mingw64/bin/python2 setup.py bdist_wheel || exit

/mingw64/bin/python3 setup.py bdist_wheel || exit

mv dist/memmod* dist/x64

python setup.py sdist || exit

wheel unpack dist/Win32/memmod-1.0-cp27-cp27m-mingw.whl
rm memmod-1.0/memmod/_memmod*
sed -e "s/mingw/win32/g" memmod-1.0/memmod-1.0.dist-info/WHEEL > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/WHEEL
SIGN=$(cat memmod-1.0/memmod-1.0.dist-info/WHEEL | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod-1.0.dist-info/WHEEL)
sed -e "s;WHEEL,sha256=.*$;WHEEL,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
cp Win32/Release27/_memmod.pyd memmod-1.0/memmod
SIGN=$(cat memmod-1.0/memmod/_memmod.pyd | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod/_memmod.pyd)
sed -e "s;_memmod\.pyd,sha256=.*$;_memmod\.pyd,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
wheel pack memmod-1.0
rm -rf memmod-1.0/

wheel unpack dist/x64/memmod-1.0-cp27-cp27m-mingw.whl
rm memmod-1.0/memmod/_memmod*
sed -e "s/mingw/win_amd64/g" memmod-1.0/memmod-1.0.dist-info/WHEEL > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/WHEEL
SIGN=$(cat memmod-1.0/memmod-1.0.dist-info/WHEEL | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod-1.0.dist-info/WHEEL)
sed -e "s;WHEEL,sha256=.*$;WHEEL,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
cp x64/Release27/_memmod.pyd memmod-1.0/memmod
SIGN=$(cat memmod-1.0/memmod/_memmod.pyd | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod/_memmod.pyd)
sed -e "s;_memmod\.pyd,sha256=.*$;_memmod\.pyd,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
wheel pack memmod-1.0
rm -rf memmod-1.0/

wheel unpack dist/Win32/memmod-1.0-cp37-cp37m-mingw.whl
rm memmod-1.0/memmod/_memmod*
sed -e "s/mingw/win32/g" memmod-1.0/memmod-1.0.dist-info/WHEEL > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/WHEEL
sed -e "s/Tag:\ cp37-cp37m/Tag:\ cp36-cp36m/g" memmod-1.0/memmod-1.0.dist-info/WHEEL > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/WHEEL
SIGN=$(cat memmod-1.0/memmod-1.0.dist-info/WHEEL | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod-1.0.dist-info/WHEEL)
sed -e "s;WHEEL,sha256=.*$;WHEEL,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
cp Win32/Release36/_memmod.pyd memmod-1.0/memmod
SIGN=$(cat memmod-1.0/memmod/_memmod.pyd | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod/_memmod.pyd)
sed -e "s;_memmod\.pyd,sha256=.*$;_memmod\.pyd,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
wheel pack memmod-1.0
rm -rf memmod-1.0/

wheel unpack dist/x64/memmod-1.0-cp37-cp37m-mingw.whl
rm memmod-1.0/memmod/_memmod*
sed -e "s/mingw/win_amd64/g" memmod-1.0/memmod-1.0.dist-info/WHEEL > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/WHEEL
sed -e "s/Tag:\ cp37-cp37m/Tag:\ cp36-cp36m/g" memmod-1.0/memmod-1.0.dist-info/WHEEL > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/WHEEL
SIGN=$(cat memmod-1.0/memmod-1.0.dist-info/WHEEL | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod-1.0.dist-info/WHEEL)
sed -e "s;WHEEL,sha256=.*$;WHEEL,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
cp x64/Release36/_memmod.pyd memmod-1.0/memmod
SIGN=$(cat memmod-1.0/memmod/_memmod.pyd | shasum -a 256 | cut -d " " -f 1 | xxd -r -p | base64)
SIGN="${SIGN:0:-1}"
SIZE=$(stat --printf="%s" memmod-1.0/memmod/_memmod.pyd)
sed -e "s;_memmod\.pyd,sha256=.*$;_memmod\.pyd,sha256=$SIGN,$SIZE;g" memmod-1.0/memmod-1.0.dist-info/RECORD > temp.txt
mv temp.txt memmod-1.0/memmod-1.0.dist-info/RECORD
wheel pack memmod-1.0
rm -rf memmod-1.0/

mv memmod-1.0-cp* dist
