/* maaP.h -- Internal header file for libmaa
 * Created: Sun Nov 19 13:19:44 1995 by faith@dict.org
 * Revised: Sat Mar 30 12:04:40 2002 by faith@dict.org
 * Copyright 1994-1998, 2002 Rickard E. Faith (faith@dict.org)
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
 * $Id: maaP.h,v 1.25 2005/12/05 17:21:20 cheusov Exp $
 */

#ifndef _MAAP_H_
#define _MAAP_H_

#include <stddef.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_ALLOCA_H
# include <alloca.h>
#endif

#ifndef HAVE_ALLOCA
# ifndef alloca /* predefined by HP cc +Olibcalls */
#  ifdef _AIX
#   pragma alloca
#  else
#   if !defined(__svr4__) && !defined(__sgi__)	/* not on IRIX */
     void *alloca(size_t size);
#   endif
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
# if !HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
# if !HAVE_MEMCPY
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
#endif

#if !HAVE_STRLCPY
extern size_t strlcpy_libmaa (char *s, const char * wc, size_t size);
#define strlcpy strlcpy_libmaa
#endif

#if !HAVE_STRDUP
extern char *strdup( const char * );
#endif

#if !HAVE_STRTOL
extern long strtol( const char *, char **, int );
#endif

#if !HAVE_STRTOUL
extern unsigned long int strtoul( const char *, char **, int );
#endif

#if !SIZEOF_VOID_P
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

/* Include some standard header files. */
#include <stdio.h>
#if HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
#endif

/* Handle getopt correctly */
#if HAVE_GETOPT_H
# include <getopt.h>
#else
#if !HAVE_GETOPT
extern int  getopt( int, char * const *, const char * );
extern int  optind;
extern char *optarg;
#else
# include <unistd.h>
#endif
#endif

/* We actually need a few non-ANSI C things... */
#if defined(__STRICT_ANSI__)
extern char     *strdup( const char * );
extern int      fileno( FILE *stream );
extern FILE     *fdopen( int fildes, const char *mode );
extern void     bcopy( const void *src, void *dest, int n );
extern long int random( void );
extern void     srandom( unsigned int );
#endif

#if HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif

/* Provide assert() */
#include <assert.h>

/* Provide stdarg support */
#include <stdarg.h>

/* System dependent declarations: Many brain damaged systems don't provide
declarations for standard library calls.  We provide them here for
situations that we know about. */
#include "decl.h"

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
