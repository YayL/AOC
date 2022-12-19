#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/ranges.c"
#include "../../includes/list.c"
#include "../../includes/hashmap.c"

#define WIDTH 7
#define BLOCK_COUNT 5
#define MAX_BLOCK_SIZE 5
#define TOTAL_ROUNDS 1000000000000

typedef struct pos {
	int x;
	int y;
} Pos;

typedef struct block {
	int c_x;
	int c_y;
	char stone_count;
	Pos stones[MAX_BLOCK_SIZE];
} Block;

typedef struct round {
	size_t heights[WIDTH]; // height differences
	long height;
	int i; // the round number
} Round;

void print_map();
char is_allowed();
char move();

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * input = NULL;
	size_t len = 0;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	Block blocks[BLOCK_COUNT] = {0}, falling;

	blocks[0].stone_count = 4;
	blocks[0].stones[0] = (Pos){-1,0};
	blocks[0].stones[1] = (Pos){2,0};
	blocks[0].stones[2] = (Pos){0,0};
	blocks[0].stones[3] = (Pos){1,0};

	blocks[1].stone_count = 5;
	blocks[1].stones[0] = (Pos){-1,1};
	blocks[1].stones[1] = (Pos){1,1};
	blocks[1].stones[2] = (Pos){0,2};
	blocks[1].stones[3] = (Pos){0,1};
	blocks[1].stones[4] = (Pos){0,0};

	blocks[2].stone_count = 5;
	blocks[2].stones[0] = (Pos){-1,0};
	blocks[2].stones[1] = (Pos){1,0};
	blocks[2].stones[2] = (Pos){0,0};
	blocks[2].stones[3] = (Pos){1,1};
	blocks[2].stones[4] = (Pos){1,2};

	blocks[3].stone_count = 4;
	blocks[3].stones[0] = (Pos){-1,3};
	blocks[3].stones[1] = (Pos){-1,2};
	blocks[3].stones[2] = (Pos){-1,1};
	blocks[3].stones[3] = (Pos){-1,0};

	blocks[4].stone_count = 4;
	blocks[4].stones[0] = (Pos){-1,1};
	blocks[4].stones[1] = (Pos){0,1};
	blocks[4].stones[2] = (Pos){-1,0};
	blocks[4].stones[3] = (Pos){0,0};

	size_t in_length = getline(&input, &len, fp) - 1;

	HashMap * played_rounds = new_HashMap(8);
	int rock = 0, vent_index = 0, x, y, cycle_start_index;
	Range * heights[WIDTH];
	Round * c_start, * c_end;
	size_t max_heights[WIDTH] = {0}, height = 0;
	for (int i = 0; i < WIDTH; ++i) {
		heights[i] = new_range(-1, -1);
	}
	

	for (int i = 0; 1; ++i) {
		falling = blocks[rock];
		falling.c_y = height + 4;
		falling.c_x = WIDTH / 2;
		
		do {
			move(heights, &falling, input[vent_index] == '<' ? -1 : 1, 0);
			vent_index = (vent_index + 1) % in_length;	
		} while (move(heights, &falling, 0, -1));

		for (int i_rock = 0; i_rock < falling.stone_count; ++i_rock) {
			x = falling.c_x + falling.stones[i_rock].x;
			y = falling.c_y + falling.stones[i_rock].y;
			if (height < y)
				height = y;
			if (max_heights[x] < y)
				max_heights[x] = y;
			range_insert_p(heights[x], y);
		}
		
		c_end = malloc(sizeof(Round));
		c_end->height = height - 1;
		c_end->i = i;
		for (int x = 0; x < WIDTH; ++x) {
			c_end->heights[x] = max_heights[x] - height;
		}
		char * key;
		asprintf(&key, "%d-%d", rock, vent_index);
		c_start = HM_get(played_rounds, key);
		if (c_start != NULL) {
			if (!memcmp(c_end->heights, c_start->heights, sizeof(c_start->heights))) {
				if ((TOTAL_ROUNDS - c_start->i) % (c_end->i - c_start->i) == 0) {
					cycle_start_index = c_start->i;
					goto end;
				}
			}
		}
		HM_set(played_rounds, key, c_end);
		rock = (rock + 1) % BLOCK_COUNT;
	}

	end:
	long cycle_length = c_end->i - cycle_start_index;
	
	long height1 = c_start->height;
	long height2 = c_end->height;
	long m = (TOTAL_ROUNDS - cycle_start_index) / cycle_length;

	long long answer = height1 + (height2 - height1) * m;
	println("Answer: {Li}", answer);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

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
