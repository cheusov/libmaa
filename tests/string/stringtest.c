/* stringtest.c -- Test program for Khepera string table routines
 * Created: Thu Dec 22 14:05:57 1994 by faith@dict.org
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
#include <math.h>

int main( int argc, char **argv )
{
	int           i;
	int           count;
	const char    **orig;
	char          buf[100];

	maa_init( argv[0] );

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
		const char *this;

		sprintf( buf, "key%d", i );
		this = str_find( buf );
		if (orig[i] != this)
			printf( "Pointers are different for \"%s\" (\"%s\"): %p and %p\n",
					buf,
					this,
					orig[i],
					this );
	}

	xfree( orig );

	printf( "Done.\n" );

	return 0;
}
