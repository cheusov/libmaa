/* kh.c -- General Support for Khepera
 * Created: Fri Jan 20 14:56:18 1995 by faith@cs.unc.edu
 * Revised: Wed Aug  9 13:49:55 1995 by r.faith@ieee.org
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
 * $Id: kh.c,v 1.2 1995/08/24 14:59:12 faith Exp $
 *
 * \section{General Support}
 *
 */

#include "kh.h"

/* \doc |kh_init| should be called at the start of "main()", and serves to
   initialize debugging and other support for \khepera. */

void kh_init( void )
{
   _dbg_register( KH_FAREY,     "KHfarey" );
   _dbg_register( KH_TRE,       "KHtre" );
   _dbg_register( KH_SRC,       "KHsrc" );
   _dbg_register( KH_PP,        "KHpp" );
   _dbg_register( KH_PPVERBOSE, "KHppverbose" );
   _dbg_register( KH_LINE,      "KHline" );
   _dbg_register( KH_MEMORY,    "KHmemory" );

#ifndef __CHECKER__
#ifdef HAVE_ATEXIT
      atexit( kh_shutdown );
#else
# ifdef HAVE_ON_EXIT
      on_exit( kh_shutdown, NULL );
# endif
#endif
#endif
}

void kh_shutdown( void )
{
   if (dbg_test(KH_MEMORY)) {
      str_print_stats( stderr );
      src_print_stats( stderr );
      tre_print_stats( stderr );
   }
   
   dbg_destroy();
   src_destroy();
   str_destroy();
   _tre_shutdown();
   _lst_shutdown();
}
