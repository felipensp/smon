#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "smon.h"

static void usage()
{
	printf("Usage: smon <-p pid| program> <options>\n"
			"Options:\n"
			" -s\tMonitors the stack segment\n\n");
}

int main(int argc, char **argv)
{
	int c;
	pid_t pid = 0;
	size_t segment = 0;
	
	while ((c = getopt(argc, argv, "hp:s")) != -1) {
		switch (c) {
			case 'h':
				usage();
				exit(0);
			case 'p':
				pid = atol(optarg);
				break;
			case 's':
				segment |= SMON_STACK;
				break;
		}
	}
	
	if (pid) {
		smon_pid(pid, segment);
	} else if (optind < argc) {
		smon_exec(argv[optind], segment);
	} else {
		usage();
	}

	return 0;
}
