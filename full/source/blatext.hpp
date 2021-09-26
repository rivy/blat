/*
    blatext.hpp
*/
#ifndef __BLATEXT_HPP__
#define __BLATEXT_HPP__ 1

#include "declarations.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#if defined(_MSC_VER) && (_MSC_VER < 1300)
// For VC6, disable warnings from various standard Windows headers
// NOTE: #pragma warning(push) ... #pragma warning(pop) is broken/unusable for MSVC 6 (re-enables multiple other warnings)
#pragma warning(disable : 4068) // DISABLE: unknown pragma warning
#pragma warning(disable : 4035) // DISABLE: no return value warning
#endif
#include <windows.h>
#if defined(_MSC_VER) && (_MSC_VER < 1300)
#pragma warning(default : 4068) // RESET: unknown pragma warning
#pragma warning(default : 4035) // RESET: no return value warning
#endif

#include <tchar.h>

#include "blat.h"
#include "common_data.h"

extern _TCHAR  blatVersion[];
extern _TCHAR  blatVersionSuf[];
extern _TCHAR  blatBuildDate[];
extern _TCHAR  blatBuildTime[];

extern char    blatBuildDateA[];
extern char    blatBuildTimeA[];

extern LPTSTR  days[];


extern void printMsg(COMMON_DATA & CommonData, LPTSTR p, ... );

#ifdef BLATDLL_EXPORTS // this is blat.dll, not blat.exe
#if defined(__cplusplus)
extern "C" {
#endif
extern void(*pMyPrintDLL)(LPTSTR);
#if defined(__cplusplus)
}
#endif
#else
#define pMyPrintDLL _tprintf
#endif

#ifdef __cplusplus
extern "C" int _tmain( int argc, LPTSTR *argv, LPTSTR *envp );
#endif

#endif  // #ifndef __BLATEXT_HPP__
