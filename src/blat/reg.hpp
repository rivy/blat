/*
    reg.hpp
*/
#ifndef __REG_HPP__
#define __REG_HPP__ 1

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


extern int  CreateRegEntry( COMMON_DATA & CommonData, int useHKCU );
extern void ShowRegHelp   ( COMMON_DATA & CommonData );
extern int  DeleteRegEntry( COMMON_DATA & CommonData, HKEY rootKeyLevel, LPTSTR pstrSubDirectory, Buf & pstrProfile );
extern int  GetRegEntry   ( COMMON_DATA & CommonData, Buf & pstrProfile );
extern void ListProfiles  ( COMMON_DATA & CommonData, LPTSTR pstrProfile );

#endif  // #ifndef __REG_HPP__
