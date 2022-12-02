#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Vector {
	long * items;
	size_t size;
	size_t capacity;
	size_t item_size;
};

struct Vector * init_vector() {

	struct Vector * vector = calloc(1, sizeof(struct Vector));
	vector->size = 0;
	vector->capacity = 0;
	vector->item_size = sizeof(long);
	
	return vector;
}

void free_vector(struct Vector * vector) {
	free(vector->items);
	free(vector);

}

void vector_push(struct Vector * vector, long item) {
	if (!vector->size++) { // Incrementing here as it is just easier and not wasting a line for it
		vector->items = calloc(1, vector->item_size);
		vector->capacity = 1;
	}

	if(vector->size > vector->capacity){
		vector->items = realloc(vector->items, vector->size * vector->item_size);
		vector->capacity = vector->size;
	}

	vector->items[vector->size - 1] = item;
}

long vector_pop(struct Vector * vector) {
	if(!vector->size) {
		puts("[Error]: vector_pop() on size 0");
		exit(1);
	}
	
	return vector->items[vector->size--];
}

void vector_shrink(struct Vector * vector, unsigned int new_size) {	
	if (vector->size == 0)
		return;

	vector->size = new_size;
}

long vector_at(struct Vector * vector, size_t index) {

	if(0 <= index && index < vector->size)
		return vector->items[index];
	
	puts("[Error]: vector_at() Out Of Bounds");
	exit(1);
}

void vector_reserve(struct Vector * vector, unsigned int size_to_reserve) {
	vector->capacity += size_to_reserve;
	vector->items = realloc(vector->items, vector->capacity * vector->item_size);
}

struct Vector * vector_copy(struct Vector * src) {
	struct Vector * dest = init_vector();
	if (src == NULL || src->size == 0) {
		return dest;
	}

	dest->size = src->size;
	dest->item_size = src->item_size;
	vector_reserve(dest, dest->size);
	memcpy(dest->items, src->items, src->size * src->item_size);
	
	return dest;
}