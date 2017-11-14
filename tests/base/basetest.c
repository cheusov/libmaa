/* basetest.c -- Test base64 and base26 numbers
 * Created: Sun Nov 10 11:51:11 1996 by faith@dict.org
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

int main( int argc, char **argv )
{
   long int   i;
   const char *result;
   long int   limit = 0xffff;

   if (argc == 2) limit = strtol( argv[1], NULL, 0 );
   
   for (i = 0; i < limit; i++) {
      result = b26_encode( i );
      if (i != b26_decode( result )) {
	 printf( "%s => %ld != %ld\n", result, b26_decode( result ), i );
      }
      if (i < 100) {
	 result = b26_encode( 0 );
	 if (0 != b26_decode( result )) {
	    printf( "%s => %ld != %ld (cache problem)\n",
		    result, b26_decode( result ), 0L );
	 }
	 result = b26_encode( i );
	 if (i != b26_decode( result )) {
	    printf( "%s => %ld != %ld (cache problem)\n",
		    result, b64_decode( result ), i );
	 }
      }
      if (i < 10 || !(i % (limit/10)))
	 printf( "%ld = %s (base26)\n", i, result );
   }

   for (i = 0; i < limit; i++) {
      result = b64_encode( i );
      if (i != b64_decode( result )) {
	 printf( "%s => %ld != %ld\n", result, b64_decode( result ), i );
      }
      if (i < 100) {
	 result = b64_encode( 0 );
	 if (0 != b64_decode( result )) {
	    printf( "%s => %ld != %ld (cache problem)\n",
		    result, b64_decode( result ), 0L );
	 }
	 result = b64_encode( i );
	 if (i != b64_decode( result )) {
	    printf( "%s => %ld != %ld (cache problem)\n",
		    result, b64_decode( result ), i );
	 }
      }
      if (i < 10 || !(i % (limit/10)))
	 printf( "%ld = %s (base64)\n", i, result );
   }

   return 0;
}
