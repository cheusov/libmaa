/* base64.c -- Encode/decode integers in base64 format
 * Created: Mon Sep 23 16:55:12 1996 by faith@dict.org
 * Revised: Sat Mar 30 11:55:09 2002 by faith@dict.org
 * Copyright 1996, 2002 Rickard E. Faith (faith@dict.org)
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
 * $Id: base26.c,v 1.3 2002/08/02 19:43:15 faith Exp $
 *
 * \section{Base-26 Routines}
 *
 * \intro These routines perform encoding and decodingusing the 26
 * lowercase characters, a-z.  This is sometimes useful for generating
 * unique identifiers that do not contain numbers.
 *
 */

#include "maaP.h"

static unsigned char b26_list[] =
"abcdefghijklmnopqrstuvwxyz";

#define XX 100

static int b26_index[256] = {
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};

/* |b26_encode| encodes |val| in a printable base 26 format.  A MSB-first
   encoding is generated. */

const char *b26_encode( unsigned long val )
{
   static char          result[8] = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', '\0'};
   static unsigned long previous = 0;
   int                  i;

   if (previous != val) {
      previous = val;
      for (i = 6; i >= 0; i--) {
	 result[i] = b26_list[ val % 26 ];
	 val = val / 26;
      }
      result[7] = 0;
   }

   for (i = 0; i < 6; i++) if (result[i] != b26_list[0]) return result + i;
   return result + 6;
}

unsigned long b26_decode( const char *val )
{
   unsigned long v = 0;
   int           i;
   int           offset = 1;
   int           len = strlen( val );

   for (i = len - 1; i >= 0; i--) {
      int tmp = b26_index[ (unsigned char)val[i] ];

      if (tmp == XX)
	 err_internal( __FUNCTION__,
		       "Illegal character in base26 value: `%c' (%d)\n",
		       val[i], val[i] );

      v += tmp * offset;
      offset *= 26;
   }

   return v;
}
