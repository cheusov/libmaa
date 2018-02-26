/* memtest.c -- 
 * Created: Sat Jul  1 22:42:09 1995 by faith@dict.org
 * Copyright 1995, 2002 Rickard E. Faith (faith@dict.org)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maa.h"

static char const *names [] = {
	"Peter Hammill",
	"King Crimson",
	"Van ",
	"Der ",
	"Graaf ",
	"Generator",
	"Led Zeppelin"
};

int main(int argc, char** argv)
{
	mem_String strings = NULL;

	maa_init(argv[0]);

	strings = mem_create_strings();

	mem_print_string_stats(strings, stdout);
	printf("str1=%s\n", mem_strcpy(strings, names[0]));
	mem_print_string_stats(strings, stdout);
	printf("str2=%s\n", mem_strcpy(strings, names[1]));
	mem_print_string_stats(strings, stdout);
	mem_grow(strings, names[2], strlen(names[2]));
	mem_grow(strings, names[3], strlen(names[3]));
	mem_grow(strings, names[4], strlen(names[4]));
	mem_grow(strings, names[5], strlen(names[5]));
	printf("str3=%s\n", mem_finish(strings));
	mem_print_string_stats(strings, stdout);
	printf("str4=%s\n", mem_strncpy(strings, names[6], strlen(names[6])));
	mem_print_string_stats(strings, stdout);

	mem_destroy_strings(strings);

	maa_shutdown();
	return 0;
}
