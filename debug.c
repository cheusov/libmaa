/* debug.c -- Debugging support for Khepera
 * Created: Fri Dec 23 10:53:10 1994 by faith@cs.unc.edu
 * Revised: Thu Sep 28 18:58:12 1995 by faith@cs.unc.edu
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
 * $Id: debug.c,v 1.3 1995/09/29 00:55:41 faith Exp $
 *
 * \section{Debugging Support}
 *
 * \intro These routines provide low-level support for run-time debugging
 * messages.  A set of global flags are maintained using "#define"
 * statements.  These flags are assumed to be 32-bit integers, the top two
 * bits of which are used to select one of four sets of debugging lags.
 * Each set, therefore, has 30 bits of flag information.  The last set
 * (i.e., when the top two bits are both set) is reserved for internal use
 * by the \khepera library.  For convenience, each flag can be given a
 * unique name, so that flags can be set easily with command-line options.
 *
 */

#include "kh.h"

#define TEST(flags,var) (((flags)>>31)                                     \
			 ? (((flags)>>30)                                  \
			    ? ((var[3] & (flags)) << 2)                    \
			    : ((var[2] & (flags)) << 2))                   \
			 : (((flags)>>30)                                  \
			    ? ((var[1] & (flags)) << 2)                    \
			    : ((var[0] & (flags)) << 2)))

static hsh_HashTable hash;
static dbg_Type      setFlags[4];
static dbg_Type      usedFlags[4];

/* |_dbg_exists| returns non-zero if |flag| has been associated with a name
   (using the |_dbg_register| function). */

static int _dbg_exists( dbg_Type flag )
{
   return TEST( flag, usedFlags );
}

/* |_dbg_name| returns a pointer to the name that was associated with the
   |flag|.  Note the use of nested functions is not portable to non-GNU
   compilers. */

static const char *_dbg_name( dbg_Type flag )
{
   const char *pt = "unknown flag";

   static int iterator( const void *key, const void *datum )
	 {
	    if (flag == (dbg_Type)datum) {
	       pt = key;
	       return 1;
	    }
	    return 0;
	 }

   hsh_iterate( hash, iterator );
   return pt;
}

/* |_dbg_register| is documented in the |dbg_register| section. */

void _dbg_register( dbg_Type flag, const char *name )
{
   dbg_Type tmp;
   
   for (tmp = flag & 0x3fffffff; tmp && !(tmp & 1); tmp >>= 1);
   if (!tmp || tmp >> 1)
	 err_fatal( __FUNCTION__,
		    "Malformed flag (%lx):"
		    " a single low-order bit must be set\n",
		    flag );
   
   if (!hash) hash = hsh_create( NULL, NULL );
   
   if (_dbg_exists( flag ))
	 err_fatal( __FUNCTION__,
		    "The debug flag %lx has been used for \"%s\""
		    " and cannot be reused for \"%s\"\n",
		    flag,
		    _dbg_name( flag ),
		    name );

   hsh_insert( hash, name, (void *)flag );
}

/* \doc |dbg_register| is used to set up an asociated between a |flag| and
   a |name|.  After this association is made, calls to |dbg_set| can use
   |name| to set the global debugging flag.  Both of the high bits cannot
   be set simultaneously---these flags are reserved for internal use by the
   \khepera library.\footnote{For internal \khepera library use,
   |_dbg_register| can be used to register flags which have both high bits
   set.} */

void dbg_register( dbg_Type flag, const char *name )
{
				/* These values are reserved for Khepera */
   if ((flag & 0xc0000000) == 0xc0000000)
	 err_fatal( __FUNCTION__,
		    "Flag (%lx) may not have both high-order bits set\n",
		    flag );

   _dbg_register( flag, name );
}

/* \doc |dbg_set| sets the |name| flag.  If |name| is ``none,'' then all
   flags are cleared.  */

void dbg_set( const char *name )
{
   dbg_Type flag;

   if (!name) err_internal( __FUNCTION__, "name is NULL\n" );
   if (!hash) err_fatal( __FUNCTION__, "No debugging names registered\n" );
   if (!strcmp( name, "none" )) {
      setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = 0;
      return;
   }

   if (!(flag = (dbg_Type)hsh_retrieve( hash, name ))) {
      fprintf( stderr, "Valid debugging flags are:\n" );
      dbg_list( stderr );
      err_fatal( __FUNCTION__,
		 "\"%s\" is not a valid debugging flag\n",
		 name );
   }

   setFlags[ flag >> 30 ] |= flag;
}

/* \doc Thsi inlined function tests the |flag|, returning non-zero if the
   |flag| is set, and zero otherwise. */

__inline__ int dbg_test( dbg_Type flag )
{
   return TEST( flag, setFlags );
}

/* \doc |dbg_destroy| destroys the memory associated with the debugging
   support routines.  This routine should \emph{never} be called by the
   programmer: it is automatically called at program termination on systems
   that support the |atexit| or |on_exit| calls. */

void dbg_destroy( void )
{
   if (hash) hsh_destroy( hash );
   hash = NULL;
   setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = 0;
   usedFlags[0] = usedFlags[1] = usedFlags[2] = usedFlags[3] = 0;
}

/* |dbg_list| lists all of the valid user-level debugging flags to the
   specified |stream|.  Note the use of nested functions is not portable to
   non-GNU compilers. */

void dbg_list( FILE *stream )
{
   static int iterator( const void *key, const void *datum )
      {
	 dbg_Type flag = (dbg_Type)datum;
	 
	 if ((flag & 0xc0000000) != 0xc0000000)
	    fprintf( stream, "  %s\n", (char *)key );
	 return 0;
      }
   
   hsh_iterate( hash, iterator );
}
