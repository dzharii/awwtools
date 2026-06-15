// ___FILE_NAME___.h - v0.1 - stb-style single-header library
// Public Domain or MIT, your choice, see end of file.
//
// DOCUMENTATION
//
//   Overview:
//     A trivial API that returns a constant "Hello, World!" string.
//
//     TODO: Replace this overview with the actual library description.
//
//   Configuration:
//     ___CAPITALIZED_FILE_NAME____IMPLEMENTATION
//         Define in one .c file before including this header to compile the
//         implementation.
//
//     ___CAPITALIZED_FILE_NAME____STATIC
//         Define before including the implementation to make function
//         definitions static. Use this when each .c file should get its own
//         private copy.
//
//     ___CAPITALIZED_FILE_NAME____STRIP_PREFIX
//         Define before the first include to expose shorter convenience names.
//         For example, hw123_helloworld can also be called as helloworld.
//
//         Without strip prefix:
//             hw123_helloworld();
//             hw123_version();
//
//         With strip prefix:
//             helloworld();
//             version();
//
//         The compiled symbols keep their prefixed names. The shorter names are
//         preprocessor aliases only.
//
//   Build:
//     In one .c file:
//         #define ___CAPITALIZED_FILE_NAME____IMPLEMENTATION
//         #include "___FILE_NAME___.h"
//
//     In all other .c files:
//         #include "___FILE_NAME___.h"
//
//   API:
//     ___CAPITALIZED_FILE_NAME____PUBLIC_DECL const char* hw123_helloworld(void);
//       Returns a pointer to a constant, null-terminated "Hello, World!".
//
//     ___CAPITALIZED_FILE_NAME____PUBLIC_DECL const char* hw123_version(void);
//       Returns a pointer to a constant, null-terminated version string.
//
// VERSION HISTORY
//   0.1  Initial release.
//
// CONTRIBUTORS
//   %_YOU_%.
//
// LICENSE
//   See end of file for license information.
//
//   TODO: Review and edit the license text for your project. This template is
//   public domain; you may freely modify any text in this file.

#ifndef ___CAPITALIZED_FILE_NAME____H_INCLUDE
#define ___CAPITALIZED_FILE_NAME____H_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

// Public declaration macro: resolves to extern or extern "C" for C++.
// Users can override by defining HW123_PUBLIC_DECL before this include.
#ifndef ___CAPITALIZED_FILE_NAME____PUBLIC_DECL
#  ifdef ___CAPITALIZED_FILE_NAME____STATIC
#    define ___CAPITALIZED_FILE_NAME____PUBLIC_DECL static
#  else
#    ifdef __cplusplus
#      define ___CAPITALIZED_FILE_NAME____PUBLIC_DECL extern "C"
#    else
#      define ___CAPITALIZED_FILE_NAME____PUBLIC_DECL extern
#    endif
#  endif
#endif

// Public API
___CAPITALIZED_FILE_NAME____PUBLIC_DECL const char* hw123_helloworld(void);

// Version query, handy for diagnostics.
___CAPITALIZED_FILE_NAME____PUBLIC_DECL const char* hw123_version(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ___CAPITALIZED_FILE_NAME____H_INCLUDE

/*=============================================================================
   IMPLEMENTATION
  =============================================================================*/
#ifdef ___CAPITALIZED_FILE_NAME____IMPLEMENTATION

// Definition macro: empty for global symbols, or 'static' when HELLOWORLD_STATIC.
#ifndef ___CAPITALIZED_FILE_NAME____PUBLIC_DEF
#  ifdef ___CAPITALIZED_FILE_NAME____STATIC
#    define ___CAPITALIZED_FILE_NAME____PUBLIC_DEF static
#  else
#    define ___CAPITALIZED_FILE_NAME____PUBLIC_DEF
#  endif
#endif

// Private namespace prefix 'hw123__' for internal symbols.
static const char hw123__hello_literal[] = "Hello, World!";
static const char hw123__version_literal[] = "0.1";

___CAPITALIZED_FILE_NAME____PUBLIC_DEF const char* hw123_helloworld(void)
{
    return hw123__hello_literal;
}

___CAPITALIZED_FILE_NAME____PUBLIC_DEF const char* hw123_version(void)
{
    return hw123__version_literal;
}

#endif // ___CAPITALIZED_FILE_NAME____IMPLEMENTATION

#ifndef ___CAPITALIZED_FILE_NAME____PREFIX_GUARD_
#define ___CAPITALIZED_FILE_NAME____PREFIX_GUARD_
    // NOTE: The name stripping should be part of the header so it's not accidentally included
    // several times. At the same time, it should be at the end of the file so to not create any
    // potential conflicts in the ___CAPITALIZED_FILE_NAME____IMPLEMENTATION. The header obviously cannot be at the end
    // of the file because ___CAPITALIZED_FILE_NAME____IMPLEMENTATION needs the forward declarations from there. So the
    // solution is to split the header into two parts where the name stripping part is at the
    // end of the file after the ___CAPITALIZED_FILE_NAME____IMPLEMENTATION.
    #ifdef ___CAPITALIZED_FILE_NAME____STRIP_PREFIX
        #define hw123_helloworld helloworld
        #define hw123_version    version
    #endif // ___CAPITALIZED_FILE_NAME____STRIP_PREFIX
#endif // ___CAPITALIZED_FILE_NAME____PREFIX_GUARD_


/*
------------------------------------------------------------------------------
Template attribution

This generated header follows the stb-style single-header pattern documented by
Sean Barrett. The optional prefix-stripping organization was inspired by nob.h
by Tsoding / Alexey Kutepov.

This comment is attribution for the template only. You may remove or modify it
freely in generated files.
------------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2025 %_YOU_%
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/