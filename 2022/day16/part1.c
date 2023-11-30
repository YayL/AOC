#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"
#include "../../includes/hashmap_int.c"
#include "../../includes/list.c"
#include "../../includes/vector.c"
#include "../../includes/priorityqueue_min.c"

#define MAX_MINUTES 30

typedef struct valve {
	char * name;
    int index;
	int rate;
	struct List * next_valves;
} Valve;

typedef struct pos {
	int score;
    int minute;
	Valve * valve;
    char * visited;
} Position;

#define MIN(x, y) ((x) < (y) ? (x) : (y))

unsigned int ** floydwarshall(struct List * nodes) {

    unsigned int nodes_count = nodes->size;
    unsigned int ** dist = malloc(sizeof(unsigned int *) * nodes_count);
    unsigned int def = 1 << 27;

    for (int i = 0; i < nodes_count; ++i) {
        unsigned int * row = malloc(sizeof(unsigned int) * nodes_count);
        for (int j = 0; j < nodes_count; ++j) {
            row[j] = (i == j) ? 0 : def;
        }
        dist[i] = row;
    }

    Valve * valve, * next;

    for (int i = 0; i < nodes_count; ++i) {
        valve = nodes->items[i];
        for (int j = 0; j < valve->next_valves->size; ++j) {
            next = valve->next_valves->items[j];
            dist[i][next->index] = 1;
        }
    }

    for (int k = 1; k <= nodes_count; ++k) {
        for (int i = 0; i < nodes_count; ++i) {
            for (int j = 0; j < nodes_count; ++j) {
                dist[i][j] = MIN(dist[i][j], dist[i][k - 1] + dist[k - 1][j]);
            }
        }
    }

    for (int i = 0; i < nodes_count; ++i) {
        valve = nodes->items[i];
        for (int j = 0; j < nodes_count; ++j) {
            next = nodes->items[j];
            if (!next->rate)
                dist[i][next->index] = 0;
        }
    }

    return dist;
}

int heuristic(Position * pos) {

    return pos->score * (MAX_MINUTES - pos->minute);

}

int main() {
	unsigned long __start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	HashMap * valves = new_HashMap(8);
    struct List * valve_list = init_list(sizeof(Valve *));

	char * key, found;
	Valve * valve, * temp;
	int i = 0, rate;

	while((read = getline(&line, &len, fp)) != -1) {
        if (line[0] == '\n')
            continue;
		sscanf(line, "Valve %ms has flow rate=%d; %*s %*s %*s %*s%n",&key,&rate,&i);
		if (HM_has(valves, key)) {
			valve = HM_get(valves, key);
		} else {
			valve = malloc(sizeof(Valve));
		}
		valve->name = key;
		valve->rate = rate;
		valve->next_valves = init_list(sizeof(Valve *));
        valve->index = valve_list->size;
        list_push(valve_list, valve);
		HM_set(valves, key, valve);
		++i;
		for (;i < read; i += 4) {
			key = malloc(sizeof(char) * 3);
			key[2] = 0;
			sscanf(line + i, "%c%c", key,key+1);
			if (HM_has(valves, key)) {
				temp = HM_get(valves, key);
			} else {
				temp = malloc(sizeof(Valve));
				HM_set(valves, key, temp);
			}
			list_push(valve->next_valves, temp);
		}
	}

    unsigned int ** distances = floydwarshall(valve_list);

    /* for (int y = 0; y < valve_list->size; ++y) { */
    /*     for (int x = 0; x < valve_list->size; ++x) { */
    /*         print("{i} | ", distances[y][x]); */
    /*     } */
    /*     putc('\n', stdout); */
    /* } */

    struct MinHeap * pq = init_MinHeap(16);
    Position * current = malloc(sizeof(Position)), * next;
    struct List * positions = init_list(sizeof(Position *));
    current->valve = HM_get(valves, "AA");
    current->visited = calloc(valve_list->size, sizeof(char));
    current->minute = 0;
    current->score = 0;

    println("Start: {s}", current->valve->name);
    
    int max_score = 0;
    list_reserve(positions, 64);
    list_push(positions, current);

    while (positions->size) {
        current = list_pop(positions);

        for (int i = 0; i < valve_list->size; ++i) {
            if (distances[current->valve->index][i] == 0 || current->visited[i])
                continue;
            
            int minute = current->minute + distances[current->valve->index][i] + 1;
            if (minute >= MAX_MINUTES)
                continue;

            next = malloc(sizeof(Position));
            next->valve = valve_list->items[i];
            next->visited = malloc(sizeof(char) * valve_list->size);
            memcpy(next->visited, current->visited, sizeof(char) * valve_list->size);
            next->visited[i] = 1;
            next->minute = minute;
            next->score = current->score + next->valve->rate * (MAX_MINUTES - minute);
            list_push(positions, next);
            if (max_score < next->score)
                max_score = next->score;
        }
    }

    
    println("Answer: {i}", max_score);

	unsigned long __end = micros();
	println("Execution Time: {lu}", (__end - __start));

}
