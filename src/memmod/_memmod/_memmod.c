#include "_memmod.h"
#include <time.h>

/*
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
 */
 
 struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static FILE* logfile;
static int loglevel;

void get_time(char * buffer) {
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S %%s\n", tm_info);
}

static void _log(int log_type, const char * str) {
    char str_base[40];
    if (logfile == NULL || log_type > loglevel) {
        return;
    }
    switch(log_type) {
    case LOG_ERROR: memcpy(str_base, LOG_ERROR_STR, sizeof(LOG_ERROR_STR)); break;
    case LOG_INFO:  memcpy(str_base, LOG_INFO_STR,  sizeof(LOG_INFO_STR));  break;
    case LOG_DEBUG: memcpy(str_base, LOG_DEBUG_STR, sizeof(LOG_DEBUG_STR)); break;
    }
    get_time(str_base + LOG_STR_PADDING);
    fprintf(logfile, str_base, str);
    fflush(logfile);
}

static PyObject * error_out(PyObject *m) {
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
    return NULL;
}

static PyObject * memmod_SetLogFile(PyObject *self, PyObject *args) {
    LPCSTR filename;
    _log(LOG_DEBUG, "set log call");
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        _log(LOG_DEBUG, "bad args");
        return NULL;
    }
    if (logfile) {
        fclose(logfile);
        logfile = NULL;
    }
    logfile = fopen(filename, "a");
    return Py_BuildValue("K", logfile);
}

