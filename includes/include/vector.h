#pragma once

#include "common.h"

struct Vector {
	long long * items;
	size_t size;
	size_t capacity;
	size_t item_size;
};

struct Vector * init_vector();

void free_vector(struct Vector * vector);
void vector_push(struct Vector * vector, long long item);

long vector_pop(struct Vector * vector);

void vector_shrink(struct Vector * vector, unsigned int new_size);

long vector_at(struct Vector * vector, size_t index);

void vector_reserve(struct Vector * vector, size_t size_to_reserve);

struct Vector * vector_copy(struct Vector * src);

char vector_contains(struct Vector * src, long number);

void vector_clear(struct Vector * vector);

void vector_print(struct Vector * vec);

void vector_sort(struct Vector * vec);
