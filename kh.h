/* kh.h -- Internal header file for Khepera
 * Created: Sun Nov  6 13:32:25 1994 by faith@cs.unc.edu
 * Revised: Thu Aug 24 23:17:53 1995 by r.faith@ieee.org
 * Copyright 1994 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: kh.h,v 1.3 1995/08/25 04:38:27 faith Exp $
 */

#ifndef _KH_H_
#define _KH_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
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
# ifndef HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
# ifndef HAVE_MEMCPY
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
#endif
# ifndef HAVE_STRDUP
char *strdup();
# endif

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

/* Provide assert() */
#include <assert.h>

/* Provide stdarg support */
#include <stdarg.h>

/* System dependent declarations: Many brain damaged systems don't provide
declarations for standard library calls.  We provide them here for
situations that we know about. */

#if defined(__sparc__)
extern char   *strdup( const char * );
extern int    _flsbuf( unsigned char, FILE * );
extern int    fflush( FILE * );
extern int    fprintf( FILE *, const char *, ... );
extern int    fputc( char, FILE * );
extern int    fputs( const char *, FILE * );
extern int    on_exit( void (*)(), caddr_t );
extern int    pclose( FILE * );
extern int    printf( const char *, ... );
extern int    unlink( const char * );
extern int    vfprintf( FILE *, const char *, ... );
extern int    vsprintf( char *, const char *, ... );
extern long   random( void );
extern long   strtol( const char *, char **, int );
extern void   fclose( FILE * );
extern void   perror( const char * );
extern void   srandom( int );
extern int    fread( char *, int, int, FILE * );
extern int    fwrite( char *, int, int, FILE * );
extern int    _filbuf( FILE * );
extern time_t time( time_t * );
#endif				/* __sparc__ */

#if defined(__ultrix__) && defined(__MIPSEL__)
extern long random( void );
extern void srandom( int );
#endif

#include <limits.h>
#ifndef RAND_MAX
# define RAND_MAX INT_MAX
#endif

#include "khepera.h"

				/* Local stuff */
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

extern int _hsh_primes[];

				/* dmalloc must be last */
#ifdef DMALLOC_FUNC_CHECK
# include "dmalloc.h"
#endif

#endif
