/*
    attach.hpp
*/

#ifndef __ATTACH_HPP__
#define __ATTACH_HPP__ 1

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

extern void getAttachmentInfo( COMMON_DATA & CommonData, int attachNbr, LPTSTR & attachName, DWORD & attachSize, int & attachType, LPTSTR & attachDescription );
extern void releaseAttachmentInfo ( COMMON_DATA & CommonData );
extern int  collectAttachmentInfo ( COMMON_DATA & CommonData, DWORD & totalsize, size_t msgBodySize );
extern void getMaxMsgSize ( COMMON_DATA & CommonData, int buildSMTP, DWORD &length );
extern int  add_one_attachment ( COMMON_DATA & CommonData, Buf &messageBuffer, int buildSMTP, LPTSTR attachment_boundary,
                                 DWORD startOffset, DWORD &length,
                                 int part, int totalparts, int attachNbr, int * prevAttachType );
extern int  add_attachments ( COMMON_DATA & CommonData, Buf &messageBuffer, int buildSMTP, LPTSTR attachment_boundary, int nbrOfAttachments );

#endif  // #ifndef __ATTACH_HPP__
