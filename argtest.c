/* argtest.c -- 
 * Created: Sun Jan  7 20:23:34 1996 by r.faith@ieee.org
 * Revised: Sat Mar  8 15:05:43 1997 by faith@cs.unc.edu
 * Copyright 1996 Rickard E. Faith (r.faith@ieee.org)
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
 * $Id: argtest.c,v 1.3 1997/03/08 20:07:38 faith Exp $
 */

#include "maaP.h"

int main( int argc, char **argv )
{
   arg_List   a;
   int        c;
   char       **v;
   int        i;

   maa_init( argv[0] );

   a = arg_argify( "\\\\This  is a \"test of quotes\" and  ano'ther test'  " );
   arg_get_vector( a, &c, &v );
   for (i = 0; i < c; i++) printf( "Arg %d = \"%s\"\n", i, v[i] );
   arg_destroy( a );

   a = arg_argify( "  aaa1 foo  " );
   arg_get_vector( a, &c, &v );
   for (i = 0; i < c; i++) printf( "Arg %d = \"%s\"\n", i, v[i] );
   arg_destroy( a );
   
   a = arg_argify( "  aaa2 foo" );
   arg_get_vector( a, &c, &v );
   for (i = 0; i < c; i++) printf( "Arg %d = \"%s\"\n", i, v[i] );
   arg_destroy( a );
   
   a = arg_argify( "  aaa3 foo   \n" );
   arg_get_vector( a, &c, &v );
   for (i = 0; i < c; i++) printf( "Arg %d = \"%s\"\n", i, v[i] );
   arg_destroy( a );
   
   a = arg_argify( "  " );
   arg_get_vector( a, &c, &v );
   for (i = 0; i < c; i++) printf( "Arg %d = \"%s\"\n", i, v[i] );
   arg_destroy( a );

   a = arg_argify( "" );
   arg_get_vector( a, &c, &v );
   for (i = 0; i < c; i++) printf( "Arg %d = \"%s\"\n", i, v[i] );
   arg_destroy( a );

   return 0;
}
