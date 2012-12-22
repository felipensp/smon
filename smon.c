#include <stdio.h>
#include <unistd.h>

/**
 * Monitors by using a supplied pid
 */
void smon_pid(pid_t pid, size_t segment)
{

}

/**
 * Monitors by using a program name
 */
void smon_exec(const char *program, size_t segment)
{
	printf("[%s]\n", program);
}
