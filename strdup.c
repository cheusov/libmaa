/* strdup.c -- String duplicate function
 * Created: Mon Nov  7 10:23:32 1994 by faith@cs.unc.edu
 * Revised: Mon Feb 26 11:18:58 1996 by faith@cs.unc.edu
 * Public Domain 1994, 1995, 1996 Rickard E. Faith (faith@cs.unc.edu)
 *
 * This function is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * $Id: strdup.c,v 1.3 1996/02/26 19:57:17 faith Exp $ */

#include "maaP.h"

#if STDC_HEADERS
#include <stdlib.h>
#else
void *malloc();
#endif

char *strdup( const char *s)
{
   int   len = strlen( s );
   char *r;

   r = malloc( len + 1 );
   strcpy( r, s );
   return r;
}
