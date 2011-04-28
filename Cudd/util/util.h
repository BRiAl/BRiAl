/* $Id$ */

#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus

#include <ctime>
extern "C" {

#else
#include <time.h>

#endif

#if defined(__GNUC__)
#   define UTIL_INLINE __inline__
#   if __GNUC__ > 2 || __GNUC_MINOR__ >= 7
#       define UTIL_UNUSED __attribute__ ((unused))
#   else
#       define UTIL_UNUSED
#   endif
#else
#   define UTIL_INLINE
#   define UTIL_UNUSED
#endif

#ifndef SIZEOF_VOID_P
#define SIZEOF_VOID_P 4
#endif
#ifndef SIZEOF_INT
#define SIZEOF_INT 4
#endif
#ifndef SIZEOF_LONG
#define SIZEOF_LONG 4
#endif

#if SIZEOF_VOID_P == 8 && SIZEOF_INT == 4
typedef long util_ptrint;
#else
typedef int util_ptrint;
#endif

/* #define USE_MM */		/* choose libmm.a as the memory allocator */

/* these are too entrenched to get away with changing the name */
#define strsav		util_strsav
#include <unistd.h>
extern char *optarg;
extern int optind, opterr;

#define NIL(type)		((type *) 0)

#if defined(USE_MM) || defined(MNEMOSYNE)
/*
 *  assumes the memory manager is either libmm.a or libmnem.a
 *	libmm.a:
 *	- allows malloc(0) or realloc(obj, 0)
 *	- catches out of memory (and calls MMout_of_memory())
 *	- catch free(0) and realloc(0, size) in the macros
 *	libmnem.a:
 *	- reports memory leaks
 *	- is used in conjunction with the mnemalyse postprocessor
 */
#ifdef MNEMOSYNE
#include "mnemosyne.h"
#define ALLOC(type, num)	\
    ((num) ? ((type *) malloc(sizeof(type) * (num))) : \
	    ((type *) malloc(sizeof(long))))
#else
#define ALLOC(type, num)	\
    ((type *) malloc(sizeof(type) * (num)))
#endif
#define REALLOC(type, obj, num)	\
    (obj) ? ((type *) realloc((char *) obj, sizeof(type) * (num))) : \
	    ((type *) malloc(sizeof(type) * (num)))
#define FREE(obj)		\
    ((obj) ? (free((char *) (obj)), (obj) = 0) : 0)
#else
/*
 *  enforce strict semantics on the memory allocator
 *	- when in doubt, delete the '#define USE_MM' above
 */
#define ALLOC(type, num)	\
    ((type *) MMalloc((long) sizeof(type) * (long) (num)))
#define REALLOC(type, obj, num)	\
    ((type *) MMrealloc((char *) (obj), (long) sizeof(type) * (long) (num)))
#define FREE(obj)		\
    ((obj) ? (free((char *) (obj)), (obj) = 0) : 0)
#endif


/* Ultrix (and SABER) have 'fixed' certain functions which used to be int */
#if defined(ultrix) || defined(SABER) || defined(aiws) || defined(hpux) || defined(apollo) || defined(__osf__) || defined(__SVR4) || defined(__GNUC__)
#define VOID_OR_INT void
#define VOID_OR_CHAR void
#else
#define VOID_OR_INT int
#define VOID_OR_CHAR char
#endif


/* No machines seem to have much of a problem with these */
#include <stdio.h>
#include <ctype.h>


/* Some machines fail to define some functions in stdio.h */
#if !defined(__STDC__) && !defined(__cplusplus)
extern FILE *popen(), *tmpfile();
extern int pclose();
#endif


/* most machines don't give us a header file for these */
#if (defined(__STDC__) || defined(__cplusplus) || defined(ultrix)) && !defined(MNEMOSYNE) || defined(__SVR4)
# include <stdlib.h>
#else
# ifndef _IBMR2
    extern VOID_OR_INT abort(), exit();
# endif
# if !defined(MNEMOSYNE) && !defined(_IBMR2)
    extern VOID_OR_INT free (void *);
    extern VOID_OR_CHAR *malloc(), *realloc();
# endif
  extern char *getenv();
  extern int system();
  extern double atof();
#endif


/* some call it strings.h, some call it string.h; others, also have memory.h */
#if defined(__STDC__) || defined(__cplusplus) || defined(_IBMR2) || defined(ultrix)
#include <string.h>
#else
/* ANSI C string.h -- 1/11/88 Draft Standard */
extern char *strcpy(), *strncpy(), *strcat(), *strncat(), *strerror();
extern char *strpbrk(), *strtok(), *strchr(), *strrchr(), *strstr();
extern int strcoll(), strxfrm(), strncmp(), strlen(), strspn(), strcspn();
extern char *memmove(), *memccpy(), *memchr(), *memcpy(), *memset();
extern int memcmp(), strcmp();
#endif


#ifdef __STDC__
#include <assert.h>
#else
#ifndef NDEBUG
#define assert(ex) {\
    if (! (ex)) {\
	(void) fprintf(stderr,\
	    "Assertion failed: file %s, line %d\n\"%s\"\n",\
	    __FILE__, __LINE__, "ex");\
	(void) fflush(stdout);\
	abort();\
    }\
}
#else
#define assert(ex) ;
#endif
#endif


#define fail(why) {\
    (void) fprintf(stderr, "Fatal error: file %s, line %d\n%s\n",\
	__FILE__, __LINE__, why);\
    (void) fflush(stdout);\
    abort();\
}


#ifdef lint
#undef putc			/* correct lint '_flsbuf' bug */
#undef ALLOC			/* allow for lint -h flag */
#undef REALLOC
#define ALLOC(type, num)	(((type *) 0) + (num))
#define REALLOC(type, obj, num)	((obj) + (num))
#endif


/* These arguably do NOT belong in util.h */
#ifndef ABS
#define ABS(a)			((a) < 0 ? -(a) : (a))
#endif
#ifndef MAX
#define MAX(a,b)		((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)		((a) < (b) ? (a) : (b))
#endif

#ifndef USE_MM
extern char *MMalloc (long);
extern void MMout_of_memory (long);
extern void (*MMoutOfMemory) (long);
extern char *MMrealloc (char *, long);
#endif


#ifdef CUDD_ORIGINAL_INCLUSION
extern long util_cpu_time (void);

#else
  
  inline long util_cpu_time () {
    clock_t saved_time = clock();
    return (((long)saved_time)*1000) / CLOCKS_PER_SEC;

  }
#endif
extern int util_getopt (int, char **, char *);
extern void util_getopt_reset (void);
extern char *util_path_search (char *);
extern char *util_file_search (char *, char *, char *);
extern int util_pipefork (char **, FILE **, FILE **, int *);
extern void util_print_cpu_stats (FILE *);
extern char *util_print_time (unsigned long);
extern int util_save_image (char *, char *);
extern char *util_strsav (char *);
extern char *util_tilde_expand (char *);
extern void util_restart (char *, char *, int);


/* util_getopt() global variables (ack !) */
extern int util_optind;
extern char *util_optarg;
#ifdef CUDD_ORIGINAL_INCLUSION
extern long getSoftDataLimit (void);
#else
inline long
getSoftDataLimit(void) {
  const long RLIMIT_DATA_DEFAULT = 67108864;	/* assume 64MB by default */
  return RLIMIT_DATA_DEFAULT;
} /* end of getSoftDataLimit */


#endif


#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */
