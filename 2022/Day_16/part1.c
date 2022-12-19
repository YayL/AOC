#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"
#include "../../includes/list.c"
#include "../../includes/priorityqueue_min.c"

#define MAX_MINUTES 30

typedef struct valve {
	char * name;
	int rate;
	struct List * next_valves;
} Valve;

typedef struct pos {
	int score;
	Valve * valve;
	struct List * opened;
	struct List * visited;
} Position;

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	HashMap * valves = new_HashMap(4);

	char * key;
	Valve * valve, * temp;
	int i = 0, rate;

	while((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "Valve %ms has flow rate=%d; %*s %*s %*s %*s%n",&key,&rate,&i);
		if (HM_has(valves, key)) {
			valve = (Valve *) *HM_get(valves, key);
			println("prev: {s}", key);
		} else {
			println("no: {s}", key);
			valve = malloc(sizeof(Valve));
		}
		valve->name = key;
		valve->rate = rate;
		valve->next_valves = init_list(sizeof(Valve *));
		HM_set(valves, key, valve);
		++i;
		for (;i < read; i += 4) {
			key = malloc(sizeof(char) * 3);
			key[2] = 0;
			sscanf(line + i, "%c%c", key,key+1);
			temp;
			if (HM_has(valves, key)) {
				temp = (Valve *) *HM_get(valves, key);
			} else {
				temp = malloc(sizeof(Valve));
				HM_set(valves, key, temp);
			}
			list_push(valve->next_valves, temp);
		}
	}

	struct MinHeap * heap = init_MinHeap(32);
	Position * curr = malloc(sizeof(Position)), * copy;
	curr->score = 0;
	curr->valve = (Valve *) *HM_get(valves, "AA");
	valve = curr->valve;
	curr->opened = init_list(sizeof(Valve *));
	curr->visited = init_list(sizeof(Valve *));
	struct MH_Node * node = init_mh_node(curr, 0);
	HashMap * visited;
	
	while (1) {
		
		if (!list_contains(curr->visited, valve)) {
			list_push(curr->visited, valve);
		}

		if (valve->rate) {	
				copy = malloc(sizeof(Position));
				memcpy(copy, curr, sizeof(Position));

				copy->opened = list_copy(copy->opened);
				copy->visited = list_copy(copy->visited);
				copy->score += (MAX_MINUTES - node->compare - 1) * valve->rate;

				list_push(copy->opened, valve);
				MinH_insert(heap, init_mh_node(copy, node->compare + 1));
		}
		for (int i = 0; i < valve->next_valves->size; ++i) {
			temp = (Valve* )list_at(valve->next_valves, i);
			if (!list_contains(curr->opened, temp)) {
				copy = malloc(sizeof(Position));
				memcpy(copy, curr, sizeof(Position));

				copy->opened = list_copy(copy->opened);
				copy->visited = list_copy(copy->visited);
				copy->valve = temp;
				
				MinH_insert(heap, init_mh_node(copy, node->compare + 1));
			}
		}

		loop:
		free(curr);
		free(node);
		node = MinH_pop(heap);
		curr = node->ptr;
		valve = curr->valve;
		if (node->compare == MAX_MINUTES) {
			println("MAX MIN");
			if (curr->visited->size == valves->total) {
				goto end;
			}
			goto loop;
		} else if (curr->visited->size == valves->total)
			println("ALL: {i} | {i}", node->compare, heap->size);
	}

	end:
	

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}

void print_valve() {
	
}
