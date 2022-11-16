/*
    parsing.hpp

    This file is for parsing comma delimited strings into individual strings.
    This provides a central source for a common function used for email addresses,
    and attachment names.
*/
#ifndef __PARSING_HPP__
#define __PARSING_HPP__ 1

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

extern void parseCommaDelimitString ( COMMON_DATA & CommonData, LPTSTR source, Buf & parsed_strings, int pathNames );

#endif  // #ifndef __PARSING_HPP__
