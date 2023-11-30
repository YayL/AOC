#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "timer.h"

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
	char * list = malloc(sizeof(char) * list_size);

	while((read = getline(&line, &len, fp)) != -1) {
		const int _len = ((read - 1) >> 1);
		memset(list, 0, list_size);
		int index;
		for (int i = 0; i < _len; ++i) {
			index = GET_INDEX(line[i]);
			list[index] = 1;
		}
		
		for (int i = _len; i < read; ++i) {
			index = GET_INDEX(line[i]);
			if (list[index] != 0) {
				sum += index;
				list[index] = 0;
			}
		}	
	}
	
	fclose(fp);
	free(list);

	unsigned long end = micros();

	println("Sum: {i}", sum);
	println("Execution Time: {lu}", (end - start));

}
