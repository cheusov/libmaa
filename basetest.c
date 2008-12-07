/* basetest.c -- Test base64 and base26 numbers
 * Created: Sun Nov 10 11:51:11 1996 by faith@dict.org
 * Copyright 1996, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
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
 * $Id: basetest.c,v 1.4 2008/12/07 14:53:02 cheusov Exp $
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
