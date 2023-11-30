#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"

#define GET_MAX(x, y) ((x) < (y) ? (y) : (x))

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
	Range first, second, temp;

	while((read = getline(&line, &len, fp)) != -1) {

		sscanf(line, "%d-%d,%d-%d", 
			&first.min, &first.max,
			&second.min, &second.max);
		
		if (second.max <= first.max) {
			temp = first;
			first = second, second = temp;
		}
		
		if (second.min <= first.max) {
			sum++;
		}
	}

	unsigned long end = micros();
	println("Sum: {i}", sum);
	println("Execution Time: {lu}", (end - start));

}
