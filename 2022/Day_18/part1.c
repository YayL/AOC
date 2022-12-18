#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/set.c"
#include "../../includes/list.c"

#define MAX(x, y) if((x) < (y)) x = y
#define MIN(x, y) if((x) > (y)) x = y
#define get_key(x,y,z) asprintf(&key, "%d,%d,%d", x, y, z)

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL, * copy;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	Set * cubes = new_set(8);
	int min_x = 999, min_y = 999, min_z = 999;
	int max_x = 0, max_y = 0, max_z = 0;
	int x, y, z;
	
	while((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "%d,%d,%d", &x, &y, &z);
		MAX(max_x, x);
		MAX(max_y, y);
		MAX(max_z, z);
		MIN(min_x, x);
		MIN(min_y, y);
		MIN(min_z, z);

		copy = malloc(sizeof(char) * (read - 1));
		memcpy(copy, line, read - 1);
		set_add(cubes, copy);
	}
	
	char * key;
	int sides = 0;

	for (int x = min_x; x <= max_x; ++x) {
		for (int y = min_y; y <= max_y; ++y) {
			for (int z = min_z; z <= max_z; ++z) {
				get_key(x, y, z);
				if (!set_has(cubes, key))
					continue;
				get_key(x - 1, y, z);
				if (!set_has(cubes, key)) ++sides; // left
				get_key(x + 1, y, z);
				if (!set_has(cubes, key)) ++sides; // right
				get_key(x, y + 1, z);
				if (!set_has(cubes, key)) ++sides; // top
				get_key(x, y - 1, z);
				if (!set_has(cubes, key)) ++sides; // down
				get_key(x, y, z - 1);
				if (!set_has(cubes, key)) ++sides; // behind
				get_key(x, y, z + 1);
				if (!set_has(cubes, key)) ++sides; // front
			}
		}
	}
	


	println("Answer: {i}", sides);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
