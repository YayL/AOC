#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"
#include "../../includes/set.c"
#include "../../includes/hashmap.c"

typedef struct elf {
	int x;
	int y;
	int p_x;
	int p_y;
	char * key;
} Elf;

#define TOTAL_ROUNDS 30

struct List * elves;
Set * grid;

char check (int x, int y) {
	char * key = format("{i}-{i}", x, y);
	char available = !set_has(grid, key);
	free(key);
	return available;
}

void print_elves () {
	Elf * elf = elves->items[0];
	int min_x = elf->x, max_x = elf->x, min_y = elf->y, max_y = elf->y;
	for (int i = 1; i < elves->size; ++i) {
		elf = elves->items[i];
		if (elf->x < min_x)
			min_x = elf->x;
		else if (max_x < elf->x)
			max_x = elf->x;
		if (elf->y < min_y)
			min_y = elf->y;
		else if (max_y < elf->y)
			max_y = elf->y;
	}	
	
	for (int y = min_y; y <= max_y; ++y) {
		for (int x = min_x; x <= max_x; ++x) {
			if (!check(x, y))
				putc('#', stdout);
			else
				putc('.', stdout);
		}
		putc('\n', stdout);
	}

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
	
	Elf * elf;
	elves = init_list(sizeof(Elf *));
	grid = new_set(8);
	struct List * moved = init_list(sizeof(Elf *));
	HashMap * proposed = new_HashMap(8);


	for (int y = 0; (read = getline(&line, &len, fp)) != -1; ++y) {
		if (line[0] == '\n')
			continue;
		for (int x = 0; x < read; ++x) {
			if (line[x] == '#') {
				elf = malloc(sizeof(Elf));
				elf->x = x;
				elf->y = y;
				elf->key = format("{i}-{i}", x, y);
				list_push(elves, elf);
				set_add(grid, elf->key);
			}
		}
	}
	list_reserve(moved, elves->size);
	int ROUND = 0;

	unsigned long middle = micros();

	for (; 1; ++ROUND) {
		int mod = ROUND % 4;
		int m = 0;
		for (int i = 0; i < elves->size; ++i) {
			elf = elves->items[i];
			int x = elf->x, y = elf->y;
			elf->p_x = x, elf->p_y = y;

			char dir[9] = { check(x - 1, y - 1), check(x, y - 1), check(x + 1, y - 1),
							check(x - 1, y),		  0,		  check(x + 1, y),
							check(x - 1, y + 1), check(x, y + 1), check(x + 1, y + 1)};
			// NORTH
			char n = dir[0] && dir[1] && dir[2];
			// WEST
			char w = dir[0] && dir[3] && dir[6];
			// EAST
			char e = dir[2] && dir[5] && dir[8];
			// SOUTH
			char s = dir[6] && dir[7] && dir[8];
			
			if (n && e && w && s) {
				continue;
			}
			m = 1;

			switch (mod) {
				case 0:
					if (n) elf->y -= 1;
					else if (s) elf->y += 1;
					else if (w) elf->x -= 1;
					else if (e) elf->x += 1;
					break;
				case 1:
					if (s) elf->y += 1;
					else if (w) elf->x -= 1;
					else if (e) elf->x += 1;
					else if (n) elf->y -= 1;
					break;
				case 2:
					if (w) elf->x -= 1;
					else if (e) elf->x += 1;
					else if (n) elf->y -= 1;
					else if (s) elf->y += 1;
					break;
				case 3:
					if (e) elf->x += 1;
					else if (n) elf->y -= 1;
					else if (s) elf->y += 1;
					else if (w) elf->x -= 1;
					break;
			}

			char * key = format("{i}-{i}", elf->x, elf->y);
			Elf * temp = HM_get(proposed, key);
			if (temp != NULL) {
				elf->x = elf->p_x, elf->y = elf->p_y;
				temp->x = temp->p_x, temp->y = temp->p_y;
				free(key);
			} else {
				list_push(moved, elf);
				HM_set(proposed, key, elf);
			}
		}
		if (m == 0) {
			goto the_end;
		}
		while (moved->size) {
			elf = list_pop(moved);
			if (set_has(grid, elf->key)) {
				set_remove(grid, elf->key);
			}
			free(elf->key);
			elf->key = format("{i}-{i}", elf->x, elf->y);
			set_add(grid, elf->key);
		}
		proposed = new_HashMap(8);
	}
	
	the_end:

	println("Round: {i}", ROUND + 1);
	unsigned long end = micros();
	println("Execution Time: {lu}us | {lu}us", (end - start), (end - middle));

}
