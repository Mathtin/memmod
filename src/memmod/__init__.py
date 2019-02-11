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
__date__ = "$07.02.20019 00:00:00$"

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

import ctypes
from ._memmod import *

PARAMFLAG_FIN   = 0x1
PARAMFLAG_FOUT  = 0x2
PARAMFLAG_FLCID = 0x4

class MemCDLL(ctypes.CDLL):
    _func_flags_ = ctypes._FUNCFLAG_CDECL
        
    def __init__(self, data, 
                    mode=ctypes.DEFAULT_MODE,
                    use_errno=False,
                    use_last_error=False):
        handle = MemoryLoadLibrary(data)
        super(MemCDLL, self).__init__("[AT %X]" % handle, mode, handle, use_errno, use_last_error)
       
    def close(self):
        MemoryFreeLibrary(self._handle)
        self._handle = None
    
    def __bool__(self):
        return self._handle != None

class MemDLL(ctypes.WinDLL):
    _func_flags_ = ctypes._FUNCFLAG_STDCALL
    
    def __init__(self, data, 
                    mode=ctypes.DEFAULT_MODE,
                    use_errno=False,
                    use_last_error=False):
        handle = MemoryLoadLibrary(data)
        super(MemDLL, self).__init__("[AT %X]" % handle, mode, handle, use_errno, use_last_error)
       
    def close(self):
        MemoryFreeLibrary(self._handle)
        self._handle = None
    
    def __bool__(self):
        return self._handle != None

_c_functype_cache = {}
def CFUNCTYPE(restype, *argtypes, **kw):
    func_proto = ctypes.CFUNCTYPE(restype, *argtypes, **kw)
    flags = func_proto._flags_
    try:
        return _c_functype_cache[(restype, argtypes, flags)]
    except KeyError:
        class MemCFunction(object):
            _base_ = func_proto
            
            def __init__(self, tuple, paramflags=None):
                name, dll = tuple
                if not paramflags is None:
                    check_paramflags(argtypes, paramflags)
                self.__cfunc = self._base_()
                InitCFuncPtr(self.__cfunc, (name, dll), paramflags)
                self._dll = dll
                
            def __call__(self, *args, **kwargs):
                if not self._dll:
                    raise Exception("DLL library unloaded")
                return self.__cfunc(*args, **kwargs)
            
        _c_functype_cache[(restype, argtypes, flags)] = MemCFunction
        return MemCFunction

_win_functype_cache = {}
def WINFUNCTYPE(restype, *argtypes, **kw):
    func_proto = ctypes.WINFUNCTYPE(restype, *argtypes, **kw)
    flags = func_proto._flags_
    try:
        return _win_functype_cache[(restype, argtypes, flags)]
    except KeyError:
        class MemFunction(object):
            _base_ = func_proto
            
            def __init__(self, tuple, paramflags=None):
                name, dll = tuple
                if not paramflags is None:
                    check_paramflags(argtypes, paramflags)
                self.__cfunc = self._base_()
                InitCFuncPtr(self.__cfunc, (name, dll), paramflags)
                self._dll = dll
                
            def __call__(self, *args, **kwargs):
                if not self._dll:
                    raise Exception("DLL library unloaded")
                return self.__cfunc(*args, **kwargs)
            
        _win_functype_cache[(restype, argtypes, flags)] = MemFunction
        return MemFunction
        
def check_paramflags(argtypes, paramflags):
    if not isinstance(paramflags, tuple):
        raise TypeError("paramflags must be a tuple or None")
    if len(argtypes) != len(paramflags):
        raise ValueError("paramflags must have the same length as argtypes")
    for flag in paramflags:
        if not isinstance(flag, tuple) or not check_flagtypes(*flag):
            raise TypeError("paramflags must be a sequence of (int [,string [,value]]) tuples")
        if flag[0] in [0, PARAMFLAG_FIN, PARAMFLAG_FIN | PARAMFLAG_FLCID, PARAMFLAG_FIN | PARAMFLAG_FOUT]:
            continue
        elif flag[0] == PARAMFLAG_FOUT:
            continue
        else:
            raise TypeError("paramflag value %d not supported" % flag)
        
def check_flagtypes(i=None, s=None, val=None):
    if not isinstance(i, int):
        return False
    if s and not isinstance(s, str):
        return False
    return True
    