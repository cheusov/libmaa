/* maaP.h -- Internal header file for libmaa
 * Created: Sun Nov 19 13:19:44 1995 by faith@cs.unc.edu
 * Revised: Mon Feb 26 09:19:11 1996 by faith@cs.unc.edu
 * Copyright 1994, 1995 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: maaP.h,v 1.4 1996/02/26 15:39:11 faith Exp $
 */

#ifndef _MAAP_H_
#define _MAAP_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef __GNUC__
#define __FUNCTION__ __FILE__
#endif

/* AIX requires this to be the first thing in the file.  */
#ifdef __GNUC__
# define alloca __builtin_alloca
#else
# if HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
 #pragma alloca
#  else
#   ifndef alloca /* predefined by HP cc +Olibcalls */
char *alloca ();
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

#if !HAVE_STRDUP
char *strdup( const char * );
#endif

#if !HAVE_STRTOL
long strtol( const char *, char **, int );
#endif

#if !HAVE_STRTOUL
unsigned long int strtoul( const char *, char **, int );
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
extern int  getopt( int, char **, char * );
extern int  optind;
extern char *optarg;
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
#ifndef RAND_MAX
# define RAND_MAX INT_MAX
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
