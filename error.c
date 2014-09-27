/* error.c -- Error reporting routines for Khepera
 * Created: Wed Dec 21 12:55:00 1994 by faith@dict.org
 * Copyright 1994-1997, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
   const char *pt =strrchr( programName, '/' );

   if (pt)
      _err_programName = pt + 1;
   else
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
   if (_err_programName) {
      if (routine)
	 fprintf( stderr, "%s (%s): ", _err_programName, routine );
      else
	 fprintf( stderr, "%s: ", _err_programName );
   } else {
      if (routine) fprintf( stderr, "%s: ", routine );
   }
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   log_error_va( routine, format, ap );
   va_end( ap );
   
   fflush( stderr );
   fflush( stdout );
   exit ( 1 );
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
   if (_err_programName) {
      if (routine)
	 fprintf( stderr, "%s (%s): ", _err_programName, routine );
      else
	 fprintf( stderr, "%s: ", _err_programName );
   } else {
      if (routine) fprintf( stderr, "%s: ", routine );
   }
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   log_error_va( routine, format, ap );
   va_end( ap );

#if HAVE_STRERROR
   fprintf( stderr, "%s: %s\n", routine, strerror( errorno ) );
   log_error( routine, "%s: %s\n", routine, strerror( errorno ) );
#else
   errno = errorno;
   perror( routine );
   log_error( routine, "%s: errno = %d\n", routine, errorno );
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
   if (_err_programName) {
      if (routine)
	 fprintf( stderr, "%s (%s): ", _err_programName, routine );
      else
	 fprintf( stderr, "%s: ", _err_programName );
   } else {
      if (routine) fprintf( stderr, "%s: ", routine );
   }
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   log_error_va( routine, format, ap );
   va_end( ap );
}

/* \doc |err_internal| flushes "stdout", prints the fatal error message,
   flushes "stderr" and "stdout", and calls |abort| so that a core dump is
   generated. */

void err_internal( const char *routine, const char *format, ... )
{
   va_list ap;

   fflush( stdout );
   if (_err_programName) {
      if (routine)
	 fprintf( stderr, "%s (%s): Internal error\n     ",
		  _err_programName, routine );
      else
	 fprintf( stderr, "%s: Internal error\n     ", _err_programName );
   } else {
      if (routine) fprintf( stderr, "%s: Internal error\n     ", routine );
      else         fprintf( stderr, "Internal error\n     " );
   }
   
   va_start( ap, format );
   vfprintf( stderr, format, ap );
   log_error( routine, format, ap );
   va_end( ap );

   if (_err_programName)
      fprintf( stderr, "Aborting %s...\n", _err_programName );
   else
      fprintf( stderr, "Aborting...\n" );
   fflush( stderr );
   fflush( stdout );
   abort();
}
