#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"

typedef struct tree {
	char height;
} Tree;

#define get_tree(map, x, y) ((Tree *)((struct List *)map->items[y])->items[x])

int getScenicScore(struct List * map, int x, int y) {
	int count = 0, house_height = get_tree(map, x, y)->height, score = 1;

	for (int _x = x - 1; _x >= 0; --_x) { // Go left
		Tree * tree = get_tree(map, _x, y);
		count++;
		if (house_height <= tree->height) {
			break;
		}
	}

	score *= count;
	count = 0;

	for (int _y = y + 1; _y < map->size; ++_y) { // Go down
		Tree * tree = get_tree(map, x, _y);
		count++;
		if (house_height <= tree->height) {
			break;
		}
	}

	score *= count;
	count = 0;

	for (int _x = x + 1; _x < map->size; ++_x) { // Go right
		Tree * tree = get_tree(map, _x, y);
		count++;
		if (house_height <= tree->height) {
			break;
		}
	}

	score *= count;
	count = 0;

	for (int _y = y - 1; _y >= 0; --_y) { // Go up
		Tree * tree = get_tree(map, x, _y);
		count++;
		if (house_height <= tree->height) {
			break;
		}
	}

	return score * count;

}

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
			list_push(row, _tree);
		}
		list_push(map, row);
		read = getline(&line, &len, fp);
	}

	int max = 0;

	for (int y = 0; y < map->size; ++y) {
		for (int x = 0; x < map->size; ++x) {
			int score = getScenicScore(map, x, y);
			if (max < score)
				max = score;
		}
	}


	unsigned long end = micros();
	println("Max score: {i}", max);
	println("Execution Time: {lu}", (end - start));

}
