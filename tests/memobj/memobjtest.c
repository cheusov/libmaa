/* memtest.c --
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maa.h"

int main(int argc, char** argv)
{
	mem_Object objects = NULL;
	int *one;
	int *two;
	int *three;

	maa_init(argv[0]);

	objects = mem_create_objects(sizeof(int));

	one = (int *) mem_get_object(objects);
	*one = 1;

	two = (int *) mem_get_object(objects);
	*two = 2;

	printf("obj1=%d\n", *one);
	printf("obj2=%d\n", *two);

	mem_print_object_stats(objects, stdout);

	mem_free_object(objects, two);

	mem_print_object_stats(objects, stdout);

	*two = 22;
	two = (int *) mem_get_object(objects);
	printf("obj2_reused=%d\n", *two);
	mem_print_object_stats(objects, stdout);

	mem_free_object(objects, two);
	two = (int *) mem_get_empty_object(objects);
	printf("obj2_zeroed=%d\n", *two);
	*two = 2;
	mem_print_object_stats(objects, stdout);

	three = (int *) mem_get_empty_object(objects);
	*three = 3;

	printf("obj1=%d\n", *one);
	printf("obj2=%d\n", *two);
	printf("obj3=%d\n", *three);

	mem_print_object_stats(objects, stdout);

	mem_destroy_objects(objects);

	maa_shutdown();
	return 0;
}
