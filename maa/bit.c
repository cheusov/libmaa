/* bit.c -- 
 * Created: Thu Sep 28 18:09:30 1995 by faith@dict.org
 * Copyright 1995-1996, 2002 Rickard E. Faith (faith@dict.org)
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
 * \section{Bit manipulation routines}
 *
 * \intro These are a set of simple routine to manipulate bits in an
 * integer.
 *
 */

#include "maaP.h"

/* \doc Set |bit| in |flags|. */

void bit_set(unsigned long *flags, int bit)
{
	*flags |= (1UL << bit);
}

/* \doc Clear |bit| in |flags|. */

void bit_clr(unsigned long *flags, int bit)
{
	*flags &= ~(1UL << bit);
}

/* \doc Test |bit| in |flags|, returning non-zero if the bit is set and
   zero if the bit is clear. */

int bit_tst(unsigned long *flags, int bit)
{
	return (*flags & (1UL << bit));
}

/* \doc Return a count of the number of bits set in |flags|. */

int bit_cnt(unsigned long *flags)
{
	unsigned long x = *flags;

#if SIZEOF_LONG == 4
	x = (x >> 1  & 0x55555555) + (x & 0x55555555);
	x = ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x = ((x >> 4) + x) & 0x0f0f0f0f;
	x = ((x >> 8) + x);
	return (x + (x >> 16)) & 0xff;
#else
#if SIZEOF_LONG == 8
	x = (x >> 1  & 0x5555555555555555) + (x & 0x5555555555555555);
	x = ((x >> 2) & 0x3333333333333333) + (x & 0x3333333333333333);
	x = ((x >> 4) + x) & 0x0f0f0f0f0f0f0f0f;
	x = ((x >> 8) + x) & 0x00ff00ff00ff00ff;
	x = ((x >> 16) + x) & 0x0000ffff0000ffff;
	return (x + (x >> 32)) & 0xff;
#else
	err_internal(__func__,
				 "Implemented for 32-bit and 64-bit longs, not %d-bit longs",
				 SIZEOF_LONG * 8);
#endif
#endif
}
