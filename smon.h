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

#ifndef SMON_H
#define SMON_H

#include <unistd.h>
#include <inttypes.h>

typedef struct {
	pid_t pid;           /* pid of monitored program (-p option) */
	const char *program; /* program name (-e option) */
	size_t segment;      /* segment to be monitored */
	int maxdump;         /* max number of segment dump */
	uintptr_t saddr;     /* start address of segment */
	uintptr_t eaddr;     /* end address of segment */
} smon_t;

/**
 * Max segment diff buffer
 */
#define MAXBUF 320

/**
 * Segments available to be monited
 */
#define SMON_STACK 0x1

/**
 * smon functions
 */
void smon_pid(smon_t*);
void smon_exec(smon_t*);

#endif /* SMON_H */
