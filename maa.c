/* maa.c -- General Support for libmaa
 * Created: Sun Nov 19 13:24:35 1995 by faith@cs.unc.edu
 * Revised: Sun Nov 19 13:34:19 1995 by faith@cs.unc.edu
 * Copyright 1995 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: maa.c,v 1.1 1995/11/19 19:28:44 faith Exp $
 *
 * \section{General Support}
 *
 */

#include "maaP.h"
#include "maa.h"

/* \doc |maa_init| should be called at the start of "main()", and serves to
   initialize debugging and other support for \maa. */

void maa_init( void )
{
   tim_start( "total" );
   _dbg_register( KH_FAREY,     "farey" );
   _dbg_register( KH_TRE,       "tre" );
   _dbg_register( KH_SRC,       "src" );
   _dbg_register( KH_PP,        "pp" );
   _dbg_register( KH_PPVERBOSE, "ppverbose" );
   _dbg_register( KH_LINE,      "line" );
   _dbg_register( KH_MEMORY,    "memory" );
   _dbg_register( KH_TIME,      "time" );
   _dbg_register( KH_RULES,     "rules" );
   _dbg_register( KH_REPLACE,   "replace" );
   _dbg_register( KH_SCOPE,     "scope" );
   _dbg_register( KH_ENUM,      "enum" );
   _dbg_register( KH_TYPE,      "type" );
   _dbg_register( KH_INFER,     "infer" );
   _dbg_register( KH_TREES,     "trees" );
   _dbg_register( KH_DUMP,      "dump" );
   _dbg_register( KH_SLOW,      "slow" );

#ifndef __CHECKER__
#ifdef HAVE_ATEXIT
      atexit( maa_shutdown );
#else
# ifdef HAVE_ON_EXIT
      on_exit( maa_shutdown, NULL );
# endif
#endif
#endif
}

void maa_shutdown( void )
{
   if (dbg_test(KH_MEMORY)) {
      str_print_stats( stderr );
   }

   str_destroy();
   _lst_shutdown();

   tim_stop( "total" );
   if (dbg_test(KH_TIME)) {
      tim_print_timers( stderr );
   }
   _tim_shutdown();
   dbg_destroy();
}
