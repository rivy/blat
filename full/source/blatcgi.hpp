/*
    blatcgi.hpp
 */
#ifndef __BLATCGI_HPP__
#define __BLATCGI_HPP__ 1

// CGI Support by Gilles Vollant <info@winimage.com>, February 1999
// Added in Blat Version 1.8.3

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

extern BOOL getFileSize(LPCTSTR lpFn, DWORD &dwSize);
extern void GetEnv(LPTSTR lpszEnvVar, Buf &buf);
extern void ReadPostData(Buf &buf);
extern DWORD SearchNextPos(LPCTSTR lpszParamCgi, BOOL fSearchEqual);
extern DWORD SearchNextPercent(LPCTSTR lpszParamCgi);
extern void SearchVar(Buf &lpszParamCgi,LPCTSTR lpszVarName,BOOL fMimeDecode, Buf &ret);
extern BOOL BuildMessageAndCmdLine(Buf &lpszParamCgi, LPCTSTR lpszPathTranslated, Buf &lpszCmdBlat, Buf &lpszMessage);
extern DWORD WINAPI ReadCommandLine(LPTSTR szParcLine, int & argc, LPTSTR* &argv);
extern BOOL DoCgiWork(int & argc, LPTSTR*  &argv, Buf &lpszMessage,
                      Buf & lpszCgiSuccessUrl, Buf &lpszCgiFailureUrl,
                      Buf & lpszFirstReceivedData, Buf &lpszOtherHeader);

#endif  // #ifndef __BLATCGI_HPP__
