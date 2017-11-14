/* timer.c -- Timer support
 * Created: Sat Oct  7 13:05:31 1995 by faith@dict.org
 * Copyright 1995-1998, 2002 Rickard E. Faith (faith@dict.org)
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
 * \section{Timer Support}
 *
 * \intro These routines provide access to a microsecond-resolution time
 * that can be used for profiling.
 *
 */

#include "maaP.h"

static hsh_HashTable _tim_Hash;

typedef struct tim_Entry {
   double real;			/* wall time in sec */
   double self_user;		/* user time in sec */
   double self_system;		/* system time in sec */
   double children_user;	/* user time in sec */
   double children_system;	/* system time in sec */
   struct timeval real_mark;
   struct rusage  self_mark;
   struct rusage  children_mark;
} *tim_Entry;

static void _tim_check( void )
{
   if (!_tim_Hash) _tim_Hash = hsh_create( NULL, NULL );
}

/* \doc Start the named timer. */

void tim_start( const char *name )
{
   tim_Entry entry;

   _tim_check();
   if (!(entry = (tim_Entry)__UNCONST(hsh_retrieve( _tim_Hash, name )))) {
      entry = xmalloc( sizeof( struct tim_Entry  ) );
      entry->real            = 0.0;
      entry->self_user       = 0.0;
      entry->self_system     = 0.0;
      entry->children_user   = 0.0;
      entry->children_system = 0.0;
      hsh_insert( _tim_Hash, name, entry );
   }

   gettimeofday( &entry->real_mark, NULL );
   
   getrusage( RUSAGE_SELF, &entry->self_mark );
   getrusage( RUSAGE_CHILDREN, &entry->children_mark );
}

/* \doc Stop the named timer. */

void tim_stop( const char *name )
{
   tim_Entry entry;
   struct timeval  real;
   struct rusage   rusage;

#define DIFFTIME(now,then)\
   (((now).tv_sec - (then).tv_sec) \
    + ((now).tv_usec - (then).tv_usec)/1000000)

   _tim_check();
   gettimeofday( &real, NULL );
   
   if (!(entry = (tim_Entry)__UNCONST(hsh_retrieve( _tim_Hash, name )) ))
      err_internal ( __func__, "No timer: %s\n",
		     (name ? name : "<null>") );
   
   entry->real   = DIFFTIME( real, entry->real_mark );
   getrusage( RUSAGE_SELF, &rusage );
   entry->self_user   = DIFFTIME( rusage.ru_utime, entry->self_mark.ru_utime );
   entry->self_system = DIFFTIME( rusage.ru_stime, entry->self_mark.ru_stime );
   
   getrusage( RUSAGE_CHILDREN, &rusage );
   entry->children_user
      = DIFFTIME( rusage.ru_utime, entry->children_mark.ru_utime );
   entry->children_system
      = DIFFTIME( rusage.ru_stime, entry->children_mark.ru_stime );
}

/* \doc Reset the named timer to zero elapsed time.  Use |tim_start| to reset
   the start time.  */

void tim_reset( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)__UNCONST(hsh_retrieve( _tim_Hash, name )) ))
      err_internal ( __func__, "No timer: %s\n",
		     (name ? name : "<null>") );

   entry->real            = 0.0;
   entry->self_user       = 0.0;
   entry->self_system     = 0.0;
   entry->children_user   = 0.0;
   entry->children_system = 0.0;
}

/* \doc Get the wall time in seconds from the named timer.  The return
   value is a |double| and has microsecond resolution if the current system
   provides that accuracy (most don't). */

double tim_get_real( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)__UNCONST(hsh_retrieve( _tim_Hash, name )) ))
      err_internal ( __func__, "No timer: %s\n",
		     (name ? name : "<null>") );

   return entry->real;
}

/* \doc Get the number of seconds of user CPU time. */

double tim_get_user( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)__UNCONST(hsh_retrieve( _tim_Hash, name )) ))
      err_internal ( __func__, "No timer: %s\n",
		     (name ? name : "<null>") );

#if 0
   printf( "self: maxrss %ld ixrss %ld idrss %ld isrss %ld minflt %ld"
	   " majflt %ld nswap %ld inblock %ld outblock %ld msgsnd %ld"
	   " msgrcv %ld nsignals %ld nvcwm %ld nivcsm %ld\n",
	   entry->self_mark.ru_maxrss,
	   entry->self_mark.ru_ixrss,
	   entry->self_mark.ru_idrss,
	   entry->self_mark.ru_isrss,
	   entry->self_mark.ru_minflt,
	   entry->self_mark.ru_majflt,
	   entry->self_mark.ru_nswap,
	   entry->self_mark.ru_inblock,
	   entry->self_mark.ru_oublock,
	   entry->self_mark.ru_msgsnd,
	   entry->self_mark.ru_msgrcv,
	   entry->self_mark.ru_nsignals,
	   entry->self_mark.ru_nvcsw,
	   entry->self_mark.ru_nivcsw );
   printf( "chld: maxrss %ld ixrss %ld idrss %ld isrss %ld minflt %ld"
	   " majflt %ld nswap %ld inblock %ld outblock %ld msgsnd %ld"
	   " msgrcv %ld nsignals %ld nvcwm %ld nivcsm %ld\n",
	   entry->children_mark.ru_maxrss,
	   entry->children_mark.ru_ixrss,
	   entry->children_mark.ru_idrss,
	   entry->children_mark.ru_isrss,
	   entry->children_mark.ru_minflt,
	   entry->children_mark.ru_majflt,
	   entry->children_mark.ru_nswap,
	   entry->children_mark.ru_inblock,
	   entry->children_mark.ru_oublock,
	   entry->children_mark.ru_msgsnd,
	   entry->children_mark.ru_msgrcv,
	   entry->children_mark.ru_nsignals,
	   entry->children_mark.ru_nvcsw,
	   entry->children_mark.ru_nivcsw );
#endif
   
   return (entry->self_user + entry->children_user);
}

/* \doc Get the number of seconds of system CPU time. */

double tim_get_system( const char *name )
{
   tim_Entry entry;
   
   _tim_check();
   if (!(entry = (tim_Entry)__UNCONST(hsh_retrieve( _tim_Hash, name )) ))
      err_internal ( __func__, "No timer: %s\n",
		     (name ? name : "<null>") );

   return (entry->self_system + entry->children_system);
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
   xfree( __UNCONST(datum) ); /* Discard const */
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
