/* strdup.c -- String duplicate function
 * Created: Mon Nov  7 10:23:32 1994 by faith@cs.unc.edu
 * Revised: Sun Nov 19 13:30:19 1995 by faith@cs.unc.edu
 * Public Domain 1994, 1995 Rickard E. Faith (faith@cs.unc.edu)
 *
 * This function is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * $Id: strdup.c,v 1.2 1995/11/19 19:28:50 faith Exp $ */

#include "maaP.h"

#if STDC_HEADERS
#include <stdlib.h>
#else
void *malloc();
#endif

char *strdup( char *s)
{
   int   len = strlen( s );
   char *r;

   r = malloc( len + 1 );
   strcpy( r, s );
   return r;
}
