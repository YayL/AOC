#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/ranges.c"
#include "../../includes/list.c"

#define WIDTH 7
#define BLOCK_COUNT 5

typedef struct pos {
	int x;
	int y;
} Pos;

typedef struct block {
	
	int c_x;
	int c_y;
	char stone_count;
	Pos * stones;

} Block;

void print_map();
char is_allowed();
char move();
int simulate();

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	
	Block * blocks[BLOCK_COUNT];

	blocks[0] = malloc(sizeof(Block));
	blocks[0]->stone_count = 4;
	blocks[0]->stones = malloc(sizeof(Pos) * blocks[0]->stone_count);
	blocks[0]->stones[0] = (Pos){-1,0};
	blocks[0]->stones[1] = (Pos){2,0};
	blocks[0]->stones[2] = (Pos){0,0};
	blocks[0]->stones[3] = (Pos){1,0};

	blocks[1] = malloc(sizeof(Block));
	blocks[1]->stone_count = 5;
	blocks[1]->stones = malloc(sizeof(Pos) * blocks[1]->stone_count);
	blocks[1]->stones[0] = (Pos){-1,1};
	blocks[1]->stones[1] = (Pos){1,1};
	blocks[1]->stones[2] = (Pos){0,2};
	blocks[1]->stones[3] = (Pos){0,1};
	blocks[1]->stones[4] = (Pos){0,0};

	blocks[2] = malloc(sizeof(Block));
	blocks[2]->stone_count = 5;
	blocks[2]->stones = malloc(sizeof(Pos) * blocks[2]->stone_count);
	blocks[2]->stones[0] = (Pos){-1,0};
	blocks[2]->stones[1] = (Pos){1,0};
	blocks[2]->stones[2] = (Pos){0,0};
	blocks[2]->stones[3] = (Pos){1,1};
	blocks[2]->stones[4] = (Pos){1,2};

	blocks[3] = malloc(sizeof(Block));
	blocks[3]->stone_count = 4;
	blocks[3]->stones = malloc(sizeof(Pos) * blocks[3]->stone_count);
	blocks[3]->stones[0] = (Pos){-1,3};
	blocks[3]->stones[1] = (Pos){-1,2};
	blocks[3]->stones[2] = (Pos){-1,1};
	blocks[3]->stones[3] = (Pos){-1,0};

	blocks[4] = malloc(sizeof(Block));
	blocks[4]->stone_count = 4;
	blocks[4]->stones = malloc(sizeof(Pos) * blocks[4]->stone_count);
	blocks[4]->stones[0] = (Pos){-1,1};
	blocks[4]->stones[1] = (Pos){0,1};
	blocks[4]->stones[2] = (Pos){-1,0};
	blocks[4]->stones[3] = (Pos){0,0};

	read = getline(&line, &len, fp) - 1;

	

	println("{i}", simulate(blocks, line, read, 2022));


	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}

int simulate(Block * blocks[BLOCK_COUNT], char * input, size_t in_length, int rounds) {

	int max_y = 0;
	Range * heights[WIDTH];
	for (int i = 0; i < WIDTH; ++i) {
		heights[i] = new_range(-1, -1);
	}
	
	int rock = -1, vent_index = 0, x, y;
	size_t height = 0;

	for (int i = 0; i < rounds; ++i) {
		rock = i % BLOCK_COUNT;
		Block * falling = malloc(sizeof(Block));
		memcpy(falling, blocks[rock], sizeof(Block));
		falling->stones = malloc(sizeof(Pos) * blocks[rock]->stone_count);
		for (int i = 0; i < blocks[rock]->stone_count; ++i) {
			falling->stones[i] = blocks[rock]->stones[i];
		}
		falling->c_y = height + 4;
		falling->c_x = WIDTH / 2;
		
		do {
			move(heights, falling, -2 * (input[vent_index] == '<') + 1, 0);
			vent_index = (vent_index + 1) % in_length;
		} while (move(heights, falling, 0, -1));

		for (int i_rock = 0; i_rock < falling->stone_count; ++i_rock) {
			x = falling->c_x + falling->stones[i_rock].x;
			y = falling->c_y + falling->stones[i_rock].y;
			if (height < y)
				height = y;
			range_insert_p(heights[x], y);
		}
		free(falling->stones);
		free(falling);
	}
	
	return height;
}

char move(Range * heights[], Block * item, int d_x, int d_y) {
	if (is_allowed(heights, item, d_x, d_y)) {
		item->c_x += d_x;
		item->c_y += d_y;
		return 1;
	}
	return 0;
}

char is_allowed(Range * heights[], Block * item, int d_x, int d_y) {
	int x, y;
	for (int i = 0; i < item->stone_count; ++i) {
		x = item->c_x + item->stones[i].x + d_x;
		y = item->c_y + item->stones[i].y + d_y;
		if (y <= 0 || x < 0 || x == WIDTH || range_is_intersect(heights[x], y)) {
			return 0;
		}
	}
	return 1;
}
