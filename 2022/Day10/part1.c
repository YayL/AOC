#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"

struct CPU {
	int X;
	int cycles;
	
};

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
	cpu.cycles = 0, cpu.X = 1;
	char  instruct;
	int arg, sum = 0, add;

	while((read = getline(&line, &len, fp)) != -1) {
		switch (line[0]) {
			case 'n':
				if (++cpu.cycles % 40 == 20) {
					sum += cpu.cycles * cpu.X;
				}
				break;
			case 'a':
				sscanf(line, "%*s %d", &add);
				for (int i = 0; i < 2; ++i) {
					if (++cpu.cycles % 40 == 20) {
						sum += cpu.cycles * cpu.X;
					}
				}
				cpu.X += add;
				break;
		}
	}


	unsigned long end = micros();
	println("Sum: {i}", sum);
	println("Execution Time: {lu}", (end - start));

}
