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

import os, sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../src')))

import memmod
import ctypes
import unittest
from platform import architecture
from os import path

if sys.version_info > (3,):
    long = int

ARCH = architecture()[0]
DIR = path.dirname(__file__)

if ARCH == '64bit':
    ARCH_SUFFIX = 'x86_64'
elif ARCH == '32bit':
    ARCH_SUFFIX = 'win32'
else:
    raise Exception('Unsupported architecture')

class StrQueueTest(unittest.TestCase):
    __dll = None
        
    __New     = None
    __Delete  = None
    __PushPop = None
    __Len     = None
    __Empty   = None
    
    def setUp(self):
        self.__handle = None
    
    def __del__(self):
        if self.__handle:
            self.__Delete(self.__handle)
    
    def object_load_dll(self):
        #memmod.SetLogLevel(3)
        #memmod.SetLogFile('memmod.log')
        
        dllfile = open(path.join(DIR, 'StrQueue-%s.dll' % ARCH_SUFFIX), 'rb')
        data = dllfile.read()
        dllfile.close()
        dll = memmod.MemCDLL(data)
        self.assertIsNotNone(dll, 'Handle not recieved')
        self.assertNotEqual(dll, 0, 'Invalid handle')
        StrQueueTest.__dll = dll
        
        StrQueue_New_proto     = memmod.CFUNCTYPE(ctypes.c_void_p)
        StrQueue_Delete_proto  = memmod.CFUNCTYPE(None, ctypes.c_void_p)
        StrQueue_PushPop_proto = memmod.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_size_t)
        StrQueue_Len_proto     = memmod.CFUNCTYPE(ctypes.c_size_t, ctypes.c_size_t)
        StrQueue_Empty_proto   = memmod.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p)
        
        self.assertIsNotNone(StrQueue_New_proto,        'CFUNCTYPE returned None')
        self.assertIsNotNone(StrQueue_Delete_proto,     'CFUNCTYPE returned None')
        self.assertIsNotNone(StrQueue_PushPop_proto,    'CFUNCTYPE returned None')
        self.assertIsNotNone(StrQueue_Len_proto,        'CFUNCTYPE returned None')
        self.assertIsNotNone(StrQueue_Empty_proto,      'CFUNCTYPE returned None')

        StrQueueTest.__New    = StrQueue_New_proto(("StrQueue_New", dll))
        StrQueueTest.__Delete = StrQueue_Delete_proto(("StrQueue_Delete", dll))
        StrQueueTest.__Push   = StrQueue_PushPop_proto(("StrQueue_Push", dll))
        StrQueueTest.__Pop    = StrQueue_PushPop_proto(("StrQueue_Pop", dll), ((memmod.PARAMFLAG_FIN,), (memmod.PARAMFLAG_FIN | memmod.PARAMFLAG_FOUT,), (memmod.PARAMFLAG_FIN,)))
        StrQueueTest.__Len    = StrQueue_Len_proto(("StrQueue_Len", dll))
        StrQueueTest.__Empty  = StrQueue_Empty_proto(("StrQueue_Empty", dll))
        
        self.assertIsInstance(StrQueueTest.__New,       StrQueue_New_proto,     'CFUNCTYPE proto returned invalid object')
        self.assertIsInstance(StrQueueTest.__Delete,    StrQueue_Delete_proto,  'CFUNCTYPE proto returned invalid object')
        self.assertIsInstance(StrQueueTest.__Push,      StrQueue_PushPop_proto, 'CFUNCTYPE proto returned invalid object')
        self.assertIsInstance(StrQueueTest.__Pop,       StrQueue_PushPop_proto, 'CFUNCTYPE proto returned invalid object')
        self.assertIsInstance(StrQueueTest.__Len,       StrQueue_Len_proto,     'CFUNCTYPE proto returned invalid object')
        self.assertIsInstance(StrQueueTest.__Empty,     StrQueue_Empty_proto,   'CFUNCTYPE proto returned invalid object')
        
    def object_init(self):
        self.__handle = self.__New()
        self.assertIsNotNone(self.__handle, 'Queue handle not recieved')
        self.assertNotEqual(self.__handle, 0, 'Invalid queue handle')
        
    def object_push(self, s):
        buffer = s.encode('utf-8')
        self.__Push(self.__handle, ctypes.c_char_p(buffer), len(buffer))
    
    def object_pop(self):
        buffer = bytes(1) * 255
        self.__Pop(self.__handle, ctypes.c_char_p(buffer), len(buffer))
        return buffer[:buffer.index(b'\0')].decode('utf-8')
        
    def object_len(self):
        res = self.__Len(self.__handle)
        self.assertIsInstance(res, long)
        return res
        
    def object_empty(self):
        res = self.__Empty(self.__handle)
        self.assertIsInstance(res, int)
        return res != 0
        
    def test_queue(self):
        self.object_load_dll()
        self.object_init()
        
        self.assertTrue(self.object_empty())
        self.assertEqual(self.object_len(), long(0))
        
        s1 = 'Hello World'
        self.object_push(s1)
        
        self.assertFalse(self.object_empty())
        self.assertEqual(self.object_len(), long(1))
        
        s2 = 'Hello World Again'
        self.object_push(s2)
        
        self.assertEqual(self.object_len(), long(2))
        
        self.assertEqual(self.object_pop(), s1)
        
        self.assertEqual(self.object_len(), long(1))
        
        self.assertEqual(self.object_pop(), s2)
        
        self.assertEqual(self.object_len(), long(0))
        self.assertTrue(self.object_empty())
    

if __name__ == "__main__":
    unittest.main()
