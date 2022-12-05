#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"

#define GET_INDEX(x) (((x) <= 'Z') ? ((x) - 'A' + 27) : ((x) - 'a' + 1))

int main() {
	
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("File not found");
		exit(1);
	}
	
	int sum = 0;
	size_t list_size = 53;
	char * list = calloc(list_size, sizeof(char));

	for (size_t i = 0; (read = getline(&line, &len, fp) - 1) != -2; i = (i + 1) % 3) {
		int index;
		for (int j = 0; j < read; ++j) {
			index = GET_INDEX(line[j]);
			if (list[index] == i)
				++list[index];
		}

		if (i == 2) {
			for (int j = 0; j < list_size; ++j) {
				if (list[j] == 3) {
					sum += j;
				}
			}
			memset(list, 0, list_size);
		}
		
	}

	unsigned long end = micros();

	println("Sum: {i}", sum);
	println("Execution Time: {lu}", (end - start));

}
