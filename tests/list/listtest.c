/* listtest.c -- 
 * Created: Wed Aug  9 11:36:09 1995 by faith@dict.org
 * Copyright 1995, 1996, 2002 Rickard E. Faith (faith@dict.org)
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

static int print( const void *datum )
{
	printf( "%s ", (char*) __UNCONST(datum) );
	return 0;
}

int main( int argc, char **argv )
{
	lst_List     list = lst_create();
	lst_Position p;
	char         *e;
	long         i;

	maa_init( argv[0] );

	lst_append( list, "1" );
	lst_iterate( list, print ); printf( "\n" );

	lst_append( list, "2" );
	lst_append( list, "3" );
	lst_iterate( list, print ); printf( "\n" );
   
	lst_push( list, "0" );
	lst_iterate( list, print ); printf( "\n" );
	printf( "Length = %d (expect 4)\n", lst_length( list ) );

	LST_ITERATE(list,p,e) {
		printf( "%s ", e );
	}
	printf( "\n" );

	lst_pop( list );
	lst_iterate( list, print ); printf( "\n" );
	printf( "Length = %d (expect 3)\n", lst_length( list ) );

	lst_truncate( list, 1 );
	lst_iterate( list, print ); printf( "\n" );
	printf( "Length = %d (expect 1)\n", lst_length( list ) );

	for (i = 0; i < 10000; i++) lst_push( list, (void *)i );
   
	lst_destroy( list );
	return 0;
}
