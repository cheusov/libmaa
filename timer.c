/* timer.c -- Timer support
 * Created: Sat Oct  7 13:05:31 1995 by faith@cs.unc.edu
 * Revised: Sun Oct 22 15:17:41 1995 by r.faith@ieee.org
 * Copyright 1995 Rickard E. Faith (faith@cs.unc.edu)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 1, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * $Id: timer.c,v 1.3 1995/10/22 19:23:49 faith Exp $
 * 
 */

#include "kh.h"

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
   
   gettimeofday( &entry->real_mark, NULL );
   getrusage( RUSAGE_SELF, &rusage );
   
   memcpy( (void *)&entry->user_mark, (void *)&rusage.ru_utime,
	   sizeof( struct timeval ) );
   memcpy( (void *)&entry->system_mark, (void *)&rusage.ru_stime,
	   sizeof( struct timeval ) );
}

void tim_stop( const char *name )
{
   tim_Entry entry;
   struct timeval  real;
   struct rusage   rusage;

#define DIFFTIME(now,then)\
   ((now).tv_sec * 1000000 + (now).tv_usec) \
      - ((then).tv_sec * 1000000 + (then).tv_usec)

   _tim_check();
   gettimeofday( &real, NULL );
   getrusage( RUSAGE_SELF, &rusage );
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );
   
   entry->real   += DIFFTIME( real, entry->real_mark );
   entry->user   += DIFFTIME( rusage.ru_utime, entry->user_mark );
   entry->system += DIFFTIME( rusage.ru_stime, entry->system_mark );
}

void tim_reset( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   entry->real = entry->user = entry->system = 0;
}

double tim_get_real( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   return entry->real / 1000000.0;
}

double tim_get_user( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   return entry->user / 1000000.0;
}

double tim_get_system( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)hsh_retrieve( _tim_Hash, name ) ))
      err_internal ( __FUNCTION__, "No timer: %s\n", name );

   return entry->system / 1000000.0;
}

void tim_print_timer( FILE *str, const char *name )
{
   fprintf( str, "%-20s %0.3fr %0.3fu %0.3fs\n",
	    name,
	    tim_get_real( name ),
	    tim_get_user( name ),
	    tim_get_system( name ) );
}

void tim_print_timers( FILE *str )
{
   static int iterator( const void *key, const void *datum )
      {
	 tim_print_timer( str, key );
	 return 0;
      }

   if (_tim_Hash) hsh_iterate( _tim_Hash, iterator );
}

void _tim_shutdown( void )
{
   static int freer( const void *key, const void *datum )
      {
	 xfree( (void *)datum ); /* Discard const */
      }
   
   if (_tim_Hash) {
      hsh_iterate( _tim_Hash, freer );
      hsh_destroy( _tim_Hash );
   }
}
