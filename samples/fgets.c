#include <stdio.h>

int main(int argc, char **argv)
{
	char str[100];
	
	if (fgets(str, sizeof(str), stdin)) {
		printf("Received: %s\n", str);
	}
	
	return 0;
}
