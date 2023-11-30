#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"
#include "../../includes/vector.c"
#include "../../includes/set.c"
#include "../../includes/priorityqueue_min.c"

#define MAX_MINUTES 1000

typedef struct pos {
	int x;
	int y;
} Pos;

typedef struct blizzard {
	Pos pos;
	Pos dir;
	char repr;
} Blizzard;

typedef struct path {
	int minute;
	Pos pos;
} Path;

struct List * map;
char *** visited;
int width, height;
Pos * end;

int find();

#define ABS(x) ((x) < 0 ? -(x) : (x))

int start_time;

int get_distance (Path * current) {
	return current->minute - start_time + (ABS(end->x - current->pos.x) + ABS(end->y - current->pos.y));
}

char is_blizzard (int x, int y, int minute) {
	struct List * m_map = map->items[minute];
	char c = ((char*)m_map->items[y])[x];
	if (c != '.' && c != '#') {
		return 1;
	}
	return 0;
}

Path * get_path (int x, int y, int minute) {
	if (minute == MAX_MINUTES || y >= height || visited[minute][y][x])
		return NULL;
	struct List * m_map = map->items[minute];
	if (((char*)m_map->items[y])[x] != '.')
		return NULL;

	Path * path = malloc(sizeof(Path));
	path->pos.x = x;
	path->pos.y = y;
	path->minute = minute + 1;
	visited[minute][y][x] = 1;
	return path;
}

void print_map(Path * current) {
	struct List * m_map = map->items[current->minute];
	for (int y = 0; y <= height; ++y) {
		for (int x = 0; x < width - 1; ++x) {
			if (x == current->pos.x && y == current->pos.y)
				putc('P', stdout);
			else if (x == end->x && y == end->y)
				putc('E', stdout);
			else
				putc(((char*)m_map->items[y])[x], stdout);
		}
		putc('\n', stdout);
	}
}

int main() {
	unsigned long __start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL, * copy;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	Blizzard * blizzard;
	char * key;
	struct List * blizzards = init_list(sizeof(Blizzard *));
	struct List * prev_map = init_list(sizeof(char *));

	height = 1, width = 0;
	list_push(prev_map, copy);

	while((read = getline(&line, &len, fp)) != -1) {
		if (line[0] == '\n')
			continue;
		
		++height;
		width = read;

		copy = malloc(sizeof(char) * (width + 1));
		memcpy(copy, line, width - 1);
		copy[width] = 0;
		list_push(prev_map, copy);

		for (int x = 1; x < width - 1; ++x) {
			if (line[x] == '#' || line[x] == '.')
				continue;
			blizzard = malloc(sizeof(Blizzard));
			switch (line[x]) {
				case '<':
					blizzard->dir.x = -1;
					blizzard->dir.y = 0;
					break;
				case '>':
					blizzard->dir.x = 1;
					blizzard->dir.y = 0;
					break;
				case 'v':
					blizzard->dir.x = 0;
					blizzard->dir.y = 1;
					break;
				case '^':
					blizzard->dir.x = 0;
					blizzard->dir.y = -1;
					break;
				default:
					println("UHOH!");
					return 0;
			}
			blizzard->pos.x = x;
			blizzard->pos.y = height - 1;
			blizzard->repr = line[x];
			list_push(blizzards, blizzard);
		}
	}

	copy = malloc(sizeof(char) * (width + 1));
	memset(copy, '#', width - 1);
	copy[width] = 0;
	prev_map->items[0] = copy;
	list_push(prev_map, copy);

	map = init_list(sizeof(struct List *));

	for (int min = 0; min < MAX_MINUTES; ++min) {
		struct List * c_map = init_list(sizeof(char *));
		list_reserve(c_map, prev_map->size);
		for (int i = 0; i < prev_map->size; ++i) {
			list_push(c_map, malloc(sizeof(char) * (width + 1)));
			memcpy(c_map->items[i], prev_map->items[i], width);
		}
		for (int i = 0; i < blizzards->size; ++i) {
			blizzard = blizzards->items[i];
			((char*)c_map->items[blizzard->pos.y])[blizzard->pos.x] = '.';
		}
		for (int i = 0; i < blizzards->size; ++i) {
			blizzard = blizzards->items[i];
			blizzard->pos.x += blizzard->dir.x;
			blizzard->pos.y += blizzard->dir.y;
			// width wrap
			if (blizzard->pos.x == 0)
				blizzard->pos.x = width - 3;
			else if (blizzard->pos.x == width - 2)
				blizzard->pos.x = 1;
			// height wrap
			else if (blizzard->pos.y == 1)
				blizzard->pos.y = height - 2;
			else if (blizzard->pos.y == height - 1)
				blizzard->pos.y = 2;
			((char*)c_map->items[blizzard->pos.y])[blizzard->pos.x] = blizzard->repr;
		}
		list_push(map, c_map);
		prev_map = c_map;
	}


	for (int i = 0; i < blizzards->size; ++i) {
		free(blizzards->items[i]);
	}
	free(blizzards->items);
	free(blizzards);

	end = malloc(sizeof(Pos));
	end->x = width - 3;
	end->y = height - 1;

	visited = malloc(sizeof(char **) * MAX_MINUTES);
	for (int i = 0; i < MAX_MINUTES; ++i) {
		visited[i] = malloc(sizeof(char *) * height);
		for (int y = 0; y < height; ++y) {
			visited[i][y] = malloc(sizeof(char) * width);
		}
	}

	Path * current = malloc(sizeof(Path));
	current->pos.x = 1;
	current->pos.y = 1;
	current->minute = 0;

	int first = find(current);
	println("First: {i}", first);

	current = malloc(sizeof(Path));
	current->pos.x = width - 3;
	current->pos.y = height - 1;
	current->minute = first;
	end->x = 1;
	end->y = 1;

	int second = find(current);
	println("Second: {i}", second);

	current = malloc(sizeof(Path));
	current->pos.x = 1;
	current->pos.y = 1;
	current->minute = second;
	end->x = width - 3;
	end->y = height - 1;

	int third = find(current);
	println("Answer: {i}", third);

	unsigned long __end = micros();
	println("Execution Time: {lu}", (__end - __start));

}

int find (Path * current) {

	start_time = current->minute;

	for (int i = 0; i < MAX_MINUTES; ++i) {
		for (int y = 0; y < height; ++y) {
			memset(visited[i][y], 0, width);
		}
	}

	struct MinHeap * paths = init_MinHeap(64);
	struct MH_Node * node = init_mh_node(current, get_distance(current));

	Path * next;

	while (1) {
		if (current->pos.x == end->x && current->pos.y == end->y) {
			free_minheap(paths);
			return current->minute;
		}
		
		next = get_path(current->pos.x + 1, current->pos.y, current->minute);
		if (next != NULL)
			MinH_insert(paths, init_mh_node(next, get_distance(next)));

		next = get_path(current->pos.x - 1, current->pos.y, current->minute);
		if (next != NULL)
			MinH_insert(paths, init_mh_node(next, get_distance(next)));

		next = get_path(current->pos.x, current->pos.y + 1, current->minute);
		if (next != NULL)
			MinH_insert(paths, init_mh_node(next, get_distance(next)));

		next = get_path(current->pos.x, current->pos.y - 1, current->minute);
		if (next != NULL)
			MinH_insert(paths, init_mh_node(next, get_distance(next)));

		next = get_path(current->pos.x, current->pos.y, current->minute);
		if (next != NULL)
			MinH_insert(paths, init_mh_node(next, get_distance(next)));

		free(current);
		free(node);
		node = MinH_pop(paths);
		current = node->ptr;
	}


	return 0;
}
