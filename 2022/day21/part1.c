#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"

typedef struct instruction {
	char has_value;
	long value;
	char * left;
	char * right;
	char op;
} Instruction;

long calculate (HashMap * map, Instruction * root) {

	if (root->has_value) {
		return root->value;
	}

	long left = calculate(map, HM_get(map, root->left));
	long right = calculate(map, HM_get(map, root->right));
	long value;
	switch (root->op) {
		case '+':
			value = left + right; break;
		case '-':
			value = left - right; break;
		case '/':
			value = left / right; break;
		case '*':
			value = left * right; break;
		default:
			println("ERROR");
			exit(1);
	}

	root->has_value = 1;
	root->value = value;
	return value;
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

	HashMap * instructions = new_HashMap(sizeof(Instruction *));
	Instruction * curr;
	char * key;

	while((read = getline(&line, &len, fp)) != -1) {
		curr = malloc(sizeof(Instruction));
		if ('0' <= line[6] && line[6] <= '9') {
			sscanf(line, "%4ms: %ld", &key, &curr->value);
			curr->has_value = 1;
		} else {
			sscanf(line, "%4ms: %ms %c %ms", &key, &curr->left, &curr->op, &curr->right);
			curr->has_value = 0;
		}
		HM_set(instructions, key, curr);
	}

	long answer = calculate(instructions, HM_get(instructions, "root"));
	
	println("{li}", answer);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
