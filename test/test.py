#!/usr/bin/env python3
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
 * Test file for memmod project
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

import memmod
import ctypes

class StrQueue(object):
    __dll = None
        
    __New     = None
    __Delete  = None
    __PushPop = None
    __Len     = None
    __Empty   = None
    
    @staticmethod
    def load_dll(data):
        print('[!] Loading dll')
        dll = memmod.MemDLL(data)
        if not dll:
            raise Exception('Failed to load dll')
        StrQueue.__dll = dll
        StrQueue_New_proto     = memmod.WINFUNCTYPE(ctypes.c_void_p)
        StrQueue_Delete_proto  = memmod.WINFUNCTYPE(None, ctypes.c_void_p)
        StrQueue_PushPop_proto = memmod.WINFUNCTYPE(None, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_size_t)
        StrQueue_Len_proto     = memmod.WINFUNCTYPE(ctypes.c_size_t, ctypes.c_void_p)
        StrQueue_Empty_proto   = memmod.WINFUNCTYPE(ctypes.c_int, ctypes.c_void_p)

        StrQueue.__New    = StrQueue_New_proto(("StrQueue_New", dll))
        StrQueue.__Delete = StrQueue_Delete_proto(("StrQueue_Delete", dll))
        StrQueue.__Push   = StrQueue_PushPop_proto(("StrQueue_Push", dll))
        StrQueue.__Pop    = StrQueue_PushPop_proto(("StrQueue_Pop", dll), ((memmod.PARAMFLAG_FIN,), (memmod.PARAMFLAG_FIN | memmod.PARAMFLAG_FOUT,), (memmod.PARAMFLAG_FIN,)))
        StrQueue.__Len    = StrQueue_Len_proto(("StrQueue_Len", dll))
        StrQueue.__Empty  = StrQueue_Empty_proto(("StrQueue_Empty", dll))
        
    def __init__(self):
        print('[!] Constructing instance')
        self.__handle = self.__New()
    
    def __del__(self):
        print('[!] Destructing instance')
        self.__Delete(self.__handle)
        
    def push(self, s):
        buffer = s.encode('utf-8')
        self.__Push(self.__handle, ctypes.c_char_p(buffer), len(buffer))
    
    def pop(self):
        buffer = bytes(1) * 255
        self.__Pop(self.__handle, ctypes.c_char_p(buffer), len(buffer))
        return buffer[:buffer.index('\0')].decode('utf-8')
        
    def __len__(self):
        return self.__Len(self.__handle)
        
    def __bool__(self):
        return not self.empty()
        
    def empty(self):
        return self.__Empty(self.__handle) != 0

def main():
    dllfile = open('StrQueue.dll', 'rb')
    data = dllfile.read()
    dllfile.close()
    memmod.SetLogLevel(3)
    memmod.SetLogFile("memmod.log")
    
    
    StrQueue.load_dll(data)
    q = StrQueue()
    
    print('>> q.empty()')
    print(q.empty())
    
    print('>> len(q)')
    print(len(q))
    
    print('>> q.push(\'Hello World\')')
    q.push('Hello World')
    
    print('>> q.empty()')
    print(q.empty())
    
    print('>> len(q)')
    print(len(q))
    
    print('>> q.push(\'Hello World Again\')')
    q.push('Hello World Again')
    
    print('>> len(q)')
    print(len(q))
    
    print('>> q.pop()')
    print(q.pop())
    
    print('>> q.pop()')
    print(q.pop())
    
    print('>> len(q)')
    print(len(q))
    

if __name__ == "__main__":
    main()
