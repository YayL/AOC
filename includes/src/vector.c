#include "vector.h"

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
	
	return vector->items[--vector->size];
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

void vector_clear(struct Vector * vector) {
    vector->size = 0;
}

char vector_contains(struct Vector * src, long number) {
    for (int i = 0; i < src->size; ++i) {
        if (src->items[i] == number) {
            return 1;
        }
    }

    return 0;
}

void vector_print(struct Vector * vec) {

	for (int i = 0; i < vec->size; ++i) {
		println("{i}: {li}", i, vec->items[i]);
	}

}

void _vector_sort(struct Vector * vec, int first, int last) {
    
    long i, j, pivot, temp;

    if (first >= last)
        return;

    pivot = first, i = first, j = last;

    while (i < j) {
        while (vec->items[i] <= vec->items[pivot] && i < last) {
            i += 1;
        }
        
        while (vec->items[j] > vec->items[pivot]) {
            j -= 1;
        }

        if (i < j) {
            temp = vec->items[i];
            vec->items[i] = vec->items[j];
            vec->items[j] = temp;
        }
    }

    temp = vec->items[pivot];
    vec->items[pivot] = vec->items[j];
    vec->items[j] = temp;

    _vector_sort(vec, first, j - 1);
    _vector_sort(vec, j + 1, last);
}

void vector_sort(struct Vector * vec) {
    _vector_sort(vec, 0, vec->size - 1);
}
