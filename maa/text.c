/* text.c -- Text manipulation routines for Libmaa
 * Created: Wed Apr 24 14:51:51 1996 by faith@dict.org
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
	char * result /* five chars */)
{
	char        *pt = result;
	int         upper_case;
	/*                         abcdefghijklmnopqrstuvwxyz */
	static const char map[] = "01230120022455012623010202";
	char        previous = 0;
	char        transform;
	int         i;

	strcpy(result, "Z000");

	for (i = 0; *string && i < 4; ++string) {
		if (isascii ((unsigned char) *string) && isalpha((unsigned char) *string)) {
			upper_case = toupper((unsigned char) *string);
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

const char * txt_soundex(const char *string)
{
	static char buf [5];
	txt_soundex2 (string, buf);
	return buf;
}
