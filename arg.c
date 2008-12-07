/* arg.c -- Argument list support
 * Created: Sun Jan  7 13:39:29 1996 by faith@dict.org
 * Copyright 1996-1997, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
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
 * \section{Argument List Routines}
 *
 * \intro Argument lists are vectors of pointers to strings (e.g., the
 * standard |char **argv|).  These routines manage the efficient creation,
 * manipulation, and deletion of these sorts of lists.
 *
 */

#include "maaP.h"

typedef struct Arg {
#if MAA_MAGIC
   unsigned   magic;
#endif
   int        argc;		/* Current count */
   int        argm;		/* Maximum count */
   char       **argv;		/* Vector */
   mem_String object;
} *Arg;

static void _arg_check( arg_List arg, const char *function )
{
   Arg a = (Arg)arg;
   
   if (!a) err_internal( function, "arg is null\n" );
#if MAA_MAGIC
   if (a->magic != ARG_MAGIC)
      err_internal( function,
		    "Magic match failed: 0x%08x (should be 0x%08x)\n",
		    a->magic,
		    ARG_MAGIC );
#endif
}   

/* \doc Create an |arg_List| object. */

arg_List arg_create( void )
{
   Arg a = xmalloc( sizeof( struct Arg ) );

#if MAA_MAGIC
   a->magic   = ARG_MAGIC;
#endif
   a->argc    = 0;
   a->argm    = 2;
   a->argv    = xmalloc( a->argm * sizeof( char ** ) );
   a->argv[0] = NULL;
   a->object  = mem_create_strings();

   return a;
}

/* \doc Free all memory associated with |arg|, including the memory used
   for the strings. */

void arg_destroy( arg_List arg )
{
   Arg a = (Arg)arg;
   
   _arg_check( a, __func__ );
   mem_destroy_strings( a->object );
   xfree( a->argv );
#if MAA_MAGIC
   a->magic = ARG_MAGIC_FREED;
#endif
   xfree( a );
}

/* \doc Add |string| as the next item in |arg|. */

arg_List arg_add( arg_List arg, const char *string )
{
   Arg        a = (Arg)arg;
   const char *new;
   
   _arg_check( a, __func__ );
   new = mem_strcpy( a->object, string );
   if (a->argm <= a->argc + 2 )
      a->argv = xrealloc( a->argv, sizeof( char **) * (a->argm *= 2) );
   
   a->argv[a->argc++] = (char *)new; /* discard const */
   a->argv[a->argc]   = NULL;

   return a;
}

/* \doc Add |length| characters of |string| as the next item in |arg|.  A
   terminating "NULL" is added to the copied |string|. */

arg_List arg_addn( arg_List arg, const char *string, int length )
{
   Arg        a = (Arg)arg;
   const char *new;
   
   _arg_check( a, __func__ );
   new = mem_strncpy( a->object, string, length );
   if (a->argm <= a->argc + 2 )
      a->argv = xrealloc( a->argv, sizeof( char **) * (a->argm *= 2) );
   
   a->argv[a->argc++] = (char *)new; /* discard const */
   a->argv[a->argc]   = NULL;

   return a;
}

/* \doc Grow the next item of |arg| with |length| characters of |string|.
   Several calls to |arg_grow| should be followed by a single call to
   |arg_finish| without any intervening calls to other functions which
   modify |arg|. */

void arg_grow( arg_List arg, const char *string, int length )
{
   Arg        a = (Arg)arg;

   _arg_check( a, __func__ );
   mem_grow( a->object, string, length );
}

/* \doc Finish the growth of the next item in |arg| started by
   |arg_grow|. */

arg_List arg_finish( arg_List arg )
{
   Arg        a = (Arg)arg;
   const char *new;
   
   _arg_check( a, __func__ );
   new = mem_finish( a->object );
   if (a->argm <= a->argc + 2 )
      a->argv = xrealloc( a->argv, sizeof( char **) * (a->argm *= 2) );
   
   a->argv[a->argc++] = (char *)new; /* discard const */
   a->argv[a->argc]   = NULL;

   return a;
}

