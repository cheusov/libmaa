/* sltest.c -- 
 * Created: Mon Feb 19 08:57:34 1996 by faith@dict.org
 * Copyright 1996, 2002 Rickard E. Faith (faith@dict.org)
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
#include <math.h>

#undef DUMP

static int compare( const void *datum1, const void *datum2 )
{
   long a = (long)datum1;
   long b = (long)datum2;

   if (a < b) return -1;
   if (a > b) return 1;
   return 0;
}

static int print( const void *datum )
{
   printf ("%li ", (long) datum);
   return 0;
}

static const void *key( const void *datum )
{
   return datum;
}

int main( int argc, char **argv )
{
   sl_List       sl;
   int           count;
   int           i;

   maa_init( argv[0] );
   
   if (argc == 1) {
      count = 10;
   } else if (argc != 2 ) {
      fprintf( stderr, "usage: sltest count\n" );
      return 1;
   } else {
      count = atoi( argv[1] );
   }

   printf( "Running test for count of %d\n", count );

   sl = sl_create( compare, key, NULL );
   
   for (i = 1; i < count; i++) {
      printf( "adding %d\n", i );
      sl_insert( sl, (void *) (intptr_t) i );
#ifdef DUMP
      _sl_dump( sl );
#endif
   }

   sl_iterate( sl, print );
   printf( "\n" );

   sl_delete( sl, (void *)5 );
   sl_iterate( sl, print );
   printf( "\n" );

   sl_insert( sl, (void *)0 );
   sl_iterate( sl, print );
   printf( "\n" );
   sl_insert( sl, (void *)66 );
   sl_iterate( sl, print );
   printf( "\n" );
   sl_insert( sl, (void *)100 );
   sl_iterate( sl, print );
   printf( "\n" );
   sl_insert( sl, (void *)-1 );
   sl_iterate( sl, print );
   printf( "\n" );
   sl_insert( sl, (void *)5 );
   sl_iterate( sl, print );
   printf( "\n" );
   sl_insert( sl, (void *)67 );
   sl_iterate( sl, print );
   printf( "\n" );
   sl_insert( sl, (void *)68 );
   sl_iterate( sl,print );
   printf( "\n" );
   sl_insert( sl, (void *)65 );
   sl_iterate( sl, print );
   printf( "\n" );
   
   sl_destroy( sl );

   return 0;
}
