#include <stdlib.h>
#include <stdio.h>

int max_sum[3] = {0, 0, 0};

int get_min() {
	int min = 0;
	if (max_sum[min] > max_sum[1])
		min = 1;
	if (max_sum[min] > max_sum[2])
		min = 2;
	return min;
}

int main() {

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0;

	if (fp == NULL) {
		puts("File not found");
		exit(1);
	}

	int sum = 0, min = 0;
	size_t read;

	while ((read = getline(&line, &len, fp)) != -1) {
		if (line[0] == '\n') {
			min = get_min();	
			if (sum > max_sum[min]) {
				max_sum[min] = sum;
			}
			sum = 0;
		} else {
			sum += atoi(line);
		}
	}

	min = get_min();
	if (sum > max_sum[min])
		max_sum[min] = sum;

	printf("Sum: %d\n", max_sum[0] + max_sum[1] + max_sum[2]);

}
