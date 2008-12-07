/* prtest.c -- 
 * Created: Fri Jan 12 14:18:32 1996 by faith@dict.org
 * Copyright 1996, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
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
 */

#include "maaP.h"

int main( int argc, char **argv )
{
   int  fdin = 0;
   int  fdout;
   char buf[BUFSIZ];
   int  c;
   int  i;
   pid_t pid = 0;
   ssize_t cnt = 0;

   maa_init( argv[0] );

   while ((c = getopt( argc, argv, "D" )) != EOF)
      switch (c) {
      case 'D': dbg_set( ".pr" ); break;
      }

   if (argc-optind == 0) {
      pid = pr_open( "echo foo", PR_USE_STDIN | PR_CREATE_STDOUT,
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
