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
 * Raw data diffing
 */
static unsigned char *_get_raw_diff(unsigned char *s1, unsigned char *s2,
	uint64_t slen, uintptr_t *offset)
{
	const size_t bufmax = 600;
	unsigned char *buf = calloc(1, bufmax);
	register uint64_t i = 0;
	size_t buflen = 0;
	
	for (; i < slen && buflen < bufmax; ++i) {
		if (s1[i] != s2[i]) {
			buf[buflen++] = isprint(s2[i]) ? s2[i] : '.';
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
static unsigned char *_get_snapshot(pid_t pid, uintptr_t addrs[2])
{
	unsigned char *mem, *buffer = calloc(1, addrs[1] - addrs[0]);
	uintptr_t addr = addrs[0];
	
	mem = buffer;

	while (addr < addrs[1]) {
		ptrace_read_long(pid, addr, mem);
		mem += sizeof(long);
		addr += sizeof(long);		
	}
	
	return buffer;
}

/**
 * Main-loop of tracing
 */
static void _loop(pid_t pid, uintptr_t addrs[2])
{
	unsigned char *mem, *mem2 = NULL;
	int status;
	uintptr_t offset = addrs[0];
	uint64_t slen = addrs[1] - addrs[0];
	
	while (1) {
		mem = mem2 ? mem2 : NULL;
		
		if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) != 0) {
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
		
		mem2 = _get_snapshot(pid, addrs);
		
		if ((mem && mem2) && memcmp(mem, mem2, slen)) {			
			unsigned char *buf = _get_raw_diff(mem, mem2, slen, &offset);
			
			printf("[*] segment has been changed at %" PRIxPTR "\n",
				offset);			
			printf("Printable snapshot (600 chars) [%s]\n", buf);
			free(buf);
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
static int _find_segment(pid_t pid, size_t segment, uintptr_t addr[2])
{
	FILE *fp;
	size_t size;
	uintptr_t start, end;
	char fname[PATH_MAX], filename[PATH_MAX], perms[5], *line = NULL;
	unsigned int offset, dmajor, dminor, inode;

	snprintf(fname, sizeof(fname), "/proc/%d/maps", pid);
	
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
		if ((segment & SMON_STACK) && strcmp(filename, "[stack]") == 0) {
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
void smon_pid(pid_t pid, size_t segment)
{
	uintptr_t addrs[2];
	
	if (ptrace_attach(pid) == 0) {
		printf("[+] attached to pid %d\n", pid);
	} else {
		printf("[!] failed to attach to pid %d\n", pid);
		return;
	}
	
	if (_find_segment(pid, segment, addrs)) {
		_loop(pid, addrs);
	}
	
	if (ptrace_detach(pid) == 0) {
		printf("[+] detached from pid %d\n", pid);
	} else {
		printf("[!] failed to detach from pid %d\n", pid);
	}
}

/**
 * Monitors by using a program name
 */
void smon_exec(const char *program, size_t segment)
{
	printf("[%s]\n", program);
}
