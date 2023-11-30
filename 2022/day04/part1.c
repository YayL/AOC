#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"

typedef struct range {
	int min, max;
} Range;

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	int min1, max1, min2, max2;
	int sum = 0;

	while((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "%d-%d,%d-%d", &min1, &max1, &min2, &max2);

		if (min1 <= min2 && max2 <= max1)
			sum++;
		else if (min2 <= min1 && max1 <= max2)
			sum++;
	}

	unsigned long end = micros();
	println("Sum: {i}", sum);
	println("Execution Time: {lu}", (end - start));

}
