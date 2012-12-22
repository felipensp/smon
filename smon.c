#include <stdio.h>
#include <unistd.h>
#include "ptrace.h"

static void _smon_loop(pid_t pid)
{
	int status;

	while (1) {
		if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) != 0) {
			printf("Error: singlestep failed!\n");
			break;
		}		
		wait(&status);
 
		if (WIFEXITED(status)) {
			break;
		}
	}	
}

/**
 * Monitors by using a supplied pid
 */
void smon_pid(pid_t pid, size_t segment)
{
	ptrace_attach(pid);
	
	_smon_loop(pid);
	
	ptrace_detach(pid);	
}

/**
 * Monitors by using a program name
 */
void smon_exec(const char *program, size_t segment)
{
	printf("[%s]\n", program);
}
