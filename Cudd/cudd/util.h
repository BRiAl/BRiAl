/* header-only replacement for Cudd's util/ */

#ifdef CUDD_ORIG_INCLUSION
#include "../util/util.h"
#define polybori_cuddy_util_h_
#endif

#ifndef polybori_cuddy_util_h_
#define polybori_cuddy_util_h_

#ifdef __cplusplus

#include <ctime>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>

extern "C" {

#else

#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#define ALLOC(type, num)	\
    ((type *) MMalloc((long) sizeof(type) * (long) (num)))
#define REALLOC(type, obj, num)	\
    ((type *) MMrealloc((char *) (obj), (long) sizeof(type) * (long) (num)))
#define FREE(obj)		\
    ((obj) ? (free((char *) (obj)), (obj) = 0) : 0)

extern void (*MMoutOfMemory) (long);

static inline long util_cpu_time () {
  clock_t saved_time = clock();
  return (((long)saved_time)*1000) / CLOCKS_PER_SEC;
}

static inline long
getSoftDataLimit(void) {
  const long RLIMIT_DATA_DEFAULT = 67108864;	/* assume 64MB by default */
  return RLIMIT_DATA_DEFAULT;
} /* end of getSoftDataLimit */


/*
 *  These are interface routines to be placed between a program and the
 *  system memory allocator.  
 *
 *  It forces well-defined semantics for several 'borderline' cases:
 *
 *	MMalloc() of a 0 size object is guaranteed to return something
 *	    which is not 0, and can safely be freed (but not dereferenced)
 *	MMfree() accepts (silently) an 0 pointer
 *	MMrealloc of a 0 pointer is allowed, and is equiv. to malloc()
 *
 *  The function pointer MMoutOfMemory() contains a vector to handle a
 *  'out-of-memory' error (which, by default, points at a simple wrap-up 
 *  and exit routine).
 */

void 
MMout_of_memory(long size);

static inline char*
MMalloc(long size) {
  if (size == 0) size = sizeof(long);

  char* result = (char*) malloc((unsigned) size);
  if (!result  && MMoutOfMemory)
    (*MMoutOfMemory)(size);

  return result;
}

static inline char *
MMrealloc(char *obj, long size) {
  if (!obj) return MMalloc(size);
  if (size <= 0) size = sizeof(long);
    
  char* result = (char*) realloc(obj, (unsigned) size);
  if (!result  && MMoutOfMemory)
    (*MMoutOfMemory)(size);

  return result;
}

static inline void
MMfree(char *obj) {
  if (obj) free(obj); 
}

#ifdef __cplusplus
}
#endif

#endif
