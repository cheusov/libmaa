/* timer.c -- Timer support
 * Created: Sat Oct  7 13:05:31 1995 by faith@cs.unc.edu
 * Revised: Tue Mar 11 16:03:09 1997 by faith@cs.unc.edu
 * Copyright 1995, 1996 Rickard E. Faith (faith@cs.unc.edu)
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
 * 
 * $Id: timer.c,v 1.14 1997/03/12 01:11:33 faith Exp $
 *
 * \section{Timer Support}
 *
 * \intro These routines provide access to a microsecond-resolution time
 * that can be used for profiling.
 *
 */

#include "maaP.h"

static hsh_HashTable _tim_Hash;

typedef struct tim_Entry {
   unsigned long  real;		/* wall time in usec */
   unsigned long  user;		/* user time in usec */
   unsigned long  system;	/* system time in usec */
   struct timeval real_mark;
   struct timeval user_mark;
   struct timeval system_mark;
} *tim_Entry;

static void _tim_check( void )
{
   if (!_tim_Hash) _tim_Hash = hsh_create( NULL, NULL );
}

/* \doc Start the named timer. */

void tim_start( const char *name )
{
   tim_Entry entry;
   struct rusage   rusage;

   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ))) {
      entry = xmalloc( sizeof( struct tim_Entry  ) );
      entry->real = entry->user = entry->system = 0;
      hsh_insert( _tim_Hash, name, entry );
   }

#if HAVE_GETTIMEOFDAY
   gettimeofday( &entry->real_mark, NULL );
#else
   time( &entry->real_mark.tv_sec );
   entry->real_mark.tv_usec = 0;
#endif
   getrusage( RUSAGE_SELF, &rusage );
   
   memcpy( (void *)&entry->user_mark, (void *)&rusage.ru_utime,
	   sizeof( struct timeval ) );
   memcpy( (void *)&entry->system_mark, (void *)&rusage.ru_stime,
	   sizeof( struct timeval ) );
}

/* \doc Stop the named timer. */

void tim_stop( const char *name )
{
   tim_Entry entry;
   struct timeval  real;
   struct rusage   rusage;

#define DIFFTIME(now,then)\
   (((now).tv_sec - (then).tv_sec) * 1000000 + (now).tv_usec - (then).tv_usec)

   _tim_check();
#if HAVE_GETTIMEOFDAY
   gettimeofday( &real, NULL );
#else
   time( &real.tv_sec );
   real.tv_usec = 0;
#endif
   getrusage( RUSAGE_SELF, &rusage );
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );
   
   entry->real   = DIFFTIME( real, entry->real_mark );
   entry->user   = DIFFTIME( rusage.ru_utime, entry->user_mark );
   entry->system = DIFFTIME( rusage.ru_stime, entry->system_mark );
}

/* \doc Reset the named timer to zero elapsed time.  Use |tim_start| to reset
   the start time.  */

void tim_reset( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   entry->real = entry->user = entry->system = 0;
}

/* \doc Get the wall time in seconds from the named timer.  The return
   value is a |double| and has microsecond resolution if the current system
   provides that accuracy (most don't). */

double tim_get_real( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   return entry->real / 1000000.0;
}

/* \doc Get the number of seconds of user CPU time. */

double tim_get_user( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   return entry->user / 1000000.0;
}

/* \doc Get the number of seconds of system CPU time. */

double tim_get_system( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   return entry->system / 1000000.0;
}

/* \doc Print the named timer values to |str|.  The format is similar to
   "time(1)". */

void tim_print_timer( FILE *str, const char *name )
{
   fprintf( str, "%-20s %0.3fr %0.3fu %0.3fs\n",
	    name,
	    tim_get_real( name ),
	    tim_get_user( name ),
	    tim_get_system( name ) );
}

static int _tim_iterator( const void *key, const void *datum, void *arg )
{
   FILE *str = (FILE *)arg;
   
   tim_print_timer( str, key );
   return 0;
}

/* \doc Print all the timers to |str|.  The order is arbitary. */

void tim_print_timers( FILE *str )
{
   if (_tim_Hash) hsh_iterate_arg( _tim_Hash, _tim_iterator, str );
}

static int _tim_freer( const void *key, const void *datum )
{
   xfree( (void *)datum ); /* Discard const */
   return 0;
}
   
/* \doc Free all memory associated with the timers.  This function is
   called automatically at program termination.  There should never be a
   need to call this function in user-level code. */

void _tim_shutdown( void )
{
   if (_tim_Hash) {
      hsh_iterate( _tim_Hash, _tim_freer );
      hsh_destroy( _tim_Hash );
   }
   _tim_Hash = NULL;
}
