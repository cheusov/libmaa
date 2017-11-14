/* argtest.c -- 
 * Created: Sun Jan  7 20:23:34 1996 by faith@dict.org
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
