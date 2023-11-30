#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"

int main() {

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL, opponent, me;
	size_t len = 0, read, score = 0;

	if (fp == NULL) {
		puts("File not found");
		exit(1);
	}

	while((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "%c %c", &opponent, &me);
		opponent -= 'A';
		me -= 'X';

		score += 3 * me + ((opponent + ((me + 2) % 3)) % 3) + 1;
	}
	
	println("Score: {i}", score);

}