static PyObject * memmod_SetLogLevel(PyObject *self, PyObject *args) {
    _log(LOG_DEBUG, "set log call");
    if (!PyArg_ParseTuple(args, "i", &loglevel)) {
        _log(LOG_DEBUG, "bad args");
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject * memmod_MemoryLoadLibrary(PyObject *self, PyObject *args) {
    const void *data;
    size_t size;
    HMEMORYMODULE handle;
    _log(LOG_DEBUG, "load library call");
#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "y#", &data, &size)) {
#else
    if (!PyArg_ParseTuple(args, "t#", &data, &size)) {
#endif
        _log(LOG_DEBUG, "bad args");
        return NULL;
    }
    handle = MemoryLoadLibrary(data, size);
    if (handle == NULL) {
        _log(LOG_DEBUG, "failed to load library");
        PyErr_SetString(PyExc_Exception, "Expected a COM this pointer as first argument");
        return NULL;
    }
#ifdef _WIN64
    return Py_BuildValue("K", (size_t)handle);
#else
    return Py_BuildValue("I", (size_t)handle);
#endif
}

static PyObject * memmod_MemoryGetProcAddress(PyObject *self, PyObject *args) {
    HMEMORYMODULE handle;
    LPCSTR name;
    FARPROC func;
    _log(LOG_DEBUG, "get proc call");
    if (!PyArg_ParseTuple(args, "ns", &handle, &name)) {
        _log(LOG_DEBUG, "bad args");
        return NULL;
    }
    func = MemoryGetProcAddress(handle, name);
#ifdef _WIN64
    return Py_BuildValue("K", (size_t)func);
#else
    return Py_BuildValue("I", (size_t)func);
#endif
}

static PyObject * memmod_MemoryFreeLibrary(PyObject *self, PyObject *args) {
    HMEMORYMODULE handle;
    _log(LOG_DEBUG, "free library call");
    if (!PyArg_ParseTuple(args, "n", &handle)) {
        _log(LOG_DEBUG, "bad args");
        return NULL;
    }
    MemoryFreeLibrary(handle);
    Py_RETURN_NONE;
}

static int _get_name(PyObject *obj, const char **pname) {
    if (PyLong_Check(obj)) {
        /* We have to use MAKEINTRESOURCEA for Windows CE.
         * Works on Windows as well, of course.
        */
        *pname = MAKEINTRESOURCEA(PyLong_AsUnsignedLongMask(obj) & 0xFFFF);
        return 1;
    }
    if (PyBytes_Check(obj)) {
        *pname = PyBytes_AsString(obj);
        return *pname ? 1 : 0;
    }
    if (PyUnicode_Check(obj)) {
        #if PY_MAJOR_VERSION >= 3
        *pname = PyUnicode_AsUTF8(obj);
        #else
        *pname = PyBytes_AsString(obj);
        #endif
        return *pname ? 1 : 0;
    }
    PyErr_SetString(PyExc_TypeError,
                    "function name must be string, bytes object or integer");
    return 0;
}

static PyObject * memmod_InitCFuncPtr(PyObject *self_, PyObject *args) {
    PyCFuncPtrObject * self;
    const char *name;
    FARPROC address;
    PyObject *ftuple;
    PyObject *dll;
    PyObject *obj;
    HMEMORYMODULE handle;
    PyObject *paramflags = NULL;
    _log(LOG_DEBUG, "InitCFuncPtr call");
    // InitCFuncPtr(self, (ftuple), paramflags)
    if (!PyArg_ParseTuple(args, "OO|O", (PyObject *)&self, &ftuple, &paramflags)) {
        return NULL;
    }
    if (paramflags == Py_None) {
        paramflags = NULL;
    }
    ftuple = PySequence_Tuple(ftuple);
    if (!ftuple) {
        /* Here ftuple is a borrowed reference */
        return NULL;
    }
    // ("func_name", dll_handle)
    if (!PyArg_ParseTuple(ftuple, "O&O;illegal func_spec argument", _get_name, &name, &dll)) {
        Py_DECREF(ftuple);
        return NULL;
    }
    // dll._handle
    obj = PyObject_GetAttrString(dll, "_handle");
    if (!obj) {
        Py_DECREF(ftuple);
        return NULL;
    }
#if !defined(_WIN64) && PY_MAJOR_VERSION < 3
    if (!PyInt_Check(obj)) {
#else
    if (!PyLong_Check(obj)) {
#endif
        PyErr_SetString(PyExc_TypeError,
                        "the _handle attribute of the second argument must be an integer");
        Py_DECREF(ftuple);
        Py_DECREF(obj);
        return NULL;
    }
    handle = (HMEMORYMODULE)PyLong_AsVoidPtr(obj);
    Py_DECREF(obj);
    if (PyErr_Occurred()) {
        PyErr_SetString(PyExc_ValueError,
                        "could not convert the _handle attribute to a pointer");
        Py_DECREF(ftuple);
        return NULL;
    }
    address = MemoryGetProcAddress(handle, name);
    _log(LOG_DEBUG, "InitCFuncPtr step");
    if (!address) {
        if (!IS_INTRESOURCE(name))
            PyErr_Format(PyExc_AttributeError,
                         "function '%s' not found",
                         name);
        else
            PyErr_Format(PyExc_AttributeError,
                         "function ordinal %d not found",
                         (WORD)(size_t)name);
        Py_DECREF(ftuple);
        return NULL;
    }

    Py_XINCREF(paramflags);
    self->paramflags = paramflags;

    *(void **)self->b_ptr = (void*)(Py_ssize_t)address;
    Py_DECREF(ftuple);

    Py_INCREF(self);
    self->callable = (PyObject *)self;
    return (PyObject *)self;
}

static PyMethodDef _memmod_module_methods[] = {
    {"error_out", (PyCFunction)error_out, METH_NOARGS, NULL},
    
    {"SetLogFile",            memmod_SetLogFile,            METH_VARARGS,
     "Set file for logging"},
    {"SetLogLevel",           memmod_SetLogLevel,           METH_VARARGS,
     "Set logging level"},
    {"MemoryLoadLibrary",     memmod_MemoryLoadLibrary,     METH_VARARGS,
     "Loads dll from memory"},
    {"MemoryGetProcAddress",  memmod_MemoryGetProcAddress,  METH_VARARGS,
     "Resolves function address"},
    {"MemoryFreeLibrary",     memmod_MemoryFreeLibrary,     METH_VARARGS,
     "Unloads dll from memory"},
    {"InitCFuncPtr",          memmod_InitCFuncPtr,          METH_VARARGS,
     "Init CFuncPtr Object"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static int memmod_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int memmod_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static const char module_docs[] =
"Load dll libraries directly from memory";

static struct PyModuleDef _memmodmodule = {
        PyModuleDef_HEAD_INIT,
        "_memmod",
        module_docs,
        sizeof(struct module_state),
        _memmod_module_methods,
        NULL,
        memmod_traverse,
        memmod_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC PyInit__memmod(void) 

#else // PY_MAJOR_VERSION == 2

#define INITERROR return
EXP_FUNC void init_memmod(void)

#endif // ! Python version
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&_memmodmodule);
#else
    PyObject *module = Py_InitModule("_memmod", _memmod_module_methods);
#endif
    struct module_state *st;

    logfile = NULL;
#ifdef _DEBUG
    loglevel = LOG_DEBUG;
#else
    loglevel = LOG_ERROR;
#endif

    if (!module) {
        INITERROR;
    }
    st = GETSTATE(module);

    st->error = PyErr_NewException("memmod.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
