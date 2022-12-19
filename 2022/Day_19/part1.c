#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"

typedef struct blueprint {
	int id;
	int costs[4][3]; // [][0]: ore, [][1]: clay, [][2]: obsidian
} Blueprint;

typedef struct round {
	char minutes;
	char ores[4];
	char robots[4];
	char could_buy[4];
} Round;

// if i produce more than the ingrediants of a robot do not buy it

int can_buy(Blueprint * bp, Round * round, int index) {
	if (round->could_buy[index])
		return 0;
	else if (bp->costs[index][0] < round->robots[0] && 
			 bp->costs[index][1] < round->robots[1] &&
			 bp->costs[index][2] < round->robots[2])
		return 0;
	else if (bp->costs[index][0] > round->ores[0] - round->robots[0])
		return 0;
	else if (bp->costs[index][1] > round->ores[1] - round->robots[1])
		return 0;
	else if (bp->costs[index][2] > round->ores[2] - round->robots[2])
		return 0;
	return 1;
}

int get_quality(char * input) {
	
	Blueprint bp = {0};

	sscanf(input, "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d", &bp.id, &bp.costs[0][0], &bp.costs[1][0], &bp.costs[2][0], &bp.costs[2][1], &bp.costs[3][0], &bp.costs[3][2]);

	struct List * list = init_list(sizeof(Round *));
	list_reserve(list, 64);
	Round * round = calloc(1, sizeof(Round)), * temp;
	round->robots[0] = 1;
	list_push(list, round);	

	int max = 0;

	while (list->size) {
		round = list_pop(list);
		++round->minutes;
		round->ores[0] += round->robots[0];
		round->ores[1] += round->robots[1];
		round->ores[2] += round->robots[2];
		round->ores[3] += round->robots[3];
		if (round->minutes == 24) {
			if (max < round->ores[3])
				max = round->ores[3];
			free(round);
			continue;
		}
		if (can_buy(&bp, round, 0)) {
			temp = malloc(sizeof(Round));
			memcpy(temp, round, sizeof(Round));
			memset(temp->could_buy, 0, sizeof(temp->could_buy));
			++temp->robots[0];
			temp->ores[0] = temp->ores[0] - bp.costs[0][0];
			list_push(list, temp);
			round->could_buy[0] = 1;
		}
		if (can_buy(&bp, round, 1)) {
			temp = malloc(sizeof(Round));
			memcpy(temp, round, sizeof(Round));
			memset(temp->could_buy, 0, sizeof(temp->could_buy));
			++temp->robots[1];
			temp->ores[0] = temp->ores[0] - bp.costs[1][0];
			list_push(list, temp);
			round->could_buy[1] = 1;
		}
		if (can_buy(&bp, round, 2)) {
			temp = malloc(sizeof(Round));
			memcpy(temp, round, sizeof(Round));
			memset(temp->could_buy, 0, sizeof(temp->could_buy));
			++temp->robots[2];
			temp->ores[0] = temp->ores[0] - bp.costs[2][0];
			temp->ores[1] = temp->ores[1] - bp.costs[2][1];
			list_push(list, temp);
			round->could_buy[2] = 1;
		}
		if (can_buy(&bp, round, 3)) {
			temp = malloc(sizeof(Round));
			memcpy(temp, round, sizeof(Round));
			memset(temp->could_buy, 0, sizeof(temp->could_buy));
			++temp->robots[3];
			temp->ores[0] = temp->ores[0] - bp.costs[3][0];
			temp->ores[2] = temp->ores[2] - bp.costs[3][2];
			list_push(list, temp);
			round->could_buy[3] = 1;
		}
		list_push(list, round);
	}
	
	return max * bp.id;
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

	int sum;

	while ((read = getline(&line, &len, fp)) != -1) {
		sum += get_quality(line);
	}

	println("Answer: {i}", sum);

	unsigned long end = micros();
	println("Execution Time: {lu}us", (end - start));

}
