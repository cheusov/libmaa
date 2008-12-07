/* text.c -- Text manipulation routines for Libmaa
 * Created: Wed Apr 24 14:51:51 1996 by faith@dict.org
 * Copyright 1996, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
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
 */

#include <ctype.h>
#include "maaP.h"

/* \doc Given a string, return (in a |static| buffer), the Soundex value
   for the string.

   The basic algorithm for the soundex routine is from Donald
   E. Knuth's THE ART OF COMPUTER PROGRAMMING, Volume 3: Sorting and
   Searching (Addison-Wesley Publishing Co., 1973, pages 391 and 392).
   Knuth notes that the method was originally described by Margaret
   K. Odell and Robert C. Russell [US Patents 1261167 (1918) and 1435663
   (1922)]. */

void txt_soundex2(
   const char *string,
   char * result /* five chars */ )
{
   char        *pt = result;
   int         upper_case;
   /*                         abcdefghijklmnopqrstuvwxyz */
   static const char map[] = "01230120022455012623010202";
   char        previous = 0;
   char        transform;
   int         i;

   strcpy( result, "Z000" );

   for (i = 0; *string && i < 4; ++string) {
      if (isascii ( (unsigned char) *string ) && isalpha( (unsigned char) *string )) {
         upper_case = toupper( (unsigned char) *string );
         transform  = map[ upper_case - 'A' ];
         if (!i) {
            *pt++ = upper_case;
            ++i;
         } else {
            if (transform != '0' && transform != previous) {
               *pt++ = transform;
               ++i;
            }
         }
         previous = transform;
      }
   }
}

const char * txt_soundex( const char *string )
{
   static char buf [5];
   txt_soundex2 (string, buf);
   return buf;
}
