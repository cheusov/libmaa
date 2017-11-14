/* debugtest.c -- 
 * Created: Sun Dec 25 18:57:38 1994 by faith@dict.org
 * Copyright 1994, 1996, 2002 Rickard E. Faith (faith@dict.org)
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
#include <errno.h>

#define DBG_VERBOSE 0x00000001
#define DBG_TEST    0x00000002

int main( int argc, char **argv )
{
   int c;

   maa_init( argv[0] );

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
   else                         printf( "Verbose not set\n" );
   if (dbg_test( DBG_TEST )) printf( "Test set\n" );
   else                      printf( "Test not set\n" );

   return 0;
}
