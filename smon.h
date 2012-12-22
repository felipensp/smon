#ifndef SMON_H
#define SMON_H

#define SMON_STACK 0x1

void smon_pid(pid_t, size_t);
void smon_exec(const char*, size_t);

#endif /* SMON_H */
