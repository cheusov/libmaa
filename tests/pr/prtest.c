/* prtest.c -- 
 * Created: Fri Jan 12 14:18:32 1996 by faith@dict.org
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
	int  fdin = 0;
	int  fdout;
	char buf[BUFSIZ];
	int  c;
	int  i;
	ssize_t cnt = 0;

	maa_init( argv[0] );

	while ((c = getopt( argc, argv, "D" )) != EOF)
		switch (c) {
			case 'D': dbg_set( ".pr" ); break;
		}

	if (argc-optind == 0) {
		pr_open( "echo foo", PR_USE_STDIN | PR_CREATE_STDOUT,
				 &fdin, &fdout, NULL );
	} else {
		for (i = optind; i < argc; i++) {
			pr_open( argv[i], PR_USE_STDIN | PR_CREATE_STDOUT,
					 &fdin, &fdout, NULL );
			fdin = fdout;
		}
	}

	printf ("Got:");
	while (cnt = read (fdout, buf, BUFSIZ-1), cnt > 0){
		buf [cnt] = 0;
		printf( " \"%s\"\n", buf );
	}
	printf( "status = 0x%02x\n", pr_close (fdout) );

#if 0
	printf( "%s\n", maa_version() );

	pr_open( "echo foobar", PR_USE_STDIN | PR_CREATE_STDOUT,
			 NULL, &instr, NULL );
   
	pr_open( "cat", PR_USE_STDIN | PR_CREATE_STDOUT,
			 &instr, &outstr, NULL );
	fgets( buf, sizeof( buf ), outstr );
	printf( "Got \"%s\"\n", buf );

	printf( "status = 0x%02x\n", pr_close( outstr ) );

	pr_open( "cat", PR_CREATE_STDIN | PR_CREATE_STDOUT,
			 &instr, &outstr, NULL );
	fprintf( instr, "this is a test\n" );
	fflush( instr );
	fgets( buf, sizeof( buf ), outstr );
	printf( "Got \"%s\"\n", buf );
	printf( "status = 0x%02x\n", pr_close( instr ) );
	printf( "status = 0x%02x\n", pr_close( outstr ) );
#endif
   
	return 0;
}
