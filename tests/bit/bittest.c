/* bittest.c -- Test the bit functions, especially the counter
 * Created: Mon Oct  2 10:10:57 1995 by faith@dict.org
 * Copyright 1995, 2002 Rickard E. Faith (faith@dict.org)
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

int main( int argc, char **argv )
{
   unsigned long t = 0;

   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );

   bit_set( &t, 0 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 1 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 2 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 3 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 4 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 5 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 6 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 7 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 31 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 30 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 29 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 28 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_set( &t, 27 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );

   printf( "0x%08lx has %d bit set: %s\n", t, 7, bit_tst( &t, 7 ) ? "yes" : "no" );
   printf( "0x%08lx has %d bit set: %s\n", t, 8, bit_tst( &t, 8 ) ? "yes" : "no" );
   printf( "0x%08lx has %d bit set: %s\n", t, 9, bit_tst( &t, 9 ) ? "yes" : "no" );

   printf( "0x%08lx has %d bit set: %s\n", t, 27, bit_tst( &t, 27 ) ? "yes" : "no" );
   printf( "0x%08lx has %d bit set: %s\n", t, 28, bit_tst( &t, 28 ) ? "yes" : "no" );
   printf( "0x%08lx has %d bit set: %s\n", t, 29, bit_tst( &t, 29 ) ? "yes" : "no" );

   printf( "\n" );
   bit_clr( &t, 0 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_clr( &t, 31 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_clr( &t, 1 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_clr( &t, 4 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   bit_clr( &t, 7 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );
   
   printf( "\n" );
   bit_clr( &t, 16 );
   printf( "0x%08lx has %d bits set\n", t, bit_cnt( &t ) );

   printf( "0x%08lx has %d bit set: %s\n", t, 14, bit_tst( &t, 14 ) ? "yes" : "no" );
   printf( "0x%08lx has %d bit set: %s\n", t, 15, bit_tst( &t, 15 ) ? "yes" : "no" );
   printf( "0x%08lx has %d bit set: %s\n", t, 16, bit_tst( &t, 16 ) ? "yes" : "no" );

   return 0;
}
