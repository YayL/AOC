#include <stdlib.h>
#include <stdio.h>

int main() {

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0;

	if (fp == NULL) {
		puts("File not found");
		exit(1);
	}

	int sum = 0, swap = 0, max_sum = 0;
	size_t read;

	while ((read = getline(&line, &len, fp)) != -1) {
		if (line[0] != '\n') {
			if (sum > max_sum) {
				max_sum = sum;
			}
			sum = 0;
		} else {
			sum += atoi(line);
		}
	}

	if (sum > max_sum)
		max_sum = sum;

	printf("Sum: %d\n", max_sum);

}
