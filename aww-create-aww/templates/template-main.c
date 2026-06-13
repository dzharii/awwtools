/*
  Description: Build and run instructions for a C99 program.

  Compile with:
  gcc:     gcc -std=c99 -Wall -Wextra -O2 -o ___FILE_NAME___ ___FILE_NAME___.c
  clang:   clang -std=c99 -Wall -Wextra -O2 -o ___FILE_NAME___ ___FILE_NAME___.c
  cc:      cc -std=c99 -Wall -Wextra -O2 -o ___FILE_NAME___ ___FILE_NAME___.c
  tcc run: tcc -run ___FILE_NAME___.c
  tcc 
  compile 
  only:    tcc -c ___FILE_NAME___.c
  vc:      cl /TC /W4 /O2 ___FILE_NAME___.c
*/

#include <stdio.h>
// clearerr      fclose        feof          ferror
// fflush        fgetc         fgetpos       fgets
// fopen         fprintf       fputc         fputs
// fread         freopen       fscanf        fseek
// fsetpos       ftell         fwrite        getc
// getchar       gets          perror        printf
// putc          putchar       puts          remove
// rename        rewind        scanf         setbuf
// setvbuf       snprintf      sprintf       sscanf
// tmpfile       tmpnam        ungetc        vfprintf
// vfscanf       vprintf       vscanf        vsnprintf
// vsprintf      vsscanf

#include <stdlib.h>
// _Exit         abort         abs           atexit
// atof          atoi          atol          atoll
// bsearch       calloc        div           exit
// free          getenv        labs          llabs
// ldiv          lldiv         malloc        mblen
// mbstowcs      mbtowc        qsort         rand
// realloc       srand         strtod        strtof
// strtold       strtol        strtoll       strtoul
// strtoull      system        wcstombs      wctomb

#include <stdint.h>
// Exact-width integer types:
//   int8_t  uint8_t   int16_t  uint16_t
//   int32_t uint32_t  int64_t  uint64_t
// Minimum width and fastest types:
//   int_least8_t  ... int_least64_t
//   uint_least8_t ... uint_least64_t
//   int_fast8_t   ... int_fast64_t
//   uint_fast8_t  ... uint_fast64_t
// Pointer-sized and max-width:
//   intptr_t  uintptr_t  intmax_t  uintmax_t
// Useful limit macros:
//   INT8_MIN  INT8_MAX   UINT8_MAX
//   INT16_MIN INT16_MAX  UINT16_MAX
//   INT32_MIN INT32_MAX  UINT32_MAX
//   INT64_MIN INT64_MAX  UINT64_MAX
//   INTPTR_MIN INTPTR_MAX  UINTPTR_MAX
//   INTMAX_MIN INTMAX_MAX  UINTMAX_MAX
//   PTRDIFF_MIN PTRDIFF_MAX
//   SIZE_MAX
//   SIG_ATOMIC_MIN SIG_ATOMIC_MAX

#include <string.h>
// memchr        memcmp        memcpy        memmove
// memset        strcat        strchr        strcmp
// strcoll       strcpy        strcspn       strerror
// strlen        strncat       strncmp       strncpy
// strpbrk       strrchr       strspn        strstr
// strtok        strxfrm

#include <stdbool.h>
// Defines:
//   bool  true  false
// Macro:
//   __bool_true_false_are_defined

#include <stddef.h>
// Types and macros:
//   size_t  ptrdiff_t  wchar_t
//   NULL    offsetof(type, member)

#include <stdarg.h>
// Variadic arguments support:
//   va_list  va_start(ap, last)  va_arg(ap, type)
//   va_end(ap)                   va_copy(dst, src)

#include <errno.h>
// Error reporting via errno and codes like:
//   EDOM  EILSEQ  ERANGE  (plus implementation-defined values)
// Note: errno is thread-local in conforming implementations.

#include <limits.h>
// Integer limits and characteristics:
//   CHAR_BIT  MB_LEN_MAX
//   SCHAR_MIN SCHAR_MAX  UCHAR_MAX
//   CHAR_MIN  CHAR_MAX
//   SHRT_MIN  SHRT_MAX   USHRT_MAX
//   INT_MIN   INT_MAX    UINT_MAX
//   LONG_MIN  LONG_MAX   ULONG_MAX
//   LLONG_MIN LLONG_MAX  ULLONG_MAX

#include <ctype.h>
// isalnum       isalpha       isblank       iscntrl
// isdigit       isgraph       islower       isprint
// ispunct       isspace       isupper       isxdigit
// tolower       toupper

#include <time.h>
// asctime       clock         ctime         difftime
// gmtime        localtime     mktime        strftime
// time

int main(int argc, char* argv[]) {
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("  argv[%2d]: %s\n", i, argv[i]);
    }
    return 0;
}