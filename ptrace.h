#ifndef SMON_PTRACE_H
#define SMON_PTRACE_H

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdint.h>

long ptrace_attach(pid_t);
long ptrace_detach(pid_t);

long ptrace_write_long(pid_t, uintptr_t, long);
long ptrace_read_long(pid_t, uintptr_t, void*);

#endif /* SMON_PTRACE_H */
