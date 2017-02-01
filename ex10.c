#include <stdio.h>

int main(int argc, char *argv[]) {

	printf("argc = %d\n", argc);
	int i = 0;
	for (i = 0; i < argc; i++) {
		printf("arg %d: %s\n", i, argv[i]);
	}
	
	char *states[] = {"California", "Oregon", "Washington", "Texas"};
	int numStates = 4;
	for (i = 0; i < numStates; i++) {
		printf("state %d: %s\n", i, states[i]);
	}

	return 0;

}
