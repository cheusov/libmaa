/* flags.c -- Flag support for Khepera
 * Created: Sat Mar 23 10:11:52 1996 by faith@dict.org
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
 * \section{Flag Support}
 *
 * \intro These routines provide low-level support for run-time program
 * flow control.  The mechanism used is similar to that used for debugging
 * messages.  A set of global flags are maintained using "#define"
 * statements.  These flags are assumed to be 32-bit integers, the top two
 * bits of which are used to select one of four sets of debugging lags.
 * Each set, therefore, has 30 bits of flag information.  For convenience,
 * each flag can be given a unique name, so that flags can be set easily
 * with command-line options.
 * */

#include "maaP.h"

#define TEST(flags,var) (((flags)>>31)                                     \
			 ? (((flags)>>30)                                  \
			    ? ((var[3] & (flags)) << 2)                    \
			    : ((var[2] & (flags)) << 2))                   \
			 : (((flags)>>30)                                  \
			    ? ((var[1] & (flags)) << 2)                    \
			    : ((var[0] & (flags)) << 2)))

static hsh_HashTable hash;
static flg_Type      setFlags[4];
static flg_Type      usedFlags[4];

/* |_flg_exists| returns non-zero if |flag| has been associated with a name
   (using the |flg_register| function). */

static int _flg_exists( flg_Type flag )
{
   return TEST( flag, usedFlags );
}

/* |flg_name| returns a pointer to the name that was associated with the
   |flag|. */

const char *flg_name( flg_Type flag )
{
   hsh_Position position;
   void         *key;
   void         *datum;

   HSH_ITERATE( hash, position, key, datum ) {
      if (flag == (flg_Type)datum) {
	 HSH_ITERATE_END( hash );
	 return key;
      }
   }

   return "unknown flag";
}

/* \doc |flg_register| is used to set up an asociated between a |flag| and
   a |name|.  After this association is made, calls to |flg_set| can use
   |name| to set the global flag.  */

void flg_register( flg_Type flag, const char *name )
{
   flg_Type tmp;
   
   for (tmp = flag & 0x3fffffff; tmp && !(tmp & 1); tmp >>= 1);
   if (!tmp || tmp >> 1)
	 err_fatal( __func__,
		    "Malformed flag (%lx):"
		    " a single low-order bit must be set\n",
		    flag );
   
   if (!hash) hash = hsh_create( NULL, NULL );
   
   if (_flg_exists( flag ))
	 err_fatal( __func__,
		    "The flag %lx has been used for \"%s\""
		    " and cannot be reused for \"%s\"\n",
		    flag,
		    flg_name( flag ),
		    name );

   hsh_insert( hash, name, (void *)flag );
}

/* \doc |flg_set| sets the |name| flag.  If |name| is ``none,'' then all
   flags are cleared.  */

void flg_set( const char *name )
{
   flg_Type flag;

   if (!name) err_internal( __func__, "name is NULL\n" );
   if (!hash) err_fatal( __func__, "No flag names registered\n" );
   if (!strcmp( name, "none" )) {
      setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = 0;
      return;
   }
   if (!strcmp( name, "all" )) {
      setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = ~0;
      return;
   }

   if (!(flag = (flg_Type)hsh_retrieve( hash, name ))) {
      flag = 0;

      if (
	 (*name != '-' && *name != '+') ||
	 !(flag = (flg_Type) hsh_retrieve( hash, name+1 )))
      {
	 fprintf( stderr, "Valid flags are:\n" );
	 flg_list( stderr );
	 err_fatal( __func__,
		    "\"%s\" is not a valid flag\n",
		    name );
      } else {
	 if (flag){
	    if (*name == '+') setFlags[ flag >> 30 ] |= flag;
	    else              setFlags[ flag >> 30 ] &= ~flag; /* - */
	 }
      }
   } else {
      setFlags[ flag >> 30 ] |= flag;
   }
}

/* \doc This function tests the |flag|, returning non-zero if the
   |flag| is set, and zero otherwise. */

int flg_test( flg_Type flag )
{
   return TEST( flag, setFlags );
}

/* \doc |flg_destroy| destroys the memory associated with the flag support
   routines.  This routine should \emph{never} be called by the programmer:
   it is automatically called at program termination on systems that
   support the |atexit| or |on_exit| calls. */

void flg_destroy( void )
{
   if (hash) hsh_destroy( hash );
   hash = NULL;
   setFlags[0] = setFlags[1] = setFlags[2] = setFlags[3] = 0;
   usedFlags[0] = usedFlags[1] = usedFlags[2] = usedFlags[3] = 0;
}


static int _flg_user( const void *key, const void *datum, void *arg )
{
   FILE     *stream = (FILE *)arg;
   
   fprintf( stream, "  %s\n", (char *)key );
   return 0;
}

/* |flg_list| lists all of the valid flags to the specified |stream|. */

void flg_list( FILE *stream )
{
   hsh_iterate_arg( hash, _flg_user, stream );
}
