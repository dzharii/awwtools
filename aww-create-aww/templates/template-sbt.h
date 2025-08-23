// helloworld.h - v0.1 - tiny stb-style single-header "Hello, World!" library
// Public Domain or MIT, your choice, see end of file.
//
// Minimal example of the stb single-header pattern.
// No allocations, no dependencies beyond the C standard library.
//
// DOCUMENTATION
//
//   Overview:
//     A trivial API that returns "Hello, World!" either as a const string
//     or by writing it into a caller-provided buffer.
//
//   Defines that control this header:
//     HELLOWORLD_STATIC
//         Define before including the *implementation* to make function
//         definitions 'static' so each including .c/.cpp gets its own copy.
//     HELLOWORLD_IMPLEMENTATION
//         Define in exactly one .c/.cpp before including this header to
//         compile the implementation.
//     HELLOWORLD_NO_STDIO
//         Omit stdio-based sample helpers if you add any that use stdio.
//
//   Build:
//     In one source file:
//         #define HELLOWORLD_IMPLEMENTATION
//         #include "helloworld.h"
//     In all other files:
//         #include "helloworld.h"
//
//   API:
//     HW123_PUBLIC_DECL const char* hw123_helloworld(void);
//       Returns a pointer to a constant, null-terminated "Hello, World!".
//
//     HW123_PUBLIC_DECL int hw123_helloworld_into(char* out, size_t out_cap);
//       Writes "Hello, World!" into 'out' with null-termination.
//       Returns the number of bytes written excluding the terminator.
//       Writes nothing and returns 0 if out is NULL or out_cap == 0.
//
// VERSION HISTORY
//   0.1  Initial release.
//
// CONTRIBUTORS
//   %_YOU_%.
//
// LICENSE
//   See end of file for license information.

#ifndef ___CAPITALIZED_FILE_NAME____H_INCLUDE
#define ___CAPITALIZED_FILE_NAME____H_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

// Public declaration macro: resolves to extern or extern "C" for C++.
// Users can override by defining HW123_PUBLIC_DECL before this include.
#ifndef HW123_PUBLIC_DECL
#  ifdef HELLOWORLD_STATIC
#    define HW123_PUBLIC_DECL static
#  else
#    ifdef __cplusplus
#      define HW123_PUBLIC_DECL extern "C"
#    else
#      define HW123_PUBLIC_DECL extern
#    endif
#  endif
#endif

// Optional configuration hooks for future allocation needs.
// Not used by this template, but provided to match stb guidance.
#ifndef HW123_NO_ALLOC
#  include <stddef.h> // size_t for allocator signatures if you use them later
#  ifndef HW123_MALLOC
#    include <stdlib.h>
#    define HW123_MALLOC(ctx, size)        ((void)(ctx), malloc(size))
#    define HW123_FREE(ctx, ptr)           ((void)(ctx), free(ptr))
#    define HW123_REALLOC(ctx, ptr, size)  ((void)(ctx), realloc(ptr, size))
#  endif
#endif

// Public API
HW123_PUBLIC_DECL const char* hw123_helloworld(void);
HW123_PUBLIC_DECL int         hw123_helloworld_into(char* out, size_t out_cap);

// Version query, handy for diagnostics.
HW123_PUBLIC_DECL const char* hw123_version(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ___CAPITALIZED_FILE_NAME____H_INCLUDE


/*=============================================================================
   IMPLEMENTATION
  =============================================================================*/
#ifdef ___CAPITALIZED_FILE_NAME____IMPLEMENTATION

// Definition macro: empty for global symbols, or 'static' when HELLOWORLD_STATIC.
#ifndef HW123_PUBLIC_DEF
#  ifdef HELLOWORLD_STATIC
#    define HW123_PUBLIC_DEF static
#  else
#    define HW123_PUBLIC_DEF
#  endif
#endif

// Private namespace prefix 'hw123__' for internal symbols.
static const char hw123__hello_literal[] = "Hello, World!";
static const char hw123__version_literal[] = "0.1";

HW123_PUBLIC_DEF const char* hw123_helloworld(void)
{
    return hw123__hello_literal;
}

HW123_PUBLIC_DEF int hw123_helloworld_into(char* out, size_t out_cap)
{
    if (!out || out_cap == 0) return 0;
    // Simple, dependency-free copy of the known literal.
    // Keep it malloc-free per stb guidance.
    const char* s = hw123__hello_literal;
    size_t i = 0;
    // Reserve one byte for terminator.
    while (s[i] != '\0' && i + 1 < out_cap) {
        out[i] = s[i];
        ++i;
    }
    out[i < out_cap ? i : out_cap - 1] = '\0';
    return (int)i;
}

HW123_PUBLIC_DEF const char* hw123_version(void)
{
    return hw123__version_literal;
}

#endif // ___CAPITALIZED_FILE_NAME____IMPLEMENTATION


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