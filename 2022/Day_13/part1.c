#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/vector.c"
#include "../../includes/list.c"

#define is_digit(x) ('0' <= (x) && (x) <= '9')
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef struct string {
	char * str;
	size_t length;
} String;

typedef struct pack {
	struct List * list;
	int number;
} Pack;

struct List * parse_packet();
int match();
int check();

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line1 = NULL, * line2 = NULL;
	size_t len1 = 0, len2 = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	int sum = 0;
	int i = 1;

	do {
		len1 = getline(&line1, &len1, fp);
		len2 = getline(&line2, &len2, fp);;
		int res = match(line1, len1, line2, len2);
		if (res)
			sum += i;
		++i;
	} while (getline(&line1, &len1, fp) != -1);


	unsigned long end = micros();
	println("Answer: {i}", sum);
	println("Execution Time: {lu}", (end - start));
}

int match(char * line1, size_t len1, char * line2, size_t len2) {
	
	int * start = malloc(sizeof(int));
	*start = 1;
	Pack * first = malloc(sizeof(Pack *));
	first->list = parse_packet(line1, start, len1);
	*start = 1;
	Pack * second = malloc(sizeof(Pack *));
	second->list = parse_packet(line2, start, len2);
	free(start);
	
	return check(first, second);
}

int check(Pack * left, Pack * right) {
	if (left->list == NULL && right->list == NULL) 
	{
		if (left->number != right->number)
			return left->number < right->number;
	} 
	else if (left->list != NULL && right->list != NULL)
	{ // both are list
		size_t min = MIN(left->list->size, right->list->size);

		for (size_t i = 0; i < min; ++i) {
			Pack * pack1 = list_at(left->list, i),
				 * pack2 = list_at(right->list, i);
			int res = check(pack1, pack2);
			if (res != -1)
				return res;
		}
		if (left->list->size != right->list->size)
			return left->list->size < right->list->size;
		return -1;
	}
	else if (left->list != NULL) // left is a list
	{ // left is a list but not right
		struct List * temp_list = init_list(sizeof(Pack *));
		list_push(temp_list, right);
		Pack * temp = malloc(sizeof(Pack));
		temp->list = temp_list;
		
		return check(left, temp);
	} 
	else if (right->list != NULL) // right is a list
	{ // right is list but not left
		struct List * temp_list = init_list(sizeof(Pack *));
		list_push(temp_list, left);
		Pack * temp = malloc(sizeof(Pack));
		temp->list = temp_list;
		
		return check(temp, right);
	}
	return -1;
}

struct List * parse_packet(char * line, int * index, size_t len) {

	struct List * packet = init_list(sizeof(Pack *));
	Pack * pack;
	for (int i = *index; i < len; ++i) {
		if (line[i] == '[') {
			pack = malloc(sizeof(Pack));
			*index = i + 1;
			pack->list = parse_packet(line, index, len);
			list_push(packet, pack);
			i = *index;
		} else if (line[i] == ']') {
			*index = i + 1;
			return packet;
		} else if (is_digit(line[i])) {
			char * next;
			pack = malloc(sizeof(Pack));
			pack->list = NULL;
			pack->number = strtol(line + i, &next, 10);
			list_push(packet, pack);
			i = next - line - 1;
		}
	}

	return packet;
}
