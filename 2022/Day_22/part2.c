#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/string.c"
#include "../../includes/list.c"
#include "../../includes/set.c"

typedef struct box {
	int x;
	int y;
	int r_x;
	int r_y;
} Box;

typedef struct face {
	struct face * left;
	struct face * right;
	struct face * up;
	struct face * down;
} Face;

int isqrt(long x) {
	unsigned int R = 0;
	unsigned int M;
	unsigned int E = x + 1;

    while( R != E - 1 )
    {
        M = (R + E) / 2;

		if( M * M <= x)
			R = M;
		else
			E = M;
	}

    return R;
}

struct List * map;
int max_x;

int main() {
	unsigned long __start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL, * temp_line;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	map = init_list(sizeof(String *));
	int offset, surface = 0, dim;
	String * str;
	max_x = 0;

	while((read = getline(&line, &len, fp)) != -1) {
		if (line[0] == '\n')
			break;
		else if (line[0] == '\t') {
			println("Tab... line: {i}", map->size);
			exit(1);
		}

		offset = 0;
		sscanf(line, "%*[^.#]%n", &offset);
		surface += (read - offset - 1);

		if (max_x < read - 1) {
			max_x = read - 1;
		}
		line[read - 1] = 0;
		str = new_string(line);
		list_push(map, str);
	}	

	println("Surface Area: {i}", surface);
	dim = isqrt(surface / 6);
	
	println("root: {i}", dim);

	return 0;

	read = getline(&line, &len, fp) - 1;
	
	Box box = {0};
	int dist, temp;
	box.r_x = 1;
	box.y = 0;
	str = map->items[0];
	for (int x = 0; x < str->length; ++x) {
		if (str->c_str[x] == '.') {
			box.x = x;
			break;
		}
	}

	Set * history = new_set(8);
	set_add(history, format("{i}-0", box.x));

	for (int i = 0; i < read;) {
		switch (line[i]) {

			case 'R':
				temp = box.r_x;
				box.r_x = -box.r_y;
				box.r_y = temp;
				++i;
				break;
			case 'L':
				temp = box.r_y;
				box.r_y = -box.r_x;
				box.r_x = temp;
				++i;
				break;
			default:
				dist = strtol(line + i, &temp_line, 10);
				i = temp_line - line;
				
				for (int j = 0; j < dist; ++j) {
					int n_x = box.x + box.r_x, n_y = box.y + box.r_y;
					
					if (box.r_x) {
						str = map->items[n_y];
						if (!(0 <= n_x && n_x < str->length) || str->c_str[n_x] == ' '){
							n_x = wrap(n_x, n_y, box.r_x, box.r_y);
						}
					}
					else {
						if (0 <= n_y && n_y < map->size) {
							str = map->items[n_y];
							if (n_x >= str->length || str->c_str[n_x] == ' ')
								n_y = wrap(n_x, n_y, box.r_x, box.r_y);
						} else {
							n_y = wrap(n_x, n_y, box.r_x, box.r_y);
						}
					}
					

					str = map->items[n_y];
					if (str->c_str[n_x] == '.') {
						box.x = n_x;
						box.y = n_y;
						char * key = format("{i}-{i}", box.x, box.y);
						set_add(history, key);
						free(key);
					} else if (str->c_str[n_x] == '#')
						break;
				}

				break;
		}
	}
	
	int facing;

	if (box.r_x == 1)
		facing = 0;
	else if (box.r_y == 1)
		facing = 1;
	else if (box.r_x == -1)
		facing = 2;
	else {
		facing = 3;
	}

	for (int y = 0; y < map->size; ++y) {
		str = map->items[y];
		for (int x = 0; x < str->length; ++x) {
			char * key = format("{i}-{i}", x, y);
			if (set_has(history, key)) {
				putc('O', stdout);
			} else {
				putc(str->c_str[x], stdout);
			}
		}
		putc('\n', stdout);
	}

	println("{i}, {i}, {i}", box.x, box.y, facing);
	println("Answer: {i}", 1000 * (box.y + 1) + 4 * (box.x + 1) + facing);

	unsigned long __end = micros();
	println("Execution Time: {lu}", (__end - __start));

}
