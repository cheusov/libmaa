/* argtest.c -- 
 * Created: Sun Jan  7 20:23:34 1996 by faith@dict.org
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
 * $Id: argtest.c,v 1.10 2008/12/07 14:53:02 cheusov Exp $
 */

#include "maaP.h"

int main( int argc, char **argv )
{
   arg_List   a;
   int        c;
   char       **v;
   int        i;
   FILE *f;
   char buffer [2000];
   char *first_bq, *last_bq;
   maa_init( argv[0] );

   --argc; ++argv;

   if (argc)
      f = fopen (argv [0], "r");
   else
      f = fopen ("argtest.in", "r");

   if (!f)
      exit (10);

   while (fgets (buffer, sizeof(buffer), f)){
      first_bq = strchr (buffer, '`');
      last_bq  = strrchr (buffer, '`');
      if (!first_bq || !last_bq)
	 return 11;

      *first_bq++ = '\0';
      *last_bq    = '\0';

      printf ("---------------------\nInput = \"%s\"\n\n", first_bq);
      /* 0 */
      a = arg_argify (first_bq, 0);
      arg_get_vector (a, &c, &v);
      for (i = 0; i < c; i++){
	 printf ("Arg %d = \"%s\"\n", i, v[i]);
      }
      arg_destroy( a );

      printf ("\n");

      /* no escape */
      a = arg_argify (first_bq, ARG_NO_ESCAPE);
      arg_get_vector (a, &c, &v);
      for (i = 0; i < c; i++){
	 printf ("Arg %d = \"%s\"\n", i, v[i]);
      }
      arg_destroy( a );

      printf ("\n");

      /* no quote */
      a = arg_argify (first_bq, ARG_NO_QUOTE);
      arg_get_vector (a, &c, &v);
      for (i = 0; i < c; i++){
	 printf ("Arg %d = \"%s\"\n", i, v[i]);
      }
      arg_destroy( a );

      printf ("\n");

      /* no quote and no escape */
      a = arg_argify (first_bq, ARG_NO_QUOTE | ARG_NO_ESCAPE);
      arg_get_vector (a, &c, &v);
      for (i = 0; i < c; i++){
	 printf ("Arg %d = \"%s\"\n", i, v[i]);
      }
      arg_destroy( a );

      printf ("\n");
   }

   return 0;
}
