#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"

typedef struct tree {
	char height;
	char visible;
} Tree;

#define get_tree(map, x, y) ((Tree *)((struct List *)map->items[y])->items[x])

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	struct List * map = init_list(sizeof(struct List *));

	read = getline(&line, &len, fp);
	list_reserve(map, read);

	while(read != -1) {
		struct List * row = init_list(sizeof(Tree *));
		list_reserve(row, read);
		for (int i = 0; i < read; ++i) {
			Tree * _tree = malloc(sizeof(Tree));
			_tree->height = line[i] - '0';
			_tree->visible = 0;
			list_push(row, _tree);
		}
		list_push(map, row);
		read = getline(&line, &len, fp);
	}

	int count = 4, max = 0; // count is 4 from start for the corners corners
	
	for (int y = 1; y < map->size - 1; ++y) { // Go from left
		max = -1;
		for (int x = 0; x < map->size - 1; ++x) {
			Tree * tree = get_tree(map, x, y);
			if (max < tree->height) {
				max = tree->height;
				if (!tree->visible) {
					count++;
					tree->visible = 1;
				}
			}
		}

		max = -1;
		for (int x = map->size - 1; x > 0; --x) { // Go from right
			Tree * tree = get_tree(map, x, y);
			if (max < tree->height) {
				max = tree->height;
				if (!tree->visible) {
					count++;
					tree->visible = 1;
				}
			}
		}
	}

	for (int x = 1; x < map->size - 1; ++x) {
		max = -1;
		for (int y = map->size - 1; y > 0; --y) { // Go from below
			Tree * tree = get_tree(map, x, y);
			if (max < tree->height) {
				max = tree->height;
				if (!tree->visible) {
					count++;
					tree->visible = 1;
				}
			}
		}
		max = -1;
		for (int y = 0; y < map->size - 1; ++y) { // go from above
			Tree * tree = get_tree(map, x, y);
			if (max < tree->height) {
				max = tree->height;
				if (!tree->visible) {
					count++;
					tree->visible = 1;
				}
			}
		}
	}


	unsigned long end = micros();
	println("Count: {i}", count);
	println("Execution Time: {lu}", (end - start));

}