/* \doc Return |item| from |arg|.  |arg| is 0-based. */

const char *arg_get( arg_List arg, int item ) /* FIXME! inline? */
{
   Arg a = (Arg)arg;
   
   _arg_check( a, __func__ );
   if (item < 0 || item >= a->argc)
      err_internal( __func__,
		    "Request for item %d in list containing %d items\n",
		    item,
		    a->argc );
   return a->argv[ item ];
}

/* \doc Return the number of items in |arg|. */

int arg_count( arg_List arg )	/* FIXME! inline? */
{
   _arg_check( arg, __func__ );
   return ((Arg)arg)->argc;
}

/* \doc Get an |argc| and |argv| from |arg|.  These are suitable for use in
   calls to |exec|.  The |argc|+1 item in |argv| is "NULL". */

void arg_get_vector( arg_List arg, int *argc, char ***argv )
{
   Arg        a = (Arg)arg;

   _arg_check( a, __func__ );
   *argc = a->argc;
   *argv = a->argv;
}

/* \doc Break up |string| into arguments, placing them as items in |arg|.
   Items within single or double quotes may contain spaces.  The quotes are
   stripped as in shell argument processing.
   Backslash followed by <char> outside quoted or
   double-quote subtokens is treated just as <char> */

#include "arggram.c"

#if ACTIONS_COUNT != 3
#error Modify arg_argify function or change arggram.txt
#endif

#if CHARTYPES_COUNT != 6
#error Modify char2char_type function or change arggram.txt
#endif

static int char2char_type (int quoteStyle, char ch)
{
   switch (ch){
   case '"':
      if (ARG_NO_QUOTE & quoteStyle)
	 return CHARTYPE_OTHER;
      else
	 return CHARTYPE_DQ;
   case '\'':
      if (ARG_NO_QUOTE & quoteStyle)
	 return CHARTYPE_OTHER;
      else
	 return CHARTYPE_SQ;
   case '\\':
      if (ARG_NO_ESCAPE & quoteStyle)
	 return CHARTYPE_OTHER;
      else
	 return CHARTYPE_BS;
   case ' ':
   case '\t':
   case '\r':
   case '\v':
   case '\n':
      return CHARTYPE_SPACE;
   case '\0':
      return CHARTYPE_NULL;
   default:
      return CHARTYPE_OTHER;
   }
}


arg_List arg_argify( const char *string, int quoteStyle )
{
   Arg        a = arg_create();
   const char *last = NULL;
   const char *pt = string;
   char ch;
   int ch_type;

   int state = 0;
   int curr_act = -1;

   do {
      ch = *pt;
      ch_type = char2char_type (quoteStyle, ch);

      curr_act = action [state] [ch_type];

/*      fprintf (stderr, "%i -- %i(%c) / %i --> %i\n", state, ch_type, ch, curr_act, transition [state] [ch_type]);*/

      state = transition [state] [ch_type];

      switch (curr_act){
      case ACTION_INCLUDE:
	 if (!last)
	    last = pt;

	 break;
      case ACTION_SKIP:
	 if (last){
	    arg_grow (a, last, pt - last);
	 }
	 last = pt + 1;

	 break;
      case ACTION_FINISH:
/*	 assert (last);*/
	 if (last){
	    arg_grow (a, last, pt - last);
	    arg_finish (a);
	    last = NULL;
	 }

	 break;
      default:
	 abort ();
      }

      ++pt;
   }while (ch && state >= 0);

   switch (state){
   case -1:
      /* Fine! Normal exit*/
      break;
   case -2:
      /* Parsing error */
      break;
   default:
      /* Oooops! */
      err_internal( __func__, "arg.c:arg_argify is buggy!!!:\n");
   }

   return a;
}
