/* error.c -- Error reporting routines for Khepera
 * Created: Wed Dec 21 12:55:00 1994 by faith@cs.unc.edu
 * Revised: Fri Sep 29 21:30:55 1995 by r.faith@ieee.org
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
 * $Id: error.c,v 1.4 1995/10/02 00:32:45 faith Exp $
 *
 * \section{Error Reporting Routines}
 *
 * \intro Several error reporting routines are provided.  These routines
 * are always used to print errors generated within the \khepera library,
 * and are available for the programmer as well.
 *
 */

#include "kh.h"
#include <stdarg.h>
#include <errno.h>

/* \doc |err_fatal| flushes "stdout", prints a fatal error report on
   "stderr", flushes "stderr" and "stdout", and calls |exit|.  |routine| is
   the name of the routine in which the error took place. */

void err_fatal( const char *routine, const char *format, ... )
{
   va_list ap;

   fflush( stdout );
   va_start( ap, format );
   fprintf( stderr, "\n%s: ", routine );
   vfprintf( stderr, format, ap );
#if 0
   if (errno) perror( routine );
#endif
   va_end( ap );
   fflush( stderr );
   fflush( stdout );
   exit( 1 );
}

/* \doc |err_warning| flushes "stdout", prints a non-fatal warning on
   "stderr", and returns to the caller.  |routine| is the name of the
   calling routine. */

void err_warning( const char *routine, const char *format, ... )
{
   va_list ap;

   fflush( stdout );
   va_start( ap, format );
   fprintf( stderr, "\n%s:\n   ", routine );
   vfprintf( stderr, format, ap );
   va_end( ap );
}

/* \doc |err_internal| flushes "stdout", prints the fatal error message,
   flushes "stderr" and "stdout", and calls |abort| so that a core dump is
   generated. */

void err_internal( const char *routine, const char *format, ... )
{
   va_list ap;

   fflush( stdout );
   va_start( ap, format );
   fprintf( stderr, "\nInternal error in %s:\n  ", routine );
   vfprintf( stderr, format, ap );
   va_end( ap );
   fprintf( stderr, "Aborting proteus: " );
   fflush( stderr );
   fflush( stdout );
   abort();
}
