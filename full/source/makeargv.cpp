/*
    makeargv.cpp
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

#include <ctype.h>
#include <malloc.h>
#include <tchar.h>

#include "makeargv.hpp"

/*
 * Parse the arguments read in from an options file.  Allow the semicolon (';') to
 * be a comment character.  This can be changed by the user when using a new option
 * called -comment followed by the desired character.  For example, -comment #.
 *
 */

size_t make_argv( _TCHAR commentChar,
                  LPTSTR arglist,                /* argument list                     */
                  LPTSTR*static_argv,            /* pointer to argv to use            */
                  size_t max_static_entries,     /* maximum number of entries allowed */
                  size_t starting_entry,         /* entry in argv to begin placing    */
                  int    from_dll )              /* blat called as .dll               */
{
    LPTSTR  startArgs;
    LPTSTR* argv;
    LPTSTR  nextarg;
    size_t  argc;
    size_t  x, y, z;

    startArgs = arglist;
    argv = static_argv;
    for ( argc = starting_entry; argc < max_static_entries; argc++) {
        int foundQuote;

        while( (*startArgs == __T(' ' )) ||
               (*startArgs == __T('\t')) ||
               (*startArgs == __T('\r')) ||
               (*startArgs == __T('\n')) )
            startArgs++;

        if ( !*startArgs )
            break;

        foundQuote = FALSE;

        /* Parse this entry to determine string length */
        for ( x = 0; startArgs[ x ]; x++ ) {
            if ( startArgs[ x ] == __T('\\') ) {
                if ( startArgs[ x+1 ] == __T('"') ) {
                    x++;
                    continue;
                }

                /*
                 * If we're parsing the command line for blat.dll, then we should
                 * not make any special accomodations for the backslash.
                 */
                if ( !from_dll ) {
                    if ( !startArgs[ x+1 ] ) {
                        x++;
                        break;
                    }

                    if ( foundQuote )
                        x++;

                    continue;
                }
            }

            if ( startArgs[ x ] == __T('"') ) {
                foundQuote = (foundQuote != TRUE );
                continue;
            }

            if ( (startArgs[ x ] == __T(' ') ) ||
                 (startArgs[ x ] == __T('\t')) ) {
                if ( foundQuote )
                    continue;

                break;
            }

            if ( startArgs[ x ] == commentChar ) {
                LPTSTR pChar;

                if ( foundQuote )
                    continue;

                // Terminate the string at this comment character.
                pChar = &startArgs[ x ];
                for ( ; ; ) {
                    pChar++;
                    if ( (*pChar == __T('\0')) ||
                         (*pChar == __T('\r')) ||
                         (*pChar == __T('\n')) )
                        break;
                }
                _tcscpy( &startArgs[ x ], pChar );
                break;
            }

            if ( startArgs[ x ] == __T('\r') ) {
                if ( foundQuote )
                    continue;

                break;
            }

            if ( startArgs[ x ] == __T('\n') ) {
                if ( foundQuote )
                    continue;

                break;
            }
        }

        if ( (startArgs[ 0 ] == __T('\0')) ||
             (startArgs[ 0 ] == __T('\r')) ||
             (startArgs[ 0 ] == __T('\n')) ) {
            argc--;
            continue;
        }
        /* Found end of this argument. */
        nextarg = (LPTSTR)malloc( (x + 1)*sizeof(_TCHAR) );
        if ( !nextarg )
            break;

        foundQuote = FALSE;

        for ( z = y = 0; z < x; z++ ) {
            nextarg[ y ] = startArgs[ z ];

            if ( startArgs[ z ] == __T('\\') ) {
                if ( startArgs[ z+1 ] == __T('"') ) {
                    nextarg[ y++ ] = startArgs[ ++z ];
                    continue;
                }

                if ( !from_dll ) {
                    if ( !startArgs[ z+1 ] ) {
                        y++;
                        break;
                    }

                    if ( foundQuote ) {
                        switch ( startArgs[ ++z ] ) {
                            default:
                                nextarg[ y ] = startArgs[ z ];
                                break;

                            case __T('a'):
                                nextarg[ y ] = __T('\a');
                                break;

                            case __T('b'):
                                nextarg[ y ] = __T('\b');
                                break;

                            case __T('f'):
                                nextarg[ y ] = __T('\f');
                                break;

                            case __T('n'):
                                nextarg[ y ] = __T('\n');
                                break;

                            case __T('r'):
                                nextarg[ y ] = __T('\r');
                                break;

                            case __T('t'):
                                nextarg[ y ] = __T('\t');
                                break;

                            case __T('v'):
                                nextarg[ y ] = __T('\v');
                                break;

                            case __T('x'):       /* hex conversion */
                            {
                                int hexValue = 0;
                                int c;

                                for ( z++; ; ) {
                                    if ( hexValue > (255/16) )
                                        break;

                                    c = _totlower(startArgs[ z ]);
                                    if ( (c < __T('0')) || (c > __T('f')) )
                                        break;

                                    if ( (c > __T('9')) && (c < __T('a')) )
                                        break;

                                    if ( c <= __T('9') )
                                        c -= __T('0');
                                    else
                                        c -= __T('a') - 10;

                                    hexValue = hexValue * 16 + c;
                                    z++;
                                }

                                z--;
                                nextarg[ y ] = (_TCHAR)hexValue;
                                break;
                            }

                            /* octal conversion */
                            case __T('0'):
                            case __T('1'):
                            case __T('2'):
                            case __T('3'):
                            case __T('4'):
                            case __T('5'):
                            case __T('6'):
                            case __T('7'):
                            {
                                unsigned octalValue = 0;

                                for ( ; ; ) {
                                    if ( octalValue > (((256*sizeof(_TCHAR))-1)/8) )
                                        break;

                                    if ( (startArgs[ z ] < __T('0')) || (startArgs[ z ] > __T('7')) )
                                        break;

                                    octalValue = octalValue * 8 + (startArgs[ z++ ] - __T('0'));
                                }

                                z--;
                                nextarg[ y ] = (_TCHAR)octalValue;
                                break;
                            }
                        }

                        y++;
                        continue;
                    }
                }
            }

            if ( startArgs[ z ] == __T('"') ) {
                foundQuote = (foundQuote != TRUE );
                continue;
            }

            if ( (startArgs[ z ] == __T(' ') ) ||
                 (startArgs[ z ] == __T('\t')) ) {
                if ( foundQuote ) {
                    y++;
                    continue;
                }

                break;
            }

            if ( startArgs[ z ] == commentChar ) {
                if ( foundQuote ) {
                    y++;
                    continue;
                }

                break;
            }

            if ( startArgs[ z ] == __T('\r') ) {
                if ( foundQuote ) {
                    y++;
                    continue;
                }

                break;
            }

            if ( startArgs[ z ] == __T('\n') ) {
                if ( foundQuote ) {
                    y++;
                    continue;
                }

                break;
            }

            y++;
        }

        nextarg[ y ] = __T('\0');
        argv[ argc ] = nextarg;
        startArgs += x;
    }

    return argc;
}
