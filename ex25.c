#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer) {
	// calloc(size_t num, size_t size)
	// allocates memory for 'num' elements, each of which is 'size' bytes
	// all bits are initialized to zero
	*out_string = calloc(1, max_buffer + 1);
	check_mem(*out_string);

	// fgets(char *str, int num, FILE *stream)
	// reads characters from 'stream' and stores them into 'str'
	// until num-1 characters have been read or there is a newline
	char *result = fgets(*out_string, max_buffer, stdin);
	check(result != NULL, "Input error.");

	return 0;

error:
	if (*out_string) free(*out_string);
	*out_string = NULL;
	return -1;
}

int read_int(int *out_int) {
	char *input = NULL;
	int rc = read_string(&input, MAX_DATA);
	check(rc == 0, "Failed to read number.");

	*out_int = atoi(input);

	free(input);
	return 0;

error:
	if (input) free(input);
	return -1;
}

// the "..." is a keyword in C that means this function will take
// any number of arguments after 'fmt' (but you can't specify args after ...)
int read_scan(const char *fmt, ...) {
	int i = 0;
	int rc = 0;
	int *out_int = NULL;
	char *out_char = NULL;
	char **out_string = NULL;
	int max_buffer = 0;

	// "va_list" is a type used as a parameter for macros to retrieve additional args
	// va_start(va_list ap, paramN) initializes 'ap' to retrieve the additional
	// arguments after parameter 'paramN'
	va_list argp;
	va_start(argp, fmt);
	
	for (i = 0; fmt[i] != '\0'; i++) {
		if (fmt[i] == '%') {
			i++;
			switch (fmt[i]) {
				case '\0':
					sentinel("Invalid format, you ended with %%.");
					break;
				case 'd':
					// va_arg(va_list ap, type) 'type' is the return type
					// each call to va_arg() changes the state of 'ap'
					// to be the next argument in "..."
					out_int = va_arg(argp, int *);
					rc = read_int(out_int);
					check (rc == 0, "Failed to read int.");
					break;
				case 'c':
					out_char = va_arg(argp, char *);
					// fgetc(FILE *stream)
					// returns the char currently pointed by internal file
					// position indicator of 'stream' and advances the
					// position indicator to the next character
					// returns EOF and sets "feof" if 'stream' is at the end
					*out_char = fgetc(stdin);
					break;
				case 's':
					max_buffer = va_arg(argp, int);
					out_string = va_arg(argp, char **);
					rc = read_string(out_string, max_buffer);
					check (rc == 0, "Failed to read string.");
					break;
				default:
					sentinel("Invalid format.");
			}
		} else {
			fgetc(stdin);
		}

		// feof(FILE *stream) Checks whether the end-of-file indicator associated with 'stream' is set, 
		// returning a value different from zero if it is
		// ferror(FILE *stream) Checks if the error indicator associated with 'stream' is set, returning
		// a value different from zero if it is
		check (!feof(stdin) && !ferror(stdin), "Input error.");
	}

	// va_end(va_list ap)
	// should be invoked before the return statement if the function used va_start()
	va_end(argp);
	return 0;

error:
	va_end(argp);
	return -1;
}

int main(int argc, char *argv[]) {
	char *first_name = NULL;
	char initial = ' ';
	char *last_name = NULL;
	int age = 0;

	printf("What's your first name? ");
	int rc = read_scan("%s", MAX_DATA, &first_name);
	check (rc == 0, "Failed first name.");
	
	printf("What's your initial? ");
	rc = read_scan("%c\n", &initial);
	check (rc == 0, "Failed initial.");
	
	printf("What's your last name? ");
	rc = read_scan("%s", MAX_DATA, &last_name);
	check (rc == 0, "Failed last name.");

	printf("How old are you? ");
	rc = read_scan("%d", &age);

	printf("---- RESULTS ----\n");
	printf("First name: %s", first_name);
	printf("Initial: '%c'\n", initial);
	printf("Last name: %s", last_name);
	printf("Age: %d\n", age);

	free (first_name);
	free (last_name);
	return 0;

error:
	return -1; 
}

