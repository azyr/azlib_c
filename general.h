#ifndef AZLIB_GENERAL_H
#define AZLIB_GENERAL_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#if (!defined (WIN32))
#   include <sys/time.h>
#endif

#if (defined (WIN32))
#   include <windows.h>
#endif

#include "error_functions.h"  /* Declares our error-handling functions */

/* Unfortunately some UNIX implementations define FALSE and TRUE -
   here we'll undefine them */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

//  Sleep for a number of milliseconds
void sleep_ms (int msecs);

/* Shortcuts to forking */
int az_fork(void (*child_fun)());
int az_fork2(void (*child_fun)(void*), void *arguments);
int az_fork_slp(void (*child_fun)(void*), int msecs, void *arguments);


//  On some version of Windows, POSIX subsystem is not installed by default.
//  So define srandom and random ourself.
#if (defined (WIN32))
#   define srandom srand
#   define random rand
#endif

//  Provide random number from 0..(num-1)
#define randint(num)  (int) ((float) (num) * random () / (RAND_MAX + 1.0))

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

/* Some systems don't define 'socklen_t' */

#if defined(__sgi)
typedef int socklen_t;
#endif

#if defined(__sun)
#include <sys/file.h>           /* Has definition of FASYNC */
#endif

#if ! defined(O_ASYNC) && defined(FASYNC)
/* Some systems define FASYNC instead of O_ASYNC */
#define O_ASYNC FASYNC
#endif

#if defined(MAP_ANON) && ! defined(MAP_ANONYMOUS)
/* BSD derivatives usually have MAP_ANON, not MAP_ANONYMOUS */
#define MAP_ANONYMOUS MAP_ANON
#endif

#if ! defined(O_SYNC) && defined(O_FSYNC)
/* Some implementations have O_FSYNC instead of O_SYNC */
#define O_SYNC O_FSYNC
#endif

#if defined(__FreeBSD__)
/* FreeBSD uses these alternate names for fields in the sigval structure */
#define sival_int sigval_int
#define sival_ptr sigval_ptr
#endif

/* Error handling macros.
 * Inspiration from: http://stackoverflow.com/questions/6932401/elegant-error-checking 
 */
#define check(x) do { \
  int retval = (x); \
  if (retval != 0) { \
    fprintf(stderr, "Runtime error: %s returned %d at %s:%d\n", #x, retval, __FILE__, __LINE__); \
    errExit(""); \
  } \
} while (0)

#define check_(x) do { \
  int retval = (x); \
  if (retval != 0) { \
    fprintf(stderr, "Runtime error: %s returned %d at %s:%d\n", #x, retval, __FILE__, __LINE__); \
    err_exit(""); \
  } \
} while (0)

//  Return current system clock as milliseconds
int64_t clock_ms();

// Checks that there is enough space in the heap.
// Initializes allocated block to zeroes.
static inline void *
_checked_calloc(size_t size, const char *file, unsigned line)
{
    assert(size > 0);
    void *mem = calloc(1, size);
    if (mem == 0)
    {
        fprintf (stderr, "FATAL ERROR at %s:%u\n", file, line);
        fprintf (stderr, "OUT OF MEMORY (calloc returned NULL)\n");
        fflush (stderr);
        abort ();
    }
    return mem;
}
#define azmalloc(size) _checked_calloc((size), __FILE__, __LINE__)

#endif // AZLIB_GENERAL_H
