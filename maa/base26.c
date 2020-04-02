/* base64.c -- Encode/decode integers in base64 format
 * Created: Mon Sep 23 16:55:12 1996 by faith@dict.org
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

const char *b26_encode(unsigned long val)
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

unsigned long b26_decode(const char *val)
{
	unsigned long v = 0;
	int           i;
	int           offset = 1;
	int           len = strlen(val);

	for (i = len - 1; i >= 0; i--) {
		int tmp = b26_index[ (unsigned char)val[i] ];

		if (tmp == XX)
			err_internal(__func__,
						 "Illegal character in base26 value: `%c' (%d)",
						 val[i], val[i]);

		v += tmp * offset;
		offset *= 26;
	}

	return v;
}
