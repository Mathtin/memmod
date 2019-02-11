#ifndef StrQueueDll_StaticH
#define StrQueueDll_StaticH

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

#include <stddef.h>
#define MAXLINELENGTH 256

STRQUEUE_EXP void* 		StrQueue_New(void);
STRQUEUE_EXP void 		StrQueue_Delete(void*);
	
STRQUEUE_EXP void 		StrQueue_Push(void*, const char*, size_t);
STRQUEUE_EXP void 		StrQueue_Pop(void*, char*, size_t);
	
STRQUEUE_EXP size_t     StrQueue_Len(void*);
STRQUEUE_EXP int 		StrQueue_Empty(void*);

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
