#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/priorityqueue.c"
#include "../../includes/list.c"
#include "../../includes/vector.c"

typedef struct pos {
	int x;
	int y;
} Pos;

#define get(x, y) (((char *)list_at(map, y))[x])
#define check(x, y) (((char *)list_at(visited, y))[x])

struct MH_Node * new_path(char c, int x, int y, struct MH_Node * shortest) {
	Pos * temp = malloc(sizeof(Pos));
	temp->x = x;
	temp->y = y;
			
	return init_mh_node(temp, NULL, 0, shortest->compare + 1);
}

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL, * row;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	struct MinHeap * pq = init_minheap(8);
	struct List * map = init_list(sizeof(char *)),
				* visited = init_list(sizeof(char *));

	int width = getline(&line, &len, fp) - 1;
	read = width + 1;

	while(read != -1) {
		row = malloc(sizeof(char) * read);
		memcpy(row, line, read);
		list_push(map, row);
		row = malloc(sizeof(char) * read);
		memset(row, 0, read);
		list_push(visited, row);
		read = getline(&line, &len, fp);
	}
	
	Pos * pos = malloc(sizeof(Pos));
	for (int y = 0; y < map->size; ++y) {
		for (int x = 0; x < width; ++x) {
			if (get(x, y) == 'a') {
				Pos * temp = malloc(sizeof(Pos));
				temp->x = x;
				temp->y = y;
				insert(pq, init_mh_node(temp, NULL, 0, 0));
			}
		}
	}
	struct MH_Node * shortest;
	char curr, val;

	while (1) {
		shortest = pop(pq);
		pos = shortest->ptr;
		curr = get(pos->x, pos->y);
		if (pos->x > 0) {
			if ((val = get(pos->x - 1, pos->y)) == 'E') {
				if (curr >= 'y')
					break;
			}
			else if ((val - curr) < 2) {
				if (check(pos->x - 1, pos->y) == 0) {
					insert(pq, new_path(curr, pos->x - 1, pos->y, shortest));
					check(pos->x - 1, pos->y) = 1;
				}
			}
		}
		if (pos->x < (width - 1)) {
			if ((val = get(pos->x + 1, pos->y)) == 'E') {
				if (curr >= 'y')
					break;
			}
			else if ((val - curr) < 2) {
				if (check(pos->x + 1, pos->y) == 0) {
					insert(pq, new_path(curr, pos->x + 1, pos->y, shortest));
					check(pos->x + 1, pos->y) = 1;
				}
			}
		}
		if (pos->y > 0) {
			if ((val = get(pos->x, pos->y - 1)) == 'E') {
				if (curr >= 'y')
					break;
			}
			else if ((val - curr) < 2) {
				if (check(pos->x, pos->y - 1) == 0) {
					insert(pq, new_path(curr, pos->x, pos->y - 1, shortest));
					check(pos->x, pos->y - 1) = 1;
				}
			}
		}
		if (pos->y < (map->size - 1)) {
			if ((val = get(pos->x, pos->y + 1)) == 'E') {
				if (curr >= 'y')
					break;
			}
			else if ((val - curr) < 2) {
				if (check(pos->x, pos->y + 1) == 0) {
					insert(pq, new_path(curr, pos->x, pos->y + 1, shortest));
					check(pos->x, pos->y + 1) = 1;
				}
			}
		}
	}

	unsigned long end = micros();
	println("Length: {i}", shortest->compare + 1);
	println("Execution Time: {lu}", (end - start));

}
