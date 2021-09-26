/*
    base64.hpp
*/

#ifndef __BASE64_HPP__
#define __BASE64_HPP__ 1

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

extern LPTSTR base64table;

extern int  base64_decode( Buf & in, Buf & out );
extern int  base64_decode( _TUCHAR * in, LPTSTR out );
extern void base64_encode( Buf & source, Buf & out, int inclCrLf, int inclPad );
extern void base64_encode( _TUCHAR * in, size_t length, LPTSTR out, int inclCrLf );

#endif  // #ifndef __BASE64_HPP__
