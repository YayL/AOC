#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"

typedef struct position {
	int x;
	int y;
} Pos;

HashMap * visited;
Pos head, tail;

char * pos_to_string(Pos pos) {
	return format("{i}-{i}", pos.x, pos.y);
}

void update_tail(int _x, int _y) {
	int d_x = head.x - tail.x, d_y = head.y - tail.y;
	int distance = d_x * d_x + d_y * d_y;
	if (d_x < -1 || 1 < d_x || d_y < -1 || 1 < d_y) {
		tail.x = _x;
		tail.y = _y;
		char * key = pos_to_string(tail);
		HM_set(visited, key, 0);
		free(key);
	}
}

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input_uta.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	head.x = 0, head.y = 0, tail.x = 0, tail.y = 0;
	visited = new_HashMap(26);
	HM_set(visited, "0-0", 0);
	
	char dir;
	int length, _x, _y;

	while((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "%c %d", &dir, &length);
		while (length--) {
			_x = head.x, _y = head.y;
			switch (dir) {
				case 'R':
					++head.x;
					break;
				case 'L':
					--head.x;
					break;
				case 'U':
					++head.y;
					break;
				case 'D':
					--head.y;
					break;
			}
			update_tail(_x, _y);
		}
	}


	unsigned long end = micros();
	println("Size: {i}", visited->total);
	println("Execution Time: {lu}", (end - start));

}
