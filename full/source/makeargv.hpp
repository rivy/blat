/*
    makeargv.hpp
*/
#ifndef __MAKEARGV_HPP__
#define __MAKEARGV_HPP__ 1

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

/*
 * Parse the arguments read in from an options file.  Allow the semicolon (';') to
 * be a comment character.  This can be changed by  the user whenn using a new option
 * called -comment followed by the desired character.  For example, -comment #.
 *
 */

extern size_t make_argv( _TCHAR commentChar,
                         LPTSTR arglist,                /* argument list                     */
                         LPTSTR*static_argv,            /* pointer to argv to use            */
                         size_t max_static_entries,     /* maximum number of entries allowed */
                         size_t starting_entry,         /* entry in argv to begin placing    */
                         int    from_dll );             /* blat called as .dll               */

#endif  // #ifndef __MAKEARGV_HPP__
