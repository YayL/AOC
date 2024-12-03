#include "vector.h"

#define ITEM_SIZE sizeof(long long)

struct Vector * init_vector() {
	struct Vector * vector = calloc(1, sizeof(struct Vector));
	return vector;
}

void free_vector(struct Vector * vector) {
	free(vector->items);
	free(vector);

}

void vector_push(struct Vector * vector, long long item) {
	if (!vector->capacity) {
		vector->items = malloc(ITEM_SIZE);
		vector->capacity = 1;
	} else if (vector->capacity <= vector->size) {
        vector->capacity = vector->size * 2;
		vector->items = realloc(vector->items, vector->capacity * ITEM_SIZE);
	}

	vector->items[vector->size++] = item;
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

void vector_reserve(struct Vector * vector, size_t size_to_reserve) {
    size_t value = vector->size + size_to_reserve;
    if (value < vector->capacity) {
        return;
    }

	vector->capacity = value;
	vector->items = realloc(vector->items, vector->capacity * ITEM_SIZE);
}

struct Vector * vector_copy(struct Vector * src) {
	struct Vector * dest = init_vector();
	if (src == NULL || src->size == 0) {
		return dest;
	}

	dest->size = src->size;
	vector_reserve(dest, dest->size); // capacity is set in vector_reserve
	memcpy(dest->items, src->items, src->size * ITEM_SIZE);
	
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

void vector_sort(struct Vector * vec) {
    long low = 0, high = vec->size - 1, value, temp, pivot, top = -1;
    long long * items = vec->items, stack[vec->size];
 
    stack[++top] = low;
    stack[++top] = high;

    while (top > 0) {
        high = stack[top--];
        low = stack[top--];

        // parition start

        value = items[high];
        pivot = low - 1;

        for (size_t j = low; j < high; ++j) {
            if (items[j] <= value) {
                pivot += 1;
                temp = items[pivot];
                items[pivot] = items[j];
                items[j] = temp;
            }
        }

        pivot += 1;
        temp = items[pivot];
        items[pivot] = items[high];
        items[high] = temp;

        // partition end

        if (low < pivot - 1) {
            stack[++top] = low;
            stack[++top] = pivot - 1;
        }
        

        if (pivot + 1 < high) {
            stack[++top] = pivot + 1;
            stack[++top] = high;
        }
    }
}

size_t vector_unique(struct Vector * vec) {
    if (vec->size == 0) {
        return 0;
    }

    struct Vector * copy = vector_copy(vec);

    /* println("Copy: {i}", copy->size); */

    vector_sort(copy);

    size_t count = 1;
    for (int i = 1; i < copy->size; ++i) {
        /* println("{c}, {c}", copy->items[i - 1] + 'A', copy->items[i] + 'A'); */
        count += copy->items[i - 1] != copy->items[i];
    }
    
    return count;
}

void vector_extend(struct Vector * dest, struct Vector * src) {
    dest->size += src->size;
    vector_reserve(dest, src->size); 
	memcpy(dest->items + src->size, src->items, src->size * ITEM_SIZE);
}
