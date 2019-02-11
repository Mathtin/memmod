#ifndef StrQueueDll_H
#define StrQueueDll_H

/*
 * StrQueue DLL library
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

/*---------------------------------------------------------------------------*/
/*Win32*/
#if defined(__NT__) || defined(_WIN32) || defined(WIN32)
    #ifndef WIN32
        #define WIN32
    #endif
    #ifndef _WIN32
        #define _WIN32
    #endif
    #ifndef __WIN32__
        #define __WIN32__ 1
    #endif
#endif

/*---------------------------------------------------------------------------*/
/*Win64*/
#if defined(_WIN64) || defined(WIN64)
    #ifndef WIN64
        #define WIN64
    #endif
    #ifndef _WIN64
        #define _WIN64
    #endif
    #ifndef __WIN64__
        #define __WIN64__ 1
    #endif
#endif

/*---------------------------------------------------------------------------*/
/*Windows*/
#if defined(WIN32) || defined(WIN64)
    #ifndef WINDOWS
        #define WINDOWS
    #endif
    #ifndef _WINDOWS
        #define _WINDOWS
    #endif
    #ifndef __WINDOWS__
        #define __WINDOWS__ 1
    #endif
#endif

/*---------------------------------------------------------------------------*/
/*Unix (Linux, HP, Sun, BeOS...)*/
#if defined(UNIX) || defined(_UNIX) || defined(__UNIX__)                    \
    || defined(__unix) || defined(__unix__)                                 \
    || defined(____SVR4____) || defined(__LINUX__) || defined(__sgi)        \
    || defined(__hpux) || defined(sun) || defined(__SUN__) || defined(_AIX) \
    || defined(__EMX__) || defined(__VMS) || defined(__BEOS__)
    #ifndef UNIX
        #define UNIX
    #endif
    #ifndef _UNIX
        #define _UNIX
    #endif
    #ifndef __UNIX__
        #define __UNIX__ 1
    #endif
#endif

/*---------------------------------------------------------------------------*/
/*MacOS Classic*/
#if defined(macintosh)
    #ifndef MACOS
        #define MACOS
    #endif
    #ifndef _MACOS
        #define _MACOS
    #endif
    #ifndef __MACOS__
        #define __MACOS__ 1
    #endif
#endif

/*---------------------------------------------------------------------------*/
/*MacOS X*/
#if defined(__APPLE__) && defined(__MACH__)
    #ifndef MACOSX
        #define MACOSX
    #endif
    #ifndef _MACOSX
        #define _MACOSX
    #endif
    #ifndef __MACOSX__
        #define __MACOSX__ 1
    #endif
#endif

/*Test of targets*/
#if defined(WINDOWS) && defined(UNIX) && defined(MACOS) && defined(MACOSX)
    #pragma message Multiple platforms???
#endif

#if !defined(WIN32) && !defined(UNIX) && !defined(MACOS) && !defined(MACOSX)
    #pragma message No known platforms, assume default
#endif
/*-------------------------------------------------------------------------*/
#if defined(_WIN32) || defined(WIN32)
    #ifdef _UNICODE
        #define STRQUEUEDLL_NAME L"StrQueue.dll"
    #else //_UNICODE
        #define STRQUEUEDLL_NAME "StrQueue.dll"
    #endif //_UNICODE
#elif defined(__APPLE__) && defined(__MACH__)
    #define STRQUEUEDLL_NAME "libstrqueue.0.dylib"
    #define __stdcall
    #ifdef __cplusplus
        #include <new> //for size_t
    #endif /* __cplusplus */
#else
    #define STRQUEUEDLL_NAME "libstrqueue.so.0"
    #define __stdcall
#endif //!defined(_WIN32) || defined(WIN32)

/*-------------------------------------------------------------------------*/
#undef STRQUEUE_EXP
#if defined(__WINDOWS__) && !defined(__MINGW32__) //MinGW32 does not support _declspec
    #ifdef STRQUEUE_DLL_EXPORT
        #define STRQUEUE_EXP extern _declspec(dllexport) __cdecl 
    #else
        #define STRQUEUE_EXP extern _declspec(dllimport) __cdecl 
    #endif
#else //defined(__WINDOWS__) && !defined(__MINGW32__)
    #define STRQUEUE_EXP __attribute__((__cdecl__))
#endif //defined(__WINDOWS__) && !defined(__MINGW32__)

#if !defined(__WINDOWS__)
    #define __stdcall //Supported only on windows
#endif //!defined(__WINDOWS__)

#include <stddef.h>
#define MAXLINELENGTH 256

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    static HMODULE StrQueue_Module = NULL;
#else
    #ifdef MACOSX
        #include <CoreFoundation/CFBundle.h>
    #endif

    #include <dlfcn.h>
    static void* StrQueue_Module = NULL;
#endif
    static size_t Module_Count = 0;
    
