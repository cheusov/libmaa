/* debugtest.c -- 
 * Created: Sun Dec 25 18:57:38 1994 by faith@cs.unc.edu
 * Revised: Wed Aug  9 09:47:23 1995 by r.faith@ieee.org
 * Copyright 1994 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: debugtest.c,v 1.2 1995/08/24 14:59:08 faith Exp $
 */

#include "kh.h"
#include <errno.h>

#define DBG_VERBOSE 0x00000001
#define DBG_TEST    0x00000002

int main( int argc, char **argv )
{
   int c;

   kh_init();

   dbg_register( DBG_VERBOSE, "verbose" );
   dbg_register( DBG_TEST, "test" );

   while ((c = getopt( argc, argv, "d:" )) != -1) {
      switch (c) {
      case 'd':
	 dbg_set( optarg );
	 break;
      default:
	 fprintf( stderr, "Usage: debugtest [-dverbose] [-dtest]\n" );
	 break;
      }
   }

   if (dbg_test( DBG_VERBOSE )) printf( "Verbose set\n" );
   if (dbg_test( DBG_TEST )) printf( "Test set\n" );

   return 0;
}
