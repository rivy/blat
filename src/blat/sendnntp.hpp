/*
    sendNNTP.hpp
*/

#ifndef __SENDNNTP_HPP__
#define __SENDNNTP_HPP__ 1

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

#if INCLUDE_NNTP

#include "winfile.h"
#include "common_data.h"
#include "gensock.h"

extern LPTSTR defaultNNTPPort;

extern int send_news( COMMON_DATA & CommonData, size_t msgBodySize,
                      Buf &lpszFirstReceivedData, Buf &lpszOtherHeader,
                      LPTSTR attachment_boundary, LPTSTR multipartID,
                      int nbrOfAttachments, DWORD totalsize );

#endif  // #if INCLUDE_NNTP

#endif  // #ifndef __SENDNNTP_HPP__
