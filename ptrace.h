#ifndef SMON_PTRACE_H
#define SMON_PTRACE_H

#include <sys/ptrace.h>
#include <sys/wait.h>

void ptrace_attach(pid_t);
void ptrace_detach(pid_t);

#endif /* SMON_PTRACE_H */
