#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"

#define HEIGHT 1000
#define WIDTH 1000
#define is_digit(x) ('0' <= (x) && (x) <= '9')

typedef struct pos {
	int x;
	int y;
} Pos;

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	char map[HEIGHT][WIDTH] = {0};
	Pos pos1, pos2;

	int max_y = 0;

	while((read = getline(&line, &len, fp)) != -1) {
		int i = 0, temp;
		sscanf(line, "%d,%d%n", &pos1.x, &pos1.y, &i);
		while ((i + 1) < read) {
			sscanf(line + i, " -> %d,%d%n", &pos2.x, &pos2.y, &temp);
			i += temp;
			int d_x = (pos2.x - pos1.x),
				d_y = (pos2.y - pos1.y);
			int x1 = pos1.x, x2 = pos2.x, y1 = pos1.y, y2 = pos2.y;
			if (d_x < 0) 
				x1 = pos2.x, x2 = pos1.x;
			if (d_y < 0)
				y1 = pos2.y, y2 = pos1.y;

			if (d_x) 
				for (; x1 != x2; ++x1)
					map[y1][x1] = '#';
			else if (d_y)
				for (; y1 != y2; ++y1)
					map[y1][x1] = '#';
	
			map[y2][x2] = '#';
			pos1 = pos2;
			if (max_y < pos2.y)
				max_y = pos2.y;
		}
	}
	
	max_y += 2;

	for (int x = 0; x < WIDTH; ++x) {
		map[max_y][x] = '#';
	}

	int sand_count = 0;
	Pos sand;
	for (; sand.y != max_y; ++sand_count) {
		sand.x = 500, sand.y = 0;
		if (map[sand.y][sand.x] == 'O')
			break;
		while (sand.y != max_y) {
			++sand.y;
			if (map[sand.y][sand.x] == 0)
				continue;
			else if (map[sand.y][sand.x - 1] == 0)
				--sand.x;
			else if (map[sand.y][sand.x + 1] == 0)
				++sand.x;
			else {
				map[--sand.y][sand.x] = 'O';
				break;
			}
		}
	}

	println("Answer: {i}", sand_count);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
