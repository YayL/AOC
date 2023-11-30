#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"

#define UNIQUE_LENGTH 14

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	size_t read = getline(&line, &len, fp);
	
	int start_index = 0, end_index = 0;
	HashMap * map = new_HashMap(8);

	for (int i = 0; i < read; ++i) {
		for (int j = i; j < (i + UNIQUE_LENGTH); ++j) {
			char * c = malloc(2 * sizeof(char));
			c[0] = line[j];
			c[1] = 0;
			if (HM_has(map, c)) {
				int index = HM_remove(map, c);
				for (int c_i = i; c_i < index; ++c_i) {
					char * k = malloc(2 * sizeof(char));
					k[0] = line[c_i];
					k[1] = 0;
					HM_remove(map, k);
					free(k);
				}
				i = index + 1;
			}

			HM_set(map, c, j);
			free(c);
			if (map->total == UNIQUE_LENGTH) {
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
