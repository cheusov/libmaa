/* prtest.c -- 
 * Created: Fri Jan 12 14:18:32 1996 by r.faith@ieee.org
 * Revised: Sun Jan 14 22:10:31 1996 by r.faith@ieee.org
 * Copyright 1996 Rickard E. Faith (r.faith@ieee.org)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
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
 * $Id: prtest.c,v 1.1 1996/01/15 03:48:07 faith Exp $
 * 
 */

#include "maaP.h"

int main( int argc, char **argv )
{
   FILE *instr, *outstr;
   char buf[100];
   
   maa_init( argv[0] );
   dbg_set( "pr" );

   printf( "%s\n", maa_version() );

   pr_open( "echo foobar", PR_USE_STDIN | PR_CREATE_STDOUT,
	    NULL, &instr, NULL );
   
   pr_open( "cat", PR_USE_STDIN | PR_CREATE_STDOUT,
	    &instr, &outstr, NULL );
   fgets( buf, sizeof( buf ), outstr );
   printf( "Got \"%s\"\n", buf );

   printf( "status = 0x%02x\n", pr_close( outstr ) );

   pr_open( "cat", PR_CREATE_STDIN | PR_CREATE_STDOUT,
	    &instr, &outstr, NULL );
   fprintf( instr, "this is a test\n" );
   fflush( instr );
   fgets( buf, sizeof( buf ), outstr );
   printf( "Got \"%s\"\n", buf );
   printf( "status = 0x%02x\n", pr_close( instr ) );
   printf( "status = 0x%02x\n", pr_close( outstr ) );

   return 0;
}
