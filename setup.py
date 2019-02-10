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

__author__ = "Mathtin"
__date__ = "$07.02.2019 00:00:00$"

"""
 * Python library for Memory DLL loading
 *
 * Copyright (c) 2019 by Daniel [Mathtin] Shiko / wdaniil@mail.ru
 * https://mathtin.ru
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Initial Developer of the Original Code is Daniel [Mathtin] Shiko.
 *
 * Portions created by Daniel [Mathtin] Shiko are Copyright (C) 2019
 * Daniel [Mathtin] Shiko. All Rights Reserved.
"""

from setuptools import setup, Extension
import distutils.util
from os import name as OS, path
from platform import architecture
from sys import executable as PYEXEPATH
from glob import glob as ls

if OS != "nt":
    raise Exception('Only NT OS is supported')

ARCH        = architecture()[0]

#if distutils.util.get_platform() == 'mingw':
#    if ARCH == '64bit':
#        distutils.util.get_platform = lambda: 'mingw64'
#    elif ARCH == '32bit':
#        distutils.util.get_platform = lambda: 'mingw32'

if ARCH == '64bit':
    if path.join('mingw64', 'bin') in PYEXEPATH:
        ENV = 'mingw64'
    else:
        ENV = 'win64'
elif ARCH == '32bit':
    if path.join('mingw32', 'bin') in PYEXEPATH:
        ENV = 'mingw32'
    else:
        ENV = 'win32'
else:
    raise Exception('Unsupported architecture')

CXXINCS = ''
if 'mingw' in ENV:
    CFLAGS   = ['-std=c11', '-Wall', '-pedantic']
    CXXFLAGS = ['-std=c++17', '-Wall', '-pedantic']
else:
    CFLAGS   = []
    CXXFLAGS = []

def lookup(path_, ext):
    res = []
    for e in ext:
        res += ls(path.join(path_, e))
    return res

_memmod = Extension('memmod._memmod',
    sources = lookup('src', ['*.c']),
    extra_link_args=["-Wl,--add-stdcall-alias"],
    extra_compile_args=CFLAGS)

setup(name = 'memmod',
       version = '1.0.1',
       description = 'loads dll directly from memory',
       long_description = 'Python library used to load dll libraries directly from memory',
       author='Daniel Shiko',
       author_email='wdaniil@mail.ru',
       url='https://github.com/Mathtin/memmod/',
       license='MPL',
       platforms = ['nt'],
	   packages=['memmod'],
	   package_dir={'memmod' : 'src'},
       ext_modules=[_memmod],
       data_files = [])
