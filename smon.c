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
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "ptrace.h"
#include "smon.h"

/**
 * Dumps the segment data in a hexdump way
 */
static void _dump_segment(unsigned char *s, uintptr_t offset)
{
	int i, j;
	unsigned char printable[15];
	
	for (i = j = 0; i < MAXBUF; ++i) {
		if (j == 0) {
			printf("%#" PRIxPTR "  ", offset);
		}
		printf("%02x ", s[i]);
		printable[j++] = isprint(s[i]) ? s[i] : '.';
		
		if (j == 16) {
			printf(" |%.*s|\n", 16, printable);
			j = 0;
			offset += 16;
		}
	}
	printf("\n");
}

/**
 * Raw data diffing
 */
static unsigned char *_get_raw_diff(unsigned char *s1, unsigned char *s2,
	uint64_t slen, uintptr_t *offset)
{
	unsigned char *buf = calloc(1, MAXBUF);
	register uint64_t i = 0;
	size_t buflen = 0;
	
	for (; i < slen && buflen < MAXBUF; ++i) {
		if (s1[i] != s2[i]) {
			buf[buflen++] = s2[i];
		}
		if (buflen == 0) {
			++offset;
		}
	}
	
	return buf;
}

/**
 * Gets a snapshot of the segment from the image process
 */
static unsigned char *_get_snapshot(const smon_t *smon)
{
	unsigned char *mem, *buffer = calloc(1, smon->eaddr - smon->saddr);
	uintptr_t addr = smon->saddr;
	
	mem = buffer;

	while (addr < smon->eaddr) {
		ptrace_read_long(smon->pid, addr, mem);
		mem += sizeof(long);
		addr += sizeof(long);		
	}
	
	return buffer;
}

/**
 * Main-loop of tracing
 */
static void _loop(const smon_t *smon)
{
	unsigned char *mem, *mem2 = NULL;
	int status, ndump = 0;
	uintptr_t offset = smon->saddr;
	uint64_t slen = smon->eaddr - smon->saddr;
	
	while (1) {
		mem = mem2 ? mem2 : NULL;
		
		if (ptrace(PTRACE_SYSCALL, smon->pid, NULL, NULL) != 0) {
			printf("[!] singlestep failed!\n");
			free(mem);			
			break;
		}		
		wait(&status);
 
		if (WIFEXITED(status)) {
			printf("[!] program has exitted with status %d\n",
				WEXITSTATUS(status));
			free(mem);
			break;
		}
		
		mem2 = _get_snapshot(smon);
		
		if ((mem && mem2) && memcmp(mem, mem2, slen)) {			
			unsigned char *buf = _get_raw_diff(mem, mem2, slen, &offset);
			
			printf("[*] segment has been changed at %" PRIxPTR "\n",
				offset);
			
			if (ndump < smon->maxdump) {
				_dump_segment(buf, offset);
				free(buf);
			
				++ndump;
			}
		}
		
		if (mem) {
			free(mem);
			mem = NULL;
		}
	}
	if (mem != mem2) {
		free(mem2);
	}
}

/**
 * Finds the segment to be monitored
 */
static int _find_segment(const smon_t *smon, uintptr_t addr[2])
{
	FILE *fp;
	size_t size;
	uintptr_t start, end;
	char fname[PATH_MAX], filename[PATH_MAX], perms[5], *line = NULL;
	unsigned int offset, dmajor, dminor, inode;

	snprintf(fname, sizeof(fname), "/proc/%d/maps", smon->pid);
	
	if ((fp = fopen(fname, "r")) == NULL) {
		printf("[!] failed to open '%s'\n", fname);
		return 0;
	}

	while (getline(&line, &size, fp) != -1) {
		if (sscanf(line, "%" PRIxPTR "-%" PRIxPTR " %s %x %x:%x %u %s",
			&start, &end, perms, &offset, &dmajor, &dminor, &inode, 
			filename) < 6 ||
			(end - start) == 0) {
			return 0;
		}
		if ((smon->segment & SMON_STACK) != 0 
			&& strcmp(filename, "[stack]") == 0) {
			printf("[+] stack found at %lx-%lx (%ld bytes)\n",
				start, end, end - start);
			
			addr[0] = start;
			addr[1] = end;
			return 1;
		}
	}
	
	printf("[!] failed to find segment\n");	
	fclose(fp);
	
	return 0;
}

/**
 * Monitors by using a supplied pid
 */
void smon_pid(smon_t *smon)
{
	uintptr_t addrs[2];
	
	if (ptrace_attach(smon->pid) == 0) {
		printf("[+] attached to pid %d\n", smon->pid);
	} else {
		printf("[!] failed to attach to pid %d\n", smon->pid);
		return;
	}
	
	if (_find_segment(smon, addrs)) {
		smon->saddr = addrs[0];
		smon->eaddr = addrs[1];
		_loop(smon);
	}
	
	if (ptrace_detach(smon->pid) == 0) {
		printf("[+] detached from pid %d\n", smon->pid);
	} else {
		printf("[!] failed to detach from pid %d\n", smon->pid);
	}
}

/**
 * Monitors by using a program name
 */
void smon_exec(smon_t *smon)
{
	printf("[%s]\n", smon->program);
}
