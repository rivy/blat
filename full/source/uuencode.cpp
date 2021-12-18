/*
    uuencode.cpp
*/

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
#include "blatext.hpp"
#include "bldhdrs.hpp"
#include "common_data.h"
#include "macros.h"
#include "uuencode.hpp"

#if BLAT_LITE
#else

static unsigned int encodedLineLength;

  #define UU_Mask(Ch) (_TCHAR) ((((Ch) - 1) & 0x3F) + 0x21)

void douuencode(COMMON_DATA & CommonData, Buf & source, Buf & out, LPTSTR filename, int part, int lastpart)
{
    FUNCTION_ENTRY();
    DWORD         filesize;
    DWORD         tempLength;
    _TCHAR        tmpstr[88];
    LPTSTR        p;
    unsigned int  bytes_out;
    unsigned long bitStream;
    Buf           shortNameBuf;

    p = source.Get();
    if ( !p ) {
        FUNCTION_EXIT();
        return;
    }

    fixupFileName( CommonData, filename, shortNameBuf, 0, FALSE );
    if ( part < 2 ) {
        out.Add( __T("begin 644 ") );
        out.Add( shortNameBuf );
        out.Add( __T("\r\n") );
    }

    filesize = (DWORD)source.Length();
    tempLength = (((filesize *8)+5)/6);
    tempLength += (((tempLength + (CommonData.uuencodeBytesLine * 4/3) - 1) / (CommonData.uuencodeBytesLine * 4/3)) * 3) + 1;
    out.Alloc( out.Length() + tempLength );

    // Ensure the data is padded with NULL to work the for() loop.
    p[filesize] = __T('\0');

    encodedLineLength = (CommonData.uuencodeBytesLine * 4) / 3;

    for ( bytes_out = 0; filesize > 2; filesize -= 3, p += 3 ) {
        if ( bytes_out > encodedLineLength ) {
            tmpstr[ bytes_out ] = __T('\0');
            out.Add( tmpstr );
            out.Add( __T("\r\n") );
            bytes_out = 0;
        }

        if ( bytes_out == 0 )
            tmpstr[ bytes_out++ ] = UU_Mask((filesize > CommonData.uuencodeBytesLine) ? CommonData.uuencodeBytesLine : filesize);

        bitStream = (unsigned long)(p[0] << 16) | (unsigned long)(p[1] << 8) | p[2];
        tmpstr[ bytes_out++ ] = UU_Mask( bitStream >> 18 );
        tmpstr[ bytes_out++ ] = UU_Mask( bitStream >> 12 );
        tmpstr[ bytes_out++ ] = UU_Mask( bitStream >>  6 );
        tmpstr[ bytes_out++ ] = UU_Mask( bitStream       );
    }

    /* If filesize == 0, then we're done.
     * If filesize == 1 at this point, then the p[0] is the last byte of the file, and p[1] is a binary zero.
     * If filesize == 2 at this point, then p[0] and p[1] are the last bytes, while p[2] is a binary zero.
     * In all cases, p[2] is not needed.
     */
    if ( filesize ) {
        if ( bytes_out > encodedLineLength ) {
            tmpstr[ bytes_out ] = __T('\0');
            out.Add( tmpstr );
            out.Add( __T("\r\n") );
            bytes_out = 0;
        }

        if ( bytes_out == 0 )
            tmpstr[ bytes_out++ ] = UU_Mask( filesize );

        bitStream = (unsigned long)((p[0] << 8) | p[1]);
        tmpstr[ bytes_out++ ] = UU_Mask( bitStream >> 10 );
        tmpstr[ bytes_out++ ] = UU_Mask( bitStream >>  4 );

        if ( filesize == 2 )
            tmpstr[ bytes_out++ ] = UU_Mask( bitStream << 2 );
    }

    if ( bytes_out ) {
        tmpstr[ bytes_out ] = __T('\0');
        out.Add( tmpstr );
        out.Add( __T("\r\n") );
    }

    if ( part == lastpart )
        out.Add( __T("\x60\r\nend\r\n") );   // x60 represents a zero length record, marking the end.

    FUNCTION_EXIT();
}


  #if INCLUDE_XXENCODE

  #define XX_Mask(Ch) (char) xxenctable[ (Ch) & 0x3F ]

static const _TCHAR xxenctable[]  = __T("+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

void xxencode(Buf & source, Buf & out, LPTSTR filename, int part, int lastpart )
{
    DWORD           filesize;
    _TCHAR          tmpstr[88];
    _TUCHAR       * p;
    unsigned int    bytes_out;
    unsigned long   bitStream;
    Buf             shortNameBuf;

    p = (_TUCHAR *) source.Get();
    if ( !p ) {
        return;
    }

    fixupFileName( CommonData, filename, shortNameBuf, 0, FALSE );
    if ( part < 2 ) {
        out.Add( __T("begin 700 ") );
        out.Add( shortNameBuf );
        out.Add( __T("\r\n") );
    }

    filesize = source.Length();
    out.Alloc( out.Length() + (filesize*2) + 4 );

    // Ensure the data is padded with NULL to work the for() loop.
    p[filesize] = __T('\0');

    encodedLineLength = (CommonData.uuencodeBytesLine * 4) / 3;

    for ( bytes_out = 0; filesize > 2; filesize -= 3, p += 3 ) {
        if ( bytes_out > encodedLineLength ) {
            tmpstr[ bytes_out ] = __T('\0');
            out.Add( tmpstr );
            out.Add( __T("\r\n") );
            bytes_out = 0;
        }

        if ( bytes_out == 0 )
            tmpstr[ bytes_out++ ] = XX_Mask((filesize > CommonData.uuencodeBytesLine) ? CommonData.uuencodeBytesLine : filesize);

        bitStream = (p[0] << 16) | (p[1] << 8) | p[2];
        tmpstr[ bytes_out++ ] = XX_Mask( bitStream >> 18 );
        tmpstr[ bytes_out++ ] = XX_Mask( bitStream >> 12 );
        tmpstr[ bytes_out++ ] = XX_Mask( bitStream >>  6 );
        tmpstr[ bytes_out++ ] = XX_Mask( bitStream       );
    }

    /* If filesize == 0, then we're done.
     * If filesize == 1 at this point, then p[0] is the last byte of the file, and p[1] is a binary zero.
     * If filesize == 2 at this point, then p[0] and p[1] are the last bytes, while p[2] is a binary zero.
     * In all cases, p[2] is not needed.
     */
    if ( filesize ) {
        if ( bytes_out > encodedLineLength ) {
            tmpstr[ bytes_out ] = __T('\0');
            out.Add( tmpstr );
            out.Add( __T("\r\n") );
            bytes_out = 0;
        }

        if ( bytes_out == 0 )
            tmpstr[ bytes_out++ ] = XX_Mask( filesize );

        bitStream = (p[0] << 8) | p[1];
        tmpstr[ bytes_out++ ] = XX_Mask( bitStream >> 10 );
        tmpstr[ bytes_out++ ] = XX_Mask( bitStream >>  4 );

        if ( filesize == 2 )
            tmpstr[ bytes_out++ ] = XX_Mask( bitStream << 2 );
    }

    if ( bytes_out ) {
        tmpstr[ bytes_out ] = __T('\0');
        out.Add( tmpstr );
        out.Add( __T("\r\n") );
    }

    if ( part == lastpart )
        out.Add( __T("end\r\n") );
}
  #endif
#endif
