#ifndef MEMMOD_H
#define MEMMOD_H

/*
 * Python library for Memory DLL loading
 *
 * Copyright (c) 2019 by Daniil [Mathtin] Shigapov / wdaniil@mail.ru
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
 * The Initial Developer of the Original Code is Daniil [Mathtin] Shigapov.
 *
 * Portions created by Daniil [Mathtin] Shigapov are Copyright (C) 2019
 * Daniil [Mathtin] Shigapov. All Rights Reserved.
 */

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "MemoryModule.h"

#define LOG_ERROR   1
#define LOG_INFO    2
#define LOG_DEBUG   3

#define LOG_STR_PADDING 8
#define LOG_ERROR_STR   "[ERROR] "
#define LOG_INFO_STR    "[INFO]  "
#define LOG_DEBUG_STR   "[DEBUG] "

#if !defined(__MINGW32__) //MinGW32 does not support _declspec
    #define EXP_FUNC extern _declspec(dllexport)
#else //defined(__WINDOWS__) && !defined(__MINGW32__)
    #define EXP_FUNC
#endif //defined(__WINDOWS__) && !defined(__MINGW32__)

union value {
                char c[16];
                short s;
                int i;
                long l;
                float f;
                double d;
                long long ll;
                long double D;
};

typedef struct {
    /* First part identical to tagCDataObject */
    PyObject_HEAD
    char *b_ptr;                /* pointer to memory block */
    int  b_needsfree;           /* need _we_ free the memory? */
    void *b_base;        /* pointer to base object or NULL */
    Py_ssize_t b_size;          /* size of memory block in bytes */
    Py_ssize_t b_length;        /* number of references we need */
    Py_ssize_t b_index;         /* index of this object into base's
                               b_object list */
    PyObject *b_objects;        /* list of references we need to keep */
    union value b_value;
    /* end of tagCDataObject, additional fields follow */

    void *thunk;
    PyObject *callable;

    /* These two fields will override the ones in the type's stgdict if
       they are set */
    PyObject *converters;
    PyObject *argtypes;
    PyObject *restype;
    PyObject *checker;
    PyObject *errcheck;
    int index;
    GUID *iid;
    PyObject *paramflags;
} PyCFuncPtrObject;

#endif
