/*
    mime.hpp
*/
#ifndef __MIME_HPP__
#define __MIME_HPP__ 1

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


extern int  CheckIfNeedQuotedPrintable(COMMON_DATA & CommonData, LPTSTR pszStr, int inHeader);
extern int  GetLengthQuotedPrintable(COMMON_DATA & CommonData, LPTSTR pszStr, int inHeader);
extern void ConvertToQuotedPrintable(COMMON_DATA & CommonData, Buf & source, Buf & out, int inHeader);

#endif  // #ifndef __MIME_HPP__
