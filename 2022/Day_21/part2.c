#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/hashmap.c"

typedef struct instruction {
	char has_value;
	long long value;
	char * left;
	char * right;
	char op;
	char is_human;
} Instruction;

long calculate (HashMap * map, Instruction * root, char is_left) {
	
	if (root->is_human) {
		print(" x ");
		return 0;
	}

	if (root->has_value) {
		print(" {i} ", root->value);
		return root->value;
	}

	print("(");
	long long left = calculate(map, HM_get(map, root->left), is_left);
	print("{c}", root->op);
	long long right = calculate(map, HM_get(map, root->right), !is_left);
	print(")");
	long long value;
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

	HashMap * map = new_HashMap(sizeof(Instruction *));
	Instruction * curr;
	char * key;

	while((read = getline(&line, &len, fp)) != -1) {
		curr = malloc(sizeof(Instruction));
		if ('0' <= line[6] && line[6] <= '9') {
			sscanf(line, "%4ms: %Ld", &key, &curr->value);
			curr->has_value = 1;
		} else {
			sscanf(line, "%4ms: %ms %c %ms", &key, &curr->left, &curr->op, &curr->right);
			curr->has_value = 0;
		}
		if (strcmp(key, "humn") == 0)
			curr->is_human = 1;
		HM_set(map, key, curr);
	}

	Instruction * root = HM_get(map, "root");

	long long left = calculate(map, HM_get(map, root->left), 1);
	print(" == ");
	long long right = calculate(map, HM_get(map, root->right), 0);
	println("\n{i} == {i}", left, right);
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
	
	println("{li}", value);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
