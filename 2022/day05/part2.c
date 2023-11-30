#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/vector.c"
#include "../../includes/string.c"

#define STACK_COUNT 9

void reverse_vector(struct Vector * list) {

	long * reversed_items = malloc(list->capacity * list->item_size);

	for (int i = list->size; i >= 1; --i) {
		reversed_items[list->size - i] = list->items[i - 1];
	}

	free(list->items);
	list->items = reversed_items;
}

void print_vector(struct Vector * vec) {

	for (int i = 0; i < vec->size; ++i) {
		putc(vec->items[i], stdout);
	}
	putc('\n', stdout);

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

	struct Vector ** stacks = calloc(STACK_COUNT, sizeof(struct Vector *));
	
	while((read = getline(&line, &len, fp)) != -1) {
		if (line[1] == '1') {
			getline(&line, &len, fp);
			break;
		}
		for (int i = 0; i < read - 1; ++i) {
			if (line[i] != ' ') {
				int stack = i >> 2;
				if (stacks[stack] == NULL) {
					stacks[stack] = init_vector();
				}
				vector_push(stacks[stack], line[i + 1]);
				i += 3;
			}
		}
	}

	for (int i = 0; i < STACK_COUNT; ++i) {
		reverse_vector(stacks[i]);
	}

	int count, src, dest;

	while ((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "move %d from %d to %d", &count, &src, &dest);

		int size = stacks[src - 1]->size;
		for (int i = 0; i < count; ++i) {
			vector_push(stacks[dest - 1], stacks[src - 1]->items[size - count + i]);
		}

		for (int i = 0; i < count; ++i) {
			vector_pop(stacks[src - 1]);
		}
	}

	unsigned long end = micros();

	print("Answer: ");
	for (int i = 0; i < STACK_COUNT; ++i) {
		print("{c}", vector_at(stacks[i], stacks[i]->size - 1));
	}

	println("\nExecution Time: {lu}", (end - start));

}
