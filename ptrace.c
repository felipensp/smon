#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "ptrace.h"

void ptrace_attach(pid_t pid)
{
	int status;

	ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	wait(&status);
}

void ptrace_detach(pid_t pid)
{
	ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

void ptrace_write_long(pid_t pid, uintptr_t addr, long value) {
	ptrace(PTRACE_POKETEXT, pid, addr, value);
}
 
void ptrace_read_long(pid_t pid, uintptr_t addr, void *vptr) {
	long word = ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);
	memcpy(vptr, &word, sizeof(long));
}
