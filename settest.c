/* settest.c -- Test program for Khepera set routines
 * Created: Wed Nov  9 15:04:25 1994 by faith@cs.unc.edu
 * Revised: Sun Nov 19 13:30:20 1995 by faith@cs.unc.edu
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
 * $Id: settest.c,v 1.4 1995/11/19 19:28:49 faith Exp $
 */

#include "maaP.h"
#include <math.h>

static void init_rand( void )
{
   srandom(1);
}

static int get_rand( int ll, int ul )
{
   double r = ((double)random()) / ((double)RAND_MAX);

   return floor( ll + r * (ul - ll) );
}

static int iterator( const void *key )
{
   printf( "%s\n", (const char *)key );
   return 0;
}

static int freer( const void *key )
{
   xfree( (void *)key );
   return 0;
}

int main( int argc, char **argv )
{
   set_Set      t;
   set_Set      t1;
   set_Set      t2;
   int          i;
   int          j;
   int          count;
   set_Position p;
   const void   *k;

   if (argc == 1) {
      count = 100;
   } else if (argc != 2 ) {
      fprintf( stderr, "usage: settest count\n" );
      return 1;
   } else {
      count = atoi( argv[1] );
   }

   printf( "Running test for count of %d\n", count );

				/* Test sequential keys */
   t = set_create( NULL, NULL );
   
   for (i = 0; i < count; i++) {
      char *key   = xmalloc( 20 );

      sprintf( key, "key%d", i );
      set_insert( t, key );
   }

   for (i = count; i >= 0; i--) {
      char        key[100];

      sprintf( key, "key%d", i );
      if (!set_member( t, key ))
	    printf( "\"%s\" is not a member of the set", key );
   }

   if (count <= 200) set_iterate( t, iterator );
   
   set_print_stats( t, stdout );

   set_iterate( t, freer );
   set_destroy( t );



				/* Test random keys */
   t = set_create( NULL, NULL );

   init_rand();
   for (i = 0; i < count; i++) {
      int  len = get_rand( 2, 32 );
      char *key   = xmalloc( len + 1 );

      for (j = 0; j < len; j++) key[j] = get_rand( 32, 128 );
      key[ len ] = '\0';
      set_insert( t, key );
   }

   init_rand();
   for (i = 0; i < count; i++) {
      int         len = get_rand( 2, 32 );
      char       *key = xmalloc( len + 1 );

      for (j = 0; j < len; j++) key[j] = get_rand( 32, 128 );
      key[ len ] = '\0';
      if (!set_member( t, key ))
	    printf( "\"%s\" is not a member of the set", key );

      xfree( key );
   }
   
   set_print_stats( t, stdout );

   set_iterate( t, freer );
   set_destroy( t );



				/* Test (random) integer keys */
   t = set_create( hsh_pointer_hash, hsh_pointer_compare );

   init_rand();
   for (i = 0; i < count; i++) {
      int  key    = get_rand( 1, 16777216 );

      set_insert( t, (void *)key );
   }

   init_rand();
   for (i = 0; i < count; i++) {
      int         key = get_rand( 1, 16777216 );

      if (!set_member( t, (void *)key ))
	    printf( "%d is not a member of the set", key );
   }
   
   set_print_stats( t, stdout );

   set_destroy( t );

				/* Test set operations */

   t1 = set_create( NULL, NULL );
   t2 = set_create( NULL, NULL );

   set_insert( t1, "foo" );
   set_insert( t1, "bar" );

   set_insert( t2, "t2-foo" );
   set_insert( t2, "bar" );
   
   printf( "\nSet 1:\n" );
   set_iterate( t1, iterator );

   
   printf( "\nSet 1 (again):\n" );
   SET_ITERATE(t1,p,k) printf( "%s\n", (const char *)k );

   printf( "\nSet 2:\n" );
   set_iterate( t2, iterator );

   printf( "Set 1 == Set 2 ? ===> %d\n", set_equal( t1, t2 ) );
   printf( "Set 1 == Set 1 ? ===> %d\n", set_equal( t1, t1 ) );

   printf( "\nUnion:\n");
   t = set_union( t1, t2 );
   set_iterate( t, iterator );
   set_destroy( t );
   
   printf( "\nIntersection:\n");
   t = set_inter( t1, t2 );
   set_iterate( t, iterator );
   set_destroy( t );
   
   printf( "\nDifference:\n");
   t = set_diff( t1, t2 );
   set_iterate( t, iterator );
   set_destroy( t );

   set_destroy( t1 );
   set_destroy( t2 );

   return 0;
}
