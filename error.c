/* error.c -- Error reporting routines for Khepera
 * Created: Wed Dec 21 12:55:00 1994 by faith@cs.unc.edu
 * Revised: Mon Sep 23 16:23:49 1996 by faith@cs.unc.edu
 * Copyright 1994, 1995, 1996 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: error.c,v 1.10 1996/09/23 23:20:37 faith Exp $
 *
 * \section{Error Reporting Routines}
 *
 * \intro Several error reporting routines are provided.  These routines
 * are always used to print errors generated within the \khepera library,
 * and are available for the programmer as well.
 *
 */

#include "maaP.h"
#include <errno.h>

const char *_err_programName;

/* \doc |err_set_program_name| records the value of |argv[0]| for the
   calling program.  If this value is not "NULL", then it will be used when
   printing errors and warnings. */

void err_set_program_name( const char *programName )
{
   _err_programName = programName;
}

/* \doc |err_program_name| returns the value of |programName| that was
   previously set with |err_set_program_name|.  This value may be
   "NULL". */

const char *err_program_name( void )
{
   return _err_programName;
}


/* \doc |err_fatal| flushes "stdout", prints a fatal error report on
   "stderr", flushes "stderr" and "stdout", and calls |exit|.  |routine| is
   the name of the routine in which the error took place. */

void err_fatal( const char *routine, const char *format, ... )
{
   va_list ap;

   fflush( stdout );
   if (_err_programName)
      fprintf( stderr, "\n%s:%s: ", _err_programName, routine );
   else
      fprintf( stderr, "\n%s: ", routine );
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   va_end( ap );
   
   fflush( stderr );
   fflush( stdout );
   exit( 1 );
}

/* \doc |err_fatal_errno| flushes "stdout", prints a fatal error report on
   "stderr", prints the system error corresponding to |errno|, flushes
   "stderr" and "stdout", and calls |exit|.  |routine| is the name of the
   routine in which the error took place. */

void err_fatal_errno( const char *routine, const char *format, ... )
{
   va_list ap;
   int     errorno = errno;

   fflush( stdout );
   if (_err_programName)
      fprintf( stderr, "\n%s:%s: ", _err_programName, routine );
   else
      fprintf( stderr, "\n%s: ", routine );
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   va_end( ap );

#if HAVE_STRERROR
   fprintf( stderr, "%s: %s\n", routine, strerror( errorno ) );
#else
   errno = errorno;
   perror( routine );
#endif
   
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
   fflush( stderr );
   if (_err_programName)
      fprintf( stderr, "%s:%s:\n   ", _err_programName, routine );
   else
      fprintf( stderr, "%s:\n   ", routine );
   
   va_start( ap, format );
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
   fprintf( stderr, "\nInternal error in %s:\n  ", routine );
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   va_end( ap );

   if (_err_programName)
      fprintf( stderr, "Aborting %s: ", _err_programName );
   else
      fprintf( stderr, "Aborting: " );
   fflush( stderr );
   fflush( stdout );
   abort();
}
