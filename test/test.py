#!/usr/bin/env python3
# -*- coding: utf-8 -*-
###################################################
#........../\./\...___......|\.|..../...\.........#
#........./..|..\/\.|.|_|._.|.\|....|.c.|.........#
#......../....../--\|.|.|.|i|..|....\.../.........#
#        Mathtin (c)                              #
###################################################
#	Author: Daniil [Mathtin] Shigapov             #
#   Copyright (c) 2019 <wdaniil@mail.ru>          #
#   This file is released under the MIT license.  #
###################################################
    
__author__ = "Mathtin"
__date__ = "$05.11.2015 17:43:27$"

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
        print('[!] Cratring new instance')
        self.__handle = self.__New()
    
    def __del__(self):
        print('[!] Destructing instance')
        self.__Delete(self.__handle)
        
    def push(self, s):
        self.__Push(self.__handle, ctypes.c_char_p(s.encode('utf-8')), len(s))
    
    def pop(self):
        buffer = bytes(255)
        self.__Pop(self.__handle, ctypes.c_char_p(buffer), len(buffer))
        return buffer[:buffer.index(0)].decode('utf-8')
        
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