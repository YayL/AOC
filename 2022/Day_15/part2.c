#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/ranges.c"
#include "../../includes/hashmap.c"
#include "../../includes/list.c"
#include "../../includes/string.c"

typedef struct beacon {
	int x;
	int y;
	char is_checked;
} Beacon;

typedef struct sensor {
	int x;
	int y;
	Beacon * beacon;
	int dist;
} Sensor;

#define CHAR_BIT 8
#define ABS_MASK(x) ((x) >> (sizeof(int) * CHAR_BIT - 1))
#define ABS(x) (((x) + ABS_MASK(x)) ^ ABS_MASK(x))
#define MIN(x, y) (y + ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1))))
#define MAX(x, y) (x - ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1))))

#define SEARCH_SPACE 4000000

int main () {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	char * key;
	Sensor * sensor;
	Beacon * beacon;

	HashMap * beacons = new_HashMap(8);
	struct List * sensors = init_list(sizeof(Sensor *));
	int b_x, b_y, diff;

	while ((read = getline(&line, &len, fp)) != -1) {
		sensor = malloc(sizeof(Sensor));
		sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
					&sensor->x, &sensor->y, &b_x, &b_y);
		
		sensor->dist = ABS(b_x - sensor->x) + ABS(b_y - sensor->y);
		key = format("{i}-{i}", b_x, b_y);
		if (HM_has(beacons, key)) {
			beacon = (Beacon *)HM_get(beacons, key);
		} else {
			beacon = malloc(sizeof(Beacon));
			beacon->x = b_x;
			beacon->y = b_y;
			HM_set(beacons, key, (long) beacon);
		}
		sensor->beacon = beacon;
		list_push(sensors, sensor);
	}
	
	int count = 0, y1, y2;
	Range * root;
	Range ** ranges = malloc(sizeof(Range *) * SEARCH_SPACE);

	for (int y = 0; y < SEARCH_SPACE; ++y) {
		ranges[y] = new_range(0, 0);
	}

	for (int i = 0; i < sensors->size; ++i) {
		sensor = list_at(sensors, i);

		for (int d_y = 0; d_y < sensor->dist; ++d_y) {
			diff = sensor->dist - d_y;
			y1 = sensor->y + d_y, y2 = sensor->y - d_y;
			if (0 <= y1 && y1 < SEARCH_SPACE) {
				range_insert(ranges[y1], 
							new_range(sensor->x - diff, sensor->x + diff));
			}
			if (0 <= y2 && y2 < SEARCH_SPACE) {
				range_insert(ranges[y2],
							new_range(sensor->x - diff, sensor->x + diff));
			}
		}	
	}

	for (int y = 0; y < SEARCH_SPACE; ++y) {
		root = ranges[y];
		if (root->next != NULL) {
			println("{i}, {i} == {Li}", root->end + 1, y, 4000000 * (root->end + 1) + y);
			break;
		}
	}

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
