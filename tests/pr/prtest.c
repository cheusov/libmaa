/* prtest.c -- 
 * Created: Fri Jan 12 14:18:32 1996 by faith@dict.org
 * Copyright 1996, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2024 Aleksey Cheusov (vle@gmx.net)
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
#include <errno.h>

#include "maaP.h"

#define BUFFER_SIZE 1024

static void callback(void)
{
	printf("baz\n");
	fflush(stdout);
}

int main(int argc, char **argv)
{
	int  fdin = 0;
	int  fdout;
	char buf[BUFFER_SIZE];
	int  c;
	int  i;
	ssize_t cnt = 0;

	maa_init(argv[0]);

	while ((c = getopt(argc, argv, "D")) != EOF)
		switch (c) {
			case 'D': dbg_set(".pr"); break;
		}

	if (argc-optind == 0) {
		pr_open("echo foo", PR_USE_STDIN | PR_CREATE_STDOUT,
				 &fdin, &fdout, NULL);
	} else {
		for (i = optind; i < argc; i++) {
			pr_open(argv[i], PR_USE_STDIN | PR_CREATE_STDOUT,
					 &fdin, &fdout, NULL);
			fdin = fdout;
		}
	}

	printf ("Got:");
	while (cnt = read(fdout, buf, BUFFER_SIZE), cnt > 0){
		buf [cnt] = 0;
		printf(" \"%s\"\n", buf);
	}
	printf("status = 0x%02x\n", pr_close (fdout));

	pr_open2("echo foobar", callback, PR_USE_STDIN | PR_CREATE_STDOUT,
			 NULL, &fdin, NULL);

	pr_open("cat", PR_USE_STDIN | PR_CREATE_STDOUT,
			 &fdin, &fdout, NULL);
	ssize_t size = read(fdout, buf, sizeof(buf));
	assert(size == 7);
	buf[size] = '\0';
	printf("Got \"%s\"\n", buf);

	printf("status = 0x%02x\n", pr_close(fdout));

	pr_open("cat", PR_CREATE_STDIN | PR_CREATE_STDOUT,
			 &fdin, &fdout, NULL);
	static const char message[] = "this is a test\n";
	size = write(fdin, message, sizeof(message) - 1);
	assert(size == sizeof(message) - 1);
	size = read(fdout, buf, sizeof(buf));
	assert(size == sizeof(message) - 1);
	buf[size] = '\0';
	printf("Got \"%s\"\n", buf);
	printf("status = 0x%02x\n", pr_close(fdin));
	printf("status = 0x%02x\n", pr_close(fdout));
	fflush(stdout);

	return 0;
}
