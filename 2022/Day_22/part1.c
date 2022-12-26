#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/string.c"
#include "../../includes/list.c"

typedef struct box {
	int x;
	int y;
	int r_x;
	int r_y;
} Box;

struct List * map;
int max_x;

int wrap (int x, int y, int r_x, int r_y) {
	String * row;
	if (r_x) {
		row = map->items[y];
		do {
			x -= r_x;
			if (row->c_str[x] == ' ')
				break;
		}
		while (0 <= x && x < row->length);
		return x + r_x;
	} else {
		y -= r_y;
		while (0 <= y && y < map->size) {
			row = map->items[y];
			if (row->length <= x || row->c_str[x] == ' ')
				break;
			y -= r_y;
		}
		return y + r_y;
	}
}

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
	String * str;
	max_x = 0;

	while((read = getline(&line, &len, fp)) != -1) {
		if (line[0] == '\n')
			break;
		else if (line[0] == '\t') {
			println("Tab... line: {i}", map->size);
			exit(1);
		}

		if (max_x < read - 1) {
			max_x = read - 1;
		}
		line[read - 1] = 0;
		str = new_string(line);
		list_push(map, str);
	}	

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

	println("Answer: {i}", 1000 * (box.y + 1) + 4 * (box.x + 1) + facing);

	unsigned long __end = micros();
	println("Execution Time: {lu}", (__end - __start));

}
