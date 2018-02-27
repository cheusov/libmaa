/* debug.c -- Debugging support for Khepera
 * Created: Fri Dec 23 10:53:10 1994 by faith@dict.org
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

#include "maaP.h"

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
   |flag|. */

static const char *_dbg_name( dbg_Type flag )
{
   hsh_Position position;
   void         *key;
   void         *datum;

   if (!hash) err_fatal( __func__, "No debugging names registered\n" );
   HSH_ITERATE( hash, position, key, datum ) {
      if (flag == (dbg_Type)datum) {
	 HSH_ITERATE_END( hash );
	 return key;
      }
   }

   return "unknown flag";
}

/* |_dbg_register| is documented in the |dbg_register| section. */

void _dbg_register( dbg_Type flag, const char *name )
{
   dbg_Type tmp;
   
   for (tmp = flag & 0x3fffffff; tmp && !(tmp & 1); tmp >>= 1);
   if (!tmp || tmp >> 1)
	 err_fatal( __func__,
		    "Malformed flag (%lx):"
		    " a single low-order bit must be set\n",
		    flag );
   
   if (!hash) hash = hsh_create( NULL, NULL );
   
   if (_dbg_exists( flag ))
	 err_fatal( __func__,
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
	 err_fatal( __func__,
		    "Flag (%lx) may not have both high-order bits set\n",
		    flag );

   _dbg_register( flag, name );
}

/* \doc |dbg_set| sets the |name| flag.  If |name| is ``none,'' then all
   flags are cleared.  */

void dbg_set( const char *name )
{
   dbg_Type flag;

   if (!name) err_internal( __func__, "name is NULL\n" );
   if (!hash) err_fatal( __func__, "No debugging names registered\n" );
   if (!strcmp( name, "none" )) {
      setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = 0;
      return;
   }
   if (!strcmp( name, "all" )) {
      setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = ~0;
      return;
   }
   
   if (!(flag = (dbg_Type)hsh_retrieve( hash, name ))) {
      if (!(flag = (dbg_Type)hsh_retrieve( hash, name+1 ))
	  && *name != '-'
	  && *name != '+') {
	 
	 fprintf( stderr, "Valid debugging flags are:\n" );
	 dbg_list( stderr );
	 err_fatal( __func__,
		    "\"%s\" is not a valid debugging flag\n",
		    name );
      } else {
	 if (*name == '+') setFlags[ flag >> 30 ] |= flag;
	 else              setFlags[ flag >> 30 ] &= ~flag; /* - */
      }
   } else {
      setFlags[ flag >> 30 ] |= flag;
   }
}

/* \doc Used to set the flag using the predefined macro instead of the
   string name. */

void dbg_set_flag( dbg_Type flag )
{
   setFlags[ flag >> 30 ] |= flag;
}

/* \doc Used to unset the flag using the predefined macro instead of the
   string name. */

void dbg_unset_flag( dbg_Type flag )
{
   setFlags[ flag >> 30 ] &= ~flag;
}

/* \doc This function tests the |flag|, returning non-zero if the
   |flag| is set, and zero otherwise. */

int dbg_test( dbg_Type flag )
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


static int _dbg_user( const void *key, const void *datum, void *arg )
{
   FILE     *stream = (FILE *)arg;
   dbg_Type flag    = (dbg_Type)datum;
   
   if ((flag & 0xc0000000) != 0xc0000000)
	   fprintf( stream, "  %s\n", (char*) __UNCONST(key) );
   return 0;
}

static int _dbg_builtin( const void *key, const void *datum, void *arg )
{
   FILE     *stream = (FILE *)arg;
   dbg_Type flag    = (dbg_Type)datum;
   
   if ((flag & 0xc0000000) == 0xc0000000)
      fprintf( stream, "  %s (builtin)\n", (char*) __UNCONST(key) );
   return 0;
}
   
/* |dbg_list| lists all of the valid user-level debugging flags to the
   specified |stream|. */

void dbg_list( FILE *stream )
{
   hsh_iterate_arg( hash, _dbg_user, stream );
   hsh_iterate_arg( hash, _dbg_builtin, stream );
}
