/**
 * smon
 * Copyright (c) 2012-2013 Felipe Pena <felipensp(at)gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "smon.h"

static void usage()
{
	printf("Usage: smon <-p pid| program> <options>\n"
			"Options:\n"
			" -s\tMonitors the stack segment\n"
			" -n\tMax number of segment dumps\n\n");
}

int main(int argc, char **argv)
{
	int c;
	smon_t info = {0};
	
	while ((c = getopt(argc, argv, "hn:p:s")) != -1) {
		switch (c) {
			case 'h':
				usage();
				exit(0);
			case 'n':
				info.maxdump = atol(optarg);
				break;
			case 'p':
				info.pid = atol(optarg);
				break;
			case 's':
				info.segment |= SMON_STACK;
				break;
		}
	}
	
	if (info.pid) {
		smon_pid(&info);
	} else if (optind < argc) {
		info.program = argv[optind];
		smon_exec(&info);
	} else {
		usage();
	}

	return 0;
}
