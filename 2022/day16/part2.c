#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"
#include "../../includes/hashmap_int.c"
#include "../../includes/list.c"
#include "../../includes/vector.c"
#include "../../includes/priorityqueue_min.c"

// Do not run...
// Took my input 35 minutes

#define MAX_MINUTES 26
#define PLAYER_COUNT 2

typedef struct valve {
	char * name;
    int index;
	int rate;
	struct List * next_valves;
} Valve;

typedef struct player {
    int minute;
    Valve * valve;
} Player;

typedef struct pos {
	int score;
    Player players[PLAYER_COUNT];
    char * visited;
} Position;

#define MIN(x, y) ((x) < (y) ? (x) : (y))

unsigned int ** get_shortest_paths(struct List * nodes) {

    unsigned int nodes_count = nodes->size;
    unsigned int ** dist = malloc(sizeof(unsigned int *) * nodes_count);
    unsigned int def = 1 << 27; // some high number
    
    // initialize all nodes to "INF" or 0 if path to self
    for (int i = 0; i < nodes_count; ++i) {
        unsigned int * row = malloc(sizeof(unsigned int) * nodes_count);
        for (int j = 0; j < nodes_count; ++j) {
            row[j] = (i == j) ? 0 : def;
        }
        dist[i] = row;
    }

    Valve * valve, * next;
    
    // set distances between direct sibling nodes
    for (int i = 0; i < nodes_count; ++i) {
        valve = nodes->items[i];
        for (int j = 0; j < valve->next_valves->size; ++j) {
            next = valve->next_valves->items[j];
            dist[i][next->index] = 1;
        }
    }

    // floydwarshall algo
    for (int k = 1; k <= nodes_count; ++k) {
        for (int i = 0; i < nodes_count; ++i) {
            for (int j = 0; j < nodes_count; ++j) {
                dist[i][j] = MIN(dist[i][j], dist[i][k - 1] + dist[k - 1][j]);
            }
        }
    }
    
    // setting all paths that go to a valve with rate of 0 to 0 so we can easily avoid it
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

    unsigned int ** distances = get_shortest_paths(valve_list);

    struct MinHeap * pq = init_MinHeap(16);
    Position * current = malloc(sizeof(Position)), * next;
    struct List * positions = init_list(sizeof(Position *));
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        current->players[i].valve = HM_get(valves, "AA");
        current->players[i].minute = 0;
    }
    current->visited = calloc(valve_list->size, sizeof(char));
    current->visited[current->players[0].valve->index] = 1;
    current->score = 0;
    
    int max_score = 0, minute = 0;
    list_reserve(positions, 64);
    list_push(positions, current);

    while (positions->size) {
        current = list_pop(positions);
        
        for (int i = 0; i < valve_list->size; ++i) {
            if (!distances[current->players[0].valve->index][i] || current->visited[i])
                continue;

            minute = current->players[0].minute + distances[current->players[0].valve->index][i] + 1;
            if (minute >= MAX_MINUTES)
                continue;

            next = malloc(sizeof(Position));
            
            next->visited = malloc(sizeof(char) * valve_list->size);
            memcpy(next->visited, current->visited, sizeof(char) * valve_list->size);
            next->visited[i] = 1;
            
            next->players[0].valve = valve_list->items[i];
            next->players[0].minute = minute;
            next->players[1] = current->players[1];
            next->score = current->score + next->players[0].valve->rate * (MAX_MINUTES - minute);

            if (max_score < next->score) {
                max_score = next->score;
            }

            list_push(positions, next);
        }

        for (int i = 0; i < valve_list->size; ++i) {
            if (!distances[current->players[1].valve->index][i] || current->visited[i])
                continue;

            minute = current->players[1].minute + distances[current->players[1].valve->index][i] + 1;
            if (minute >= MAX_MINUTES)
                continue;

            next = malloc(sizeof(Position));
            
            next->visited = malloc(sizeof(char) * valve_list->size);
            memcpy(next->visited, current->visited, sizeof(char) * valve_list->size);
            next->visited[i] = 1;

            next->players[0] = current->players[0];
            next->players[1].valve = valve_list->items[i];
            next->players[1].minute = minute;
            next->score = current->score + next->players[1].valve->rate * (MAX_MINUTES - minute);

            if (max_score < next->score) {
                max_score = next->score;
            }
            
            list_push(positions, next);
        }
        free(current->visited);
        free(current);
    }

    
    println("Answer: {i}", max_score);

	unsigned long __end = micros();
	println("Execution Time: {lu}", (__end - __start));

}
