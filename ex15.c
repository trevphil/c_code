#include <stdio.h>

int main(int argc, char *argv[]) {
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};
	int count = sizeof(ages)/sizeof(int);
	for (int i = 0; i < count; i++) {
		printf("%s has %d years alive.\n", names[i], ages[i]);
	}
	printf("---\n");
	
	int *cur_age = ages;
	char **cur_name = names;
	for (int i = 0; i < count; i++) {
		printf("%s is %d years old.\n", *(cur_name+i), *(cur_age+i));
	}
	printf("---\n");

	for (int i = 0; i < count; i++) {
		printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
	}
	printf("---\n");

	for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++) {
		printf("%s lived %d years so far.\n", *cur_name, *cur_age);
	}
	printf("---\n");

	cur_name = names;
	cur_age = ages;
	for (int i = 0; i < count; i++) {
		printf("cur_age %d is pointing at the address %p\n", i, cur_age);
                printf("cur_name %d is pointing at the address %p\n", i, cur_name);	
		cur_age++;
		cur_name++;
	}
	printf("sizeof(cur_age) = %lu, sizeof(cur_name) = %lu\n", sizeof(cur_age), sizeof(cur_name));

	return 0;
}
