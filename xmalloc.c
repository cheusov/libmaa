/* xmalloc.c -- Error-checking malloc
 * Created: Sun Nov  6 18:14:10 1994 by faith@dict.org
 * Copyright 1994-1996, 2002 Rickard E. Faith (faith@dict.org)
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

#if STDC_HEADERS
#include <stdlib.h>
#else
void *malloc();
void *calloc();
void *realloc();
void free();
#endif

#ifndef DMALLOC_FUNC_CHECK

void *xmalloc( size_t size )
{
   void *pt = malloc( size );

   if (!pt) err_fatal( __func__,
		       "Out of memory while allocating %lu bytes\n",
		       (unsigned long)size );
   return pt;
}

void *xcalloc( size_t num, size_t size )
{
   void *pt = calloc( num, size );

   if (!pt) err_fatal( __func__,
		       "Out of memory while allocating %lu X %lu byte block\n",
		       (unsigned long)num, (unsigned long)size );
   return pt;
}

void *xrealloc( void *pt, size_t size )
{
   void *new = realloc( pt, size );

   if (!new) err_fatal( __func__,
			"Out of memory while reallocating block at %p to"
			" %lu bytes\n",
			pt,
			(unsigned long)size );
   return new;
}

void xfree( void *pt )
{
   if (pt) free( pt );
   else err_fatal( __func__, "Attempt to free null pointer\n" );
}

char *xstrdup( const char *s )
{
   size_t len = strlen (s);
   char *pt = xmalloc (len + 1);

   if (!pt) err_fatal( __func__,
		       "Out of memory while duplicating string\n" );

   memcpy (pt, s, len + 1);
   return pt;
}

#endif
