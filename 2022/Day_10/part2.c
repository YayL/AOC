#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/string.c"
#include "../../includes/list.c"

#define ABS(x) ((x) < 0 ? -(x) : (x))

struct CPU {
	int X;
	int cycles;
	int CRTrow;
	int CRTindex;
	struct List * list;
};

void next_cycle(struct CPU * cpu) {
	++cpu->cycles;
	((char *)cpu->list->items[cpu->CRTrow])[cpu->CRTindex] = 
		(ABS(cpu->X - cpu->CRTindex) < 2)
			? '#'
			: ' ';
	if (++cpu->CRTindex == 40) {
		++cpu->CRTrow;
		cpu->CRTindex = 0;
		char * str = malloc(sizeof(char) * 41);
		str[40] = 0;
		str[41] = 0;
		list_push(cpu->list, str);
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
	
	struct CPU cpu;
	cpu.cycles = 0, cpu.X = 1, cpu.CRTrow = 0, cpu.CRTindex = 0;
	cpu.list = init_list(sizeof(char *));
	list_reserve(cpu.list, 6);

	char * str = malloc(sizeof(char) * 41);
	str[40] = 0;
	str[41] = 0;
	list_push(cpu.list, str);

	char  instruct;
	int arg, sum = 0, add;

	while((read = getline(&line, &len, fp)) != -1) {
		switch (line[0]) {
			case 'n':
				next_cycle(&cpu);
				break;
			case 'a':
				sscanf(line, "%*s %d", &add);
				next_cycle(&cpu);
				next_cycle(&cpu);
				cpu.X += add;
				break;
		}
	}


	unsigned long end = micros();
	for (int i = 0; i < cpu.list->size; ++i) {
		println("{s}", cpu.list->items[i]);
	}
	println("Execution Time: {lu}", (end - start));

}
