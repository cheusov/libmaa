/* hashtest.c -- Test program for Khepera hash table routines
 * Created: Sun Nov  6 18:55:23 1994 by faith@dict.org
 * Copyright 1994, 1995, 2002 Rickard E. Faith (faith@dict.org)
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
 */

#include "maaP.h"

extern void init_rand( void );
extern int get_rand( int ll, int ul );

static int iterator( const void *key, const void *datum )
{
   printf( "%s: %s\n", (const char *)key, (const char *)datum );
   return 0;
}

static int freer( const void *key, const void *datum )
{
   xfree( __UNCONST(datum) );
   xfree( __UNCONST(key) );
   return 0;
}

static int free_data( const void *key, const void *datum )
{
   xfree( __UNCONST(datum) );
   return 0;
}

int main( int argc, char **argv )
{
   hsh_HashTable t;
   int           i;
   int           j;
   int           count;

   if (argc == 1) {
      count = 100;
   } else if (argc != 2 ) {
      fprintf( stderr, "usage: hashtest count\n" );
      return 1;
   } else {
      count = atoi( argv[1] );
   }

   printf( "Running test for count of %d\n", count );

				/* Test sequential keys */
   t = hsh_create( NULL, NULL );
   
   for (i = 0; i < count; i++) {
      char *key   = xmalloc( 20 );
      char *datum = xmalloc( 20 );

      sprintf( key, "key%d", i );
      sprintf( datum, "datum%d", i );
      hsh_insert( t, key, datum );
   }

   for (i = count; i >= 0; i--) {
      char        key[100];
      char        datum[100];
      const char *pt;

      sprintf( key, "key%d", i );
      sprintf( datum, "datum%d", i );
      pt = hsh_retrieve( t, key );

      if (!pt || strcmp( pt, datum )) {
	 printf( "Expected \"%s\", got \"%s\"\n", datum, pt ? pt : "(null)" );
      }
   }

   if (count <= 200) hsh_iterate( t, iterator );
   
/*   hsh_print_stats( t, stdout );*/

   hsh_iterate( t, freer );
   hsh_destroy( t );



				/* Test random keys */
   t = hsh_create( NULL, NULL );

   init_rand();
   for (i = 0; i < count; i++) {
      int  len = get_rand( 2, 32 );
      char *key   = xmalloc( len + 1 );
      char *datum = xmalloc( 20 );

      for (j = 0; j < len; j++) key[j] = get_rand( 32, 128 );
      key[ len ] = '\0';
      sprintf( datum, "datum%d", i );
      hsh_insert( t, key, datum );
   }
   
   init_rand();
   for (i = 0; i < count; i++) {
      int         len = get_rand( 2, 32 );
      char       *key = xmalloc( len + 1 );
      char        datum[100];
      const char *pt;

      for (j = 0; j < len; j++) key[j] = get_rand( 32, 128 );
      key[ len ] = '\0';
      sprintf( datum, "datum%d", i );
      pt = hsh_retrieve( t, key );

      if (!pt || strcmp( pt, datum ))
	    printf( "Expected \"%s\", got \"%s\" for key \"%s\"\n",
		    datum, pt, key );

      xfree( key );
   }
   
/*   hsh_print_stats( t, stdout );*/

   hsh_iterate( t, freer );
   hsh_destroy( t );



				/* Test (random) integer keys */
   t = hsh_create( hsh_pointer_hash, hsh_pointer_compare );

   init_rand();
   for (i = 0; i < count; i++) {
      long key    = get_rand( 1, 16777216 );
      char *datum = xmalloc( 20 );

      sprintf( datum, "datum%d", i );
      hsh_insert( t, (void *)key, datum );
   }

   init_rand();
   for (i = 0; i < count; i++) {
      long        key = get_rand( 1, 16777216 );
      char        datum[100];
      const char *pt;

      sprintf( datum, "datum%d", i );
      pt = hsh_retrieve( t, (void *)key );

      if (!pt || strcmp( pt, datum ))
	    printf( "Expected \"%s\", got \"%s\" for key %ld\n",
		    datum, pt, key );
   }
   
/*   hsh_print_stats( t, stdout );*/

   hsh_iterate( t, free_data );
   hsh_destroy( t );

   return 0;
}
