/* strdup.c -- String duplicate function
 * Created: Mon Nov  7 10:23:32 1994 by faith@dict.org
 * Revised: Sat Mar 30 12:08:35 2002 by faith@dict.org
 * Copyright 1994-1996, 2002 Rickard E. Faith (faith@dict.org)
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * $Id: strdup.c,v 1.6 2002/08/02 19:43:15 faith Exp $
 *
 */

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
