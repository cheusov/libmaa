/* listtest.c -- 
 * Created: Wed Aug  9 11:36:09 1995 by faith@dict.org
 * Revised: Sat Mar 30 12:06:35 2002 by faith@dict.org
 * Copyright 1995, 1996, 2002 Rickard E. Faith (faith@dict.org)
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
 * $Id: listtest.c,v 1.6 2002/05/03 14:12:23 faith Exp $
 */

#include "maaP.h"

static int print( const void *datum )
{
   printf( "%s ", (char *)datum );
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
