/* string.c -- String pool for Khepera
 * Created: Wed Dec 21 21:32:34 1994 by faith@cs.unc.edu
 * Revised: Thu Mar  9 14:43:43 1995 by faith@cs.unc.edu
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
 * $Id: string.c,v 1.1 1995/04/21 15:31:47 faith Exp $
 *
 * \section{String Pool Routines}
 *
 * \intro These routines provide support for string pool objects.  In
 * general, only the |str_find| function will be used.  This function takes
 * a pointer to a null-terminated string and returns a pointer to another
 * null-terminated string which contains the same information.  The pointer
 * returned will be identical for all identical strings.  Memory for string
 * storage is automatically reclaimed at program termination on systems
 * that support |atexit| or |on_exit|.
 *
 */

#include "kh.h"

static str_Pool global;

typedef struct poolInfo {
   mem_String    string;
   hsh_HashTable hash;
} *poolInfo;

/* \doc |str_pool_create| initialized a string pool object. */

str_Pool str_pool_create( void )
{
   poolInfo pool = xmalloc( sizeof( struct poolInfo ) );

   pool->string = mem_create_strings();
   pool->hash   = hsh_create( NULL, NULL );

   return pool;
}

/* \doc |str_pool_destroy| destroys the string pool object, |pool|, and all
   memory associated with it. */

void str_pool_destroy( str_Pool pool )
{
   poolInfo p = (poolInfo)pool;
   
   if (!p || !p->string || !p->hash)
	 err_fatal( __FUNCTION__, "String pool improperly initialized\n" );

   mem_destroy_strings( p->string );
   hsh_destroy( p->hash );
   xfree( p );
}

/* \doc |str_pool_find| looks up the string, |s|, in the memory associated
   with the string pool object, |pool|.  If the string is found, a pointer
   to the previously stored string is returned.  Otherwise, the string is
   copied into string pool memory, and a pointer to the newly allocated
   memory is returned. */

const char *str_pool_find( str_Pool pool, const char *s )
{
   const char *datum;
   poolInfo   p = (poolInfo)pool;
   
   if ((datum = hsh_retrieve( p->hash, s ))) return datum;
   datum = mem_strcpy( p->string, s );
   hsh_insert( p->hash, datum, datum );

   return datum;
}

/* \doc |str_find| acts like |str_pool_find|, except the global string pool
   is used.  If the global string pool has not been initialized, it will be
   initialized automatically.  Further, on systems that support |atexit| or
   |on_exit|, |str_destroy| will be called automatically at program
   termination. */

const char *str_find( const char *s )
{
   if (!global) {
      global = str_pool_create();
#ifndef __CHECKER__
#ifdef HAVE_ATEXIT
      atexit( str_destroy );
#else
# ifdef HAVE_ON_EXIT
      on_exit( str_destroy, NULL );
# endif
#endif
#endif
   }

   return str_pool_find( global, s );
}

/* \doc |str_findn| acts like |str_find|, except that the lenght of the
   string is specified, and the string does not have to be "NULL"
   terminated. */

const char *str_findn( const char *s, int length )
{
   char *tmp = alloca( sizeof( char ) * (length + 1) );
   
   if (!global) {
      global = str_pool_create();
#ifndef __CHECKER__
#ifdef HAVE_ATEXIT
      atexit( str_destroy );
#else
# ifdef HAVE_ON_EXIT
      on_exit( str_destroy, NULL );
# endif
#endif
#endif
   }

   strncpy( tmp, s, length );
   tmp[ length ] = '\0';
   return str_pool_find( global, tmp );
}

/* \doc |str_destroy| frees all of the memory associated with the global
   string pool.  Since this function is called automatically at program
   termination on systems that support |atexit| or |on_exit|, there should
   be no need to call this function explicitly.

   If this function is called explicitly, the next call to |str_find| will
   re-initialize the global string pool. */

void str_destroy( void )
{
   if (global) str_pool_destroy( global );
   global = NULL;
}
