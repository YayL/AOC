#include "common.h"

#include "fmt.h"
#include "timer.h"
#include "ranges.h"
#include "hashmap.h"
#include "list.h"

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

#define Y_TO_CHECK 2000000

int main () {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
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
		char * key = format("{i}-{i}", b_x, b_y);
		if (HM_has(beacons, key)) {
			beacon = HM_get(beacons, key);
		} else {
			beacon = malloc(sizeof(Beacon));
			beacon->x = b_x;
			beacon->y = b_y;
			beacon->is_checked = 0;
			HM_set(beacons, key, beacon);
		}
		sensor->beacon = beacon;
		free(key);
		list_push(sensors, sensor);
	}
	
	int count = 0;
	Range * root = new_range(0, 0);

	for (int i = 0; i < sensors->size; ++i) {
		sensor = list_at(sensors, i);
		beacon = sensor->beacon;

		if (sensor->y <= Y_TO_CHECK && Y_TO_CHECK <= (sensor->y + sensor->dist)) 
		{ // above
			diff = sensor->dist + sensor->y - Y_TO_CHECK;
			range_insert(root, new_range(sensor->x - diff, sensor->x + diff));
			if (beacon->y == Y_TO_CHECK)
				range_bisect(root, beacon->x);
		}
		else if ((sensor->y - sensor->dist) <= Y_TO_CHECK && Y_TO_CHECK <= sensor->y) 
		{ // below
			diff = Y_TO_CHECK - sensor->y + sensor->dist;
			range_insert(root, new_range(sensor->x - diff, sensor->x + diff));
			if (beacon->y == Y_TO_CHECK)
				range_bisect(root, beacon->x);
		}
	}
	
	println("\n\nCount: {i}", range_get_width(root) - 1);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
