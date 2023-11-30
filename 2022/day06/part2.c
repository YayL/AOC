#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"

#define UNIQUE_LENGTH 14
#define MAP_SIZE 28

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	int * map = malloc(sizeof(int) * MAP_SIZE);
	memset(map, -1, MAP_SIZE);

	size_t read = getline(&line, &len, fp);
	
	int start_index = 0, end_index = 0, count = 0;

	for (int i = 0; i < read; ++i) {
		for (int j = i; j < (i + UNIQUE_LENGTH); ++j) {
			short c = line[j] - 'a';
			if (map[c] != -1) {
				int index = map[c];
				count -= (index - i);
				for (int c_i = i; c_i < index + 1; ++c_i) {
					map[c_i - 'a'] = -1;
				}
				i = index + 1;
			}
			
			map[c] = j;
			if (++count == UNIQUE_LENGTH) {
				start_index = i;
				goto end;
			}
		}
	}

	end: {}


	unsigned long end = micros();

	print("First unique: ");
	for (int i = start_index; i < (start_index + UNIQUE_LENGTH); ++i) {
		putc(line[i], stdout);
	}

	println("\n{i}", start_index + UNIQUE_LENGTH);

	println("Execution Time: {lu}", (end - start));

}
