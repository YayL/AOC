#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/set.c"
#include "../../includes/list.c"

typedef struct square {
	int x;
	int y;
	int z;
} Square;

#define MAX(x, y) if((x) < (y)) x = y
#define MIN(x, y) if((x) > (y)) x = y
#define get_key(x,y,z) asprintf(&key, "%d,%d,%d", x, y, z)

Set * cubes; // * exteriors;

char is_exterior();

int min_x = 999, min_y = 999, min_z = 999;
int max_x = 0, max_y = 0, max_z = 0;

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL, * copy;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	cubes = new_set(8);
	//exteriors = new_set(8);
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
				if (is_exterior(x - 1, y, z)) ++sides; // left
				if (is_exterior(x + 1, y, z)) ++sides; // right
				if (is_exterior(x, y - 1, z)) ++sides; // top
				if (is_exterior(x, y + 1, z)) ++sides; // down
				if (is_exterior(x, y, z - 1)) ++sides; // behind
				if (is_exterior(x, y, z + 1)) ++sides; // front
			}
		}
	}
	
	println("Answer: {i}", sides);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}

Square * new_square(int x, int y, int z) {
	Square * sq = malloc(sizeof(Square));
	sq->x = x, sq->y = y, sq->z = z;
	return sq;
}

char is_exterior(int x, int y, int z) {
	
	char * key, * info;
	get_key(x, y, z);
	if (set_has(cubes, key))
		return 0;

	Square * square;

	struct List * to_check = init_list(sizeof(Square *));
	Set * checked = new_set(6);
	
	list_push(to_check, new_square(x, y, z));

	while(to_check->size) {
		square = list_pop(to_check);

		get_key(square->x, square->y, square->z);
		if (set_has(checked, key)) {
			continue;
		}
		
		set_add(checked, key);

		// if (set_has(exteriors, key)) {
		// 	goto end_true;
		// }

		if (!(min_x <= square->x && square->x <= max_x) ||
			!(min_y <= square->y && square->y <= max_y) ||
			!(min_z <= square->z && square->z <= max_z)) {
				goto end_true;
		}
		
		if (!set_has(cubes, key)) {
			list_push(to_check, new_square(square->x - 1, square->y, square->z));
			list_push(to_check, new_square(square->x + 1, square->y, square->z));
			list_push(to_check, new_square(square->x, square->y - 1, square->z));
			list_push(to_check, new_square(square->x, square->y + 1, square->z));
			list_push(to_check, new_square(square->x, square->y, square->z - 1));
			list_push(to_check, new_square(square->x, square->y, square->z + 1));
		}
		free(square);
	}
	
	return 0;

	end_true:

	// while (to_check->size) {
	// 	square = list_pop(to_check);
	// 	get_key(square->x, square->y, square->z);
	// 	set_add(exteriors, key);
	// 	free(square);
	// }
	// get_key(x, y, z);
	// set_add(exteriors, key);

	return 1;

}
