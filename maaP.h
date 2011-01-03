/* maaP.h -- Internal header file for libmaa
 * Created: Sun Nov 19 13:19:44 1995 by faith@dict.org
 * Copyright 1994-1998, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#ifndef _MAAP_H_
#define _MAAP_H_

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if defined(__INTERIX) && !defined(_ALL_SOURCE)
/* bug in Interix-3.5 header files? */
long random (void);
int isascii (int c);
int gethostname(char *, size_t);
void vsyslog (int priority, const char *message, va_list args);
int getopt (int argc, char * const *argv, const char *optstring);
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_STDINT_H
#include <stdint.h>
#endif

#if HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#if HAVE_ALLOCA_H
# include <alloca.h>
#endif

#ifndef HAVE_ALLOCA
# ifndef alloca /* predefined by HP cc +Olibcalls */
#  ifdef _AIX
#    pragma alloca
#  else
     void *alloca(size_t size);
#  endif
# endif
#endif

/* Get string functions */
#if STDC_HEADERS
# include <string.h>
#else
# if HAVE_STRINGS_H
#  include <strings.h>
# endif
# if !defined(HAVE_MEMCPY)
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
#endif

#if !defined(HAVE_STRLCPY)
extern size_t strlcpy_libmaa (char *s, const char * wc, size_t size);
#define strlcpy strlcpy_libmaa
#endif

#if !defined(HAVE_STRTOL)
extern long strtol( const char *, char **, int );
#endif

#if !defined(HAVE_STRTOUL)
extern unsigned long int strtoul( const char *, char **, int );
#endif

#if !defined(SIZEOF_VOID_P)
# define SIZEOF_VOID_P sizeof (void *)
#endif

/* Get time functions */
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

/* Handle getopt correctly */
#if HAVE_GETOPT_H
# include <getopt.h>
#endif
#if !defined(HAVE_GETOPT)
extern int  getopt( int, char * const *, const char * );
#endif
extern int  optind;
extern char *optarg;

/* We actually need a few non-ANSI C things... */
#if defined(__STRICT_ANSI__)
extern char     *strdup( const char * );
extern FILE     *fdopen( int fildes, const char *mode );
extern long int random( void );
#endif

#if HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif

/* Provide assert() */
#include <assert.h>

#if HAVE_LIMITS_H
#include <limits.h>
#endif

				/* Local stuff */
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#include "maa.h"

				/* dmalloc must be last */
#ifdef DMALLOC_FUNC_CHECK
# include "dmalloc.h"
#endif

#endif
