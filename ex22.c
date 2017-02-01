#include <stdio.h>
#include "ex22.h"
#include "dbg.h"

int THE_SIZE = 1000;

// 'static' keyword is the opposite of 'extern'
// says that the variable is only for this .c file
// and shouldn't be available to other parts of the program
static int THE_AGE = 37;

// commands from other files can only access THE_AGE indirectly
// it's like "private" in Java
int get_age() {
	return THE_AGE;
}
void set_age(int age) {
	THE_AGE = age;
}

double update_ratio(double new_ratio) {
	// 'static' is rarely used within a function
	// the variable is only accessible from this function
	// basically creates a constant state for the function
	static double ratio = 1.0;

	double old_ratio = ratio;
	ratio = new_ratio;

	return old_ratio;
}

void print_size() {
	log_info("I think size is: %d", THE_SIZE);
}