#if defined(_WIN32) || defined(WIN32)
#define STRQUEUE_ASSIGN(_Name,_Name2) \
    StrQueue_##_Name=(STRQUEUE_##_Name)GetProcAddress(StrQueue_Module, "StrQueue"  "_" _Name2); \
    if (StrQueue_##_Name==NULL) Errors++;
#else
#define STRQUEUE_ASSIGN(_Name,_Name2) \
    StrQueue_##_Name=(STRQUEUE_##_Name)dlsym(StrQueue_Module, "StrQueue"  "_" _Name2); \
    if (StrQueue_##_Name==NULL) Errors++;
#endif

typedef void* (*STRQUEUE_New)(void);
static STRQUEUE_New StrQueue_New;
typedef void (*STRQUEUE_Delete)(void*);
static STRQUEUE_Delete StrQueue_Delete;
	
typedef void (*STRQUEUE_Push)(void*, const char*, size_t);
static STRQUEUE_Push StrQueue_Push;
typedef void (*STRQUEUE_Pop)(void*, char*, size_t);
static STRQUEUE_Pop StrQueue_Pop;
	
typedef size_t (*STRQUEUE_Len)(void*);
static STRQUEUE_Len StrQueue_Len;
typedef int (*STRQUEUE_Empty)(void*);
static STRQUEUE_Empty StrQueue_Empty;

static size_t StrQueueDll_Load() {
    size_t Errors = 0;

    if (Module_Count > 0) {
        Module_Count++;
        return 1;
    }
    
    /* Load library */
#if defined(_WIN32) || defined(WIN32)
	#ifdef STRQUEUEDLL_PATH
    	StrQueue_Module = LoadLibrary(STRQUEUEDLL_PATH STRQUEUEDLL_NAME);
	#else
    	StrQueue_Module = LoadLibrary(STRQUEUEDLL_NAME);
	#endif
#else

	#ifdef STRQUEUEDLL_PATH
    	StrQueue_Module = dlopen(STRQUEUEDLL_PATH STRQUEUEDLL_NAME, RTLD_LAZY);
	#else

    #ifdef MACOSX
        StrQueue_Module = dlopen("@executable_path/" STRQUEUEDLL_NAME, RTLD_LAZY);
        if (!StrQueue_Module) {
                CFBundleRef mainBundle = CFBundleGetMainBundle();

                // get full app path and delete app name
                CFURLRef app_url = CFBundleCopyExecutableURL(mainBundle);
                CFURLRef app_path_url = CFURLCreateCopyDeletingLastPathComponent(NULL, app_url);

                CFStringRef app_path = CFURLCopyFileSystemPath(app_path_url, kCFURLPOSIXPathStyle);

                CFMutableStringRef mut_app_path = CFStringCreateMutableCopy(NULL, NULL, app_path);
                CFStringAppend(mut_app_path, CFSTR("/"));
                CFStringAppend(mut_app_path, CFSTR(STRQUEUEDLL_NAME));
                CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
                const char *fullPath = CFStringGetCStringPtr(mut_app_path, encodingMethod);

                StrQueue_Module = dlopen(fullPath, RTLD_LAZY);

                CFRelease(app_url);
                CFRelease(app_path_url);
                CFRelease(app_path);
                CFRelease(mut_app_path);
        }
    #endif /* MACOSX*/

	    if (!StrQueue_Module)
	        StrQueue_Module = dlopen(STRQUEUEDLL_NAME, RTLD_LAZY);
	    if (!StrQueue_Module)
	        StrQueue_Module = dlopen("./" STRQUEUEDLL_NAME, RTLD_LAZY);
	    if (!StrQueue_Module)
	        StrQueue_Module = dlopen("/usr/local/lib/" STRQUEUEDLL_NAME, RTLD_LAZY);
	    if (!StrQueue_Module)
	        StrQueue_Module = dlopen("/usr/local/lib64/" STRQUEUEDLL_NAME, RTLD_LAZY);
	    if (!StrQueue_Module)
	        StrQueue_Module = dlopen("/usr/lib/" STRQUEUEDLL_NAME, RTLD_LAZY);
	    if (!StrQueue_Module)
	        StrQueue_Module = dlopen("/usr/lib64/" STRQUEUEDLL_NAME, RTLD_LAZY);
        
	#endif /*STRQUEUEDLL_PATH*/
#endif
    if (!StrQueue_Module)
        return (size_t)-1;
    
    /* Load methods */
    STRQUEUE_ASSIGN(New, "New")
    STRQUEUE_ASSIGN(Delete, "Delete")
    STRQUEUE_ASSIGN(Push, "Push")
    STRQUEUE_ASSIGN(Pop, "Pop")
    STRQUEUE_ASSIGN(Len, "Len")
    STRQUEUE_ASSIGN(Empty, "Empty")
    
    if (Errors > 0) {
    // Unload DLL with errors
    #if defined(_WIN32) || defined(WIN32)
        FreeLibrary(StrQueue_Module);
    #else
        dlclose(StrQueue_Module);
    #endif
        StrQueue_Module = NULL;
        return (size_t) - 1;
    }
    
	Module_Count++;
    
	return (size_t)1;
}


static size_t StrQueueDll_Unload() {
    if (!Module_Count) return 0;
    Module_Count--;

    if (Module_Count > 1) {
        return 1;
    }

#if defined(_WIN32) || defined(WIN32)
    FreeLibrary(StrQueue_Module);
#else
    dlclose(StrQueue_Module);
#endif
    StrQueue_Module = NULL;

    return (size_t)1;
}

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#ifdef __cplusplus
//DLL C++ wrapper for C functions
#if !defined(STRQUEUE_DLL_EXPORT) //No Lib include and No DLL construction

namespace Queue
{

class StrQueue {
	public:
		StrQueue()			{ self = StrQueue_New(); }
		~StrQueue()			{ StrQueue_Delete(self); }
		
		void  	Push(const char* str, size_t s) 	{ StrQueue_Push(self, str, s); };
		void	Pop(char* Buff, size_t s)			{ StrQueue_Pop(self, Buff, s); }
		
		size_t 	Len()		{ return StrQueue_Len(self); }
		int 	Empty()		{ return StrQueue_Empty(self); }
		
	private:
		void* self;
};

}

#endif//#if !defined(STRQUEUE_DLL_EXPORT)
#endif /*__cplusplus*/

#endif
