/* prime.c -- Find prime numbers
 * Created: Thu Jul 20 22:04:37 1995 by faith@dict.org
 * Copyright 1995, 1996, 2002 Rickard E. Faith (faith@dict.org)
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
 * \section{Prime Number Routines}
 *
 * \intro These routines are used to find the next larger prime number for
 * expansion of the hash tables used by the hash and set routines.  These
 * routines are only useful for 32-bit unsigned values.
 *
 */

#include "maaP.h"

/* \doc |prm_is_prime| returns 1 if |value| is prime; 0 otherwise.
   Primality is determined by testings all odd divisors less than the
   square root of |value|.  For 32-bit integers, this may mean that we will
   test about $\frac{\sqrt{2^{32}-1}}{2} = 32768$ odd values instead of the
   6542 primes that would actually need to be tested.  (A table of
   pre-computed primes using less than 26kB of memory could be used to
   recover this factor of 5 performance loss.) */

int prm_is_prime(unsigned int value)
{
	unsigned int divisor = 3;
	unsigned int square  = divisor * divisor;

	if (value == 2)   return 1;
	if (value == 3)   return 1;
	if (!(value & 1)) return 0;	/* even */

	while (square < value && value % divisor) {
		++divisor;
		square += 4 * divisor;
		++divisor;
	}

	return value % divisor != 0;
}

/* \doc |prm_next_prime| returns the smallest odd prime greater than or
   equal to |start|. */

unsigned long prm_next_prime(unsigned int start)
{
	unsigned int next;
   
	for (next = start | 1; !prm_is_prime(next); next += 2);
	return next;
}
