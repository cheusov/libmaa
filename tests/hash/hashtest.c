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

#include <stdlib.h>
#include <string.h>
#include <maa.h>

extern void init_rand( void );
extern int get_rand( int ll, int ul );

static char *get_static_key(int key)
{
   static char ret[20];
   snprintf(ret, sizeof(ret), "key%d", key);
   return xstrdup(ret);
}

static char *get_static_datum(int key)
{
   static char ret[20];
   snprintf(ret, sizeof(ret), "datum%d", key);
   return xstrdup(ret);
}

static char *get_key(int key)
{
   char ret[20];
   snprintf(ret, sizeof(ret), "key%d", key);
   return xstrdup(ret);
}

static char *get_datum(int datum)
{
   char ret[20];
   snprintf(ret, sizeof(ret), "datum%d", datum);
   return xstrdup(ret);
}

static int iterator( const void *key, const void *datum )
{
   printf( "%s: %s\n", (const char *)key, (const char *)datum );
   return 0;
}

static int iterator_arg( const void *key, const void *datum, void *arg )
{
   if (arg) {
      printf( "%s: %s (%u)\n", (const char *)key, (const char *)datum,
	      (unsigned)((char *)arg - (char *)NULL));
      return strchr((const char *)datum, '5') != NULL;
   } else {
      printf( "%s: %s\n", (const char *)key, (const char *)datum);
      return 0;
   }
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

static void test_hsh_strings(int count)
{
   hsh_HashTable t;
   int           i;
   int           j;

				/* Test sequential keys */
   t = hsh_create( NULL, NULL );
   
   for (i = 0; i < count; i++) {
      char *key   = get_key(i);
      char *datum = get_datum(i);

      hsh_insert( t, key, datum );
   }

   for (i = count + 1; i >= -2; i--) {
      char *key = get_static_key(i);
      char *datum = get_static_datum(i);
      const char *pt;

      pt = hsh_retrieve( t, key );

      if (!pt || strcmp( pt, datum )) {
	 printf( "Expected \"%s\", got \"%s\"\n", datum, pt ? pt : "(null)" );
      }
   }

   /* Iterating */
   printf( "Iteration 1\n");
   if (count <= 200)
      hsh_iterate( t, iterator );

   /* Delete items 60-70 */
   for (i = 60; i <= 70; ++i) {
      hsh_delete( t, get_static_key(i));
   }

   /* Iterating */
   printf( "Iteration 2\n");
   if (count <= 200) {
      int ret = hsh_iterate_arg( t, iterator_arg, NULL);
      printf("hsh_iterate_arg returned %d\n", ret);
   }

   /* Partial iterating */
   printf( "Iteration 3\n");
   if (count <= 200) {
      int ret = hsh_iterate_arg( t, iterator_arg, (char *)NULL + 7 );
      printf("hsh_iterate_arg returned %d\n", ret);
   }

   /*   hsh_print_stats( t, stdout );*/

   /* Deleting everything */
   hsh_iterate( t, freer );
   hsh_destroy( t );

				/* Test random keys */
   t = hsh_create( NULL, NULL );

   init_rand();
   for (i = 0; i < count; i++) {
      int  len = get_rand( 2, 32 );
      char *key   = xmalloc( len + 1 );
      char *datum = get_datum(i);

      for (j = 0; j < len; j++) key[j] = get_rand( 32, 128 );
      key[ len ] = '\0';
      hsh_insert( t, key, datum );
   }

   init_rand();
   for (i = 0; i < count; i++) {
      int         len = get_rand( 2, 32 );
      char       *key = xmalloc( len + 1 );
      char       *datum = get_static_datum(i);
      const char *pt;

      for (j = 0; j < len; j++)
	 key[j] = get_rand( 32, 128 );

      key[ len ] = '\0';
      pt = hsh_retrieve( t, key );

      if (!pt || strcmp( pt, datum ))
	    printf( "Expected \"%s\", got \"%s\" for key \"%s\"\n",
		    datum, pt, key );

      xfree( key );
   }
   
/*   hsh_print_stats( t, stdout );*/

   hsh_iterate( t, freer );
   hsh_destroy( t );
}

static void test_hsh_integers(int count)
{
   hsh_HashTable t;
   int           i;

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
}

static void test_hsh_pointer_compare(void)
{
   /* hsh_pointer_compare */
   void *p1 = (void *)((char*)NULL + 10);
   void *p2 = (void *)((char*)NULL + 20);
   printf("=== hsh_pointer_compare ===\n");
   printf("p1 vs. p2: %d\n", hsh_pointer_compare(p1, p2));
   printf("p2 vs. p1: %d\n", hsh_pointer_compare(p2, p1));
   printf("p1 vs. p1: %d\n", hsh_pointer_compare(p1, p1));
}

int main( int argc, char **argv )
{
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

   test_hsh_strings(count);
   test_hsh_integers(count);
   test_hsh_pointer_compare();

   return 0;
}
