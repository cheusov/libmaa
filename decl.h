/* decl.h -- Declarations for non-ANSI hosts
 * Created: Sun Nov 19 14:04:27 1995 by faith@cs.unc.edu
 * Revised: Sun Nov 19 14:08:52 1995 by faith@cs.unc.edu
 * Copyright 1995 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: decl.h,v 1.1 1995/11/19 19:28:35 faith Exp $
 * 
 */

#ifndef _MAA_DECL_H_
#define _MAA_DECL_H_

/* System dependent declarations: Many brain damaged systems don't provide
declarations for standard library calls.  We provide them here for
situations that we know about. */

#if defined(__sparc__)
extern void   bcopy( const void *, void *, int );
extern char   *memset( void *, int, int );
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
extern char   *strchr( const char *s, int c );
extern char   *strrchr( const char *s, int c );
extern int    fread( char *, int, int, FILE * );
extern int    fwrite( char *, int, int, FILE * );
extern int    _filbuf( FILE * );
extern time_t time( time_t * );
extern int    gettimeofday( struct timeval *, struct timezone * );
extern int    getrusage( int who, struct rusage * );
#endif				/* __sparc__ */

#if defined(__ultrix__) && defined(__MIPSEL__)
extern long random( void );
extern void srandom( int );
#endif

#endif
