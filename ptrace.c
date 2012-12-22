#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "ptrace.h"

long ptrace_attach(pid_t pid)
{
	long ret;
	int status;

	ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	wait(&status);
	
	return ret;
}

long ptrace_detach(pid_t pid)
{
	return ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

long ptrace_write_long(pid_t pid, uintptr_t addr, long value) {
	return ptrace(PTRACE_POKETEXT, pid, addr, value);
}
 
long ptrace_read_long(pid_t pid, uintptr_t addr, void *vptr) {
	long word = ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);
	memcpy(vptr, &word, sizeof(long));
	
	return word;
}
