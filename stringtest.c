/* stringtest.c -- Test program for Khepera string table routines
 * Created: Thu Dec 22 14:05:57 1994 by faith@cs.unc.edu
 * Revised: Thu Dec 22 14:29:39 1994 by faith@cs.unc.edu
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
 * $Id: stringtest.c,v 1.1 1995/04/21 15:31:47 faith Exp $
 */

#include "kh.h"
#include <math.h>

int main( int argc, char **argv )
{
   int           i;
   int           count;
   const char    **orig;
   char          buf[100];

   if (argc == 1) {
      count = 100;
   } else if (argc != 2 ) {
      fprintf( stderr, "usage: stringtest count\n" );
      return 1;
   } else {
      count = atoi( argv[1] );
   }

   orig = xmalloc( count * sizeof( const char ** ) );

   printf( "Running test for count of %d\n", count );

   for (i = 0; i < count; i++) {
      sprintf( buf, "key%d", i );
      orig[i] = str_find( buf );
   }

   for (i = 0; i < count; i++) {
      sprintf( buf, "key%d", i );
      if (orig[i] != str_find( buf ))
	    printf( "Pointers are different for \"%s\": %p and %p\n",
		     buf,
		     orig[i],
		     str_find( buf ) );
   }

   xfree( orig );

   printf( "Done.\n" );

   return 0;
}
