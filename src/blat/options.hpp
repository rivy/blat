/*
    options.hpp
*/
#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__ 1

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

extern _BLATOPTIONS blatOptionsList[];

extern void printTitleLine( COMMON_DATA & CommonData, int quiet );
extern void print_usage_line( COMMON_DATA & CommonData, LPTSTR pUsageLine );
extern int  printUsage( COMMON_DATA & CommonData, int optionPtr );
extern int  processOptions( COMMON_DATA & CommonData, int argc, LPTSTR * argv, int startargv, int preprocessing );

#endif  // #ifndef __OPTIONS_HPP__
