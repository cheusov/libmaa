/* sltest.c -- 
 * Copyright 2018 Aleksey Cheusov (vle@gmx.net)
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

#include <stdio.h>

int main( int argc, char **argv )
{
	stk_Stack     stack = NULL;
	stk_Stack     stack2 = NULL;
	void *        datum;

	maa_init( argv[0] );
   
	stack = stk_create();

	printf("isempty=%d\n", stk_isempty(stack));

	stk_push(stack, __UNCONST("10"));
	stk_push(stack, __UNCONST("20"));
	stk_push(stack, __UNCONST("30"));
	stk_push(stack, __UNCONST("40"));
	stk_push(stack, __UNCONST("50"));

	printf("isempty=%d\n", stk_isempty(stack));

	while ((datum = stk_pop(stack)) != NULL){
		printf("pop=%s\n", (char *) datum);
	}

	printf("isempty=%d\n", stk_isempty(stack));

	stk_push(stack, __UNCONST("10"));
	stk_push(stack, __UNCONST("20"));
	printf("top=%s\n", (char *) stk_top(stack));
	printf("top=%s\n", (char *) stk_top(stack));

	printf("isempty=%d\n", stk_isempty(stack));

	stack2 = stk_create();

	stk_destroy(stack);
	stk_destroy(stack2);
	maa_shutdown();

	return 0;
}
