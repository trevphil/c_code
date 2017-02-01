
#include <stdio.h>

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("ERROR: You need at least one argument.\n");
		return 1;
	}
	
	for (int outer = 1; outer < argc; outer++) {
		int i = 0;
		char letter = argv[outer][i];
		for (i = 0; argv[outer][i] != '\0'; i++, letter = argv[outer][i]) {
		
			switch(letter) {
				case 'a': case 'A': printf("%d: 'A'\n", i); break;
                	        case 'e': case 'E': printf("%d: 'E'\n", i); break;
				case 'i': case 'I': printf("%d: 'I'\n", i); break;
				case 'o': case 'O': printf("%d: 'O'\n", i); break;
                	        case 'u': case 'U': printf("%d: 'U'\n", i); break;
        	                case 'y': case 'Y': 
					if (i > 2) {
						//only sometimes Y
						printf("%d: 'Y'\n", i);
					}
					break;
				default: printf("%d: %c is not a vowel\n", i, letter);
			}
		}
		printf("\n");
	}

	return 0;

}
