#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/vector.c"
#include "../../includes/list.c"
#include "../../includes/string.c"

#define is_digit(x) ('0' <= (x) && (x) <= '9')
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef struct pack {
	struct List * list;
	int number;
} Pack;

Pack * parse_packet();
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
	
	Pack * first, * second, * temp;

	String * string1 = new_string("[[2]]");
	String * string2 = new_string("[[6]]");

	int * _start = malloc(sizeof(int)), index1 = 1, index2 = 2;
	
	*_start = 1;
	first = parse_packet(string1->c_str, _start, string1->length + 1);
	*_start = 1;
	second = parse_packet(string2->c_str, _start, string2->length + 1);
	

	do {
		len1 = getline(&line1, &len1, fp);
		len2 = getline(&line2, &len2, fp);

		*_start = 1;
		temp = parse_packet(line1, _start, len1);
		if (check(temp, first))
			++index1;
		if (check(temp, second))
			++index2;

		*_start = 1;
		temp = parse_packet(line2, _start, len2);
		if (check(temp, first))
			++index1;
		if (check(temp, second))
			++index2;
	} while (getline(&line1, &len1, fp) != -1);	

	unsigned long end = micros();
	println("Answer: {i} * {i} = {i}", index1, index2, index1 * index2);
	println("Execution Time: {lu}", (end - start));
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

Pack * parse_packet(char * line, int * index, size_t len) {

	struct List * packet = init_list(sizeof(Pack *));
	Pack * pack;
	for (int i = *index; i < len; ++i) {
		if (line[i] == '[') {
			*index = i + 1;
			list_push(packet, parse_packet(line, index, len));
			i = *index;
		} else if (line[i] == ']') {
			*index = i + 1;
			pack = malloc(sizeof(Pack));
			pack->list = packet;
			return pack;
		} else if (is_digit(line[i])) {
			char * next;
			pack = malloc(sizeof(Pack));
			pack->list = NULL;
			pack->number = strtol(line + i, &next, 10);
			list_push(packet, pack);
			i = next - line - 1;
		}
	}
	
	pack = malloc(sizeof(Pack));
	pack->list = packet;

	return pack;
}
