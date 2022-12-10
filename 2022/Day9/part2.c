#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"

#define ROPE_LENGTH 10

typedef struct position {
	int x;
	int y;
} Pos;

HashMap * visited;
Pos * rope;

char * pos_to_string(Pos pos) {
	char * str;
	asprintf(&str, "%d-%d", pos.x, pos.y);
	return str;
}

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input_utaL.txt", "r");	
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	rope = calloc(sizeof(Pos), ROPE_LENGTH);
	visited = new_HashMap(28);
	HM_set(visited, "0-0", 0);
	
	char dir;
	int length;

	while((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "%c %d", &dir, &length);
		while (length--) {
			switch (dir) {
				case 'R':
					++rope[0].x;
					break;
				case 'L':
					--rope[0].x;
					break;
				case 'U':
					++rope[0].y;
					break;
				case 'D':
					--rope[0].y;
					break;
			}
			for (int i = 1; i < ROPE_LENGTH; ++i) {
				int d_x = rope[i - 1].x - rope[i].x;
				int d_y = rope[i - 1].y - rope[i].y;
				if (d_x / 2 == 0 && d_y / 2 == 0)
					break;
				rope[i].x += (d_x + (d_x < 0 ? -1 : 1)) / 2;
				rope[i].y += (d_y + (d_y < 0 ? -1 : 1)) / 2;
				if (i == ROPE_LENGTH - 1) {
					char * key = pos_to_string(rope[i]);
					HM_set(visited, key, 0);
					free(key);
				}
			}
		}
	}
	
	unsigned long end = micros();
	println("Size: {i}", visited->total);
	println("Execution Time: {lu}", (end - start));

}
