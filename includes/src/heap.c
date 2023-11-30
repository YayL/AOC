#include "heap.h"

struct Heap * init_Heap(unsigned int capacity) {
	struct Heap * heap = malloc(sizeof(struct Heap));
	heap->list = calloc(capacity, sizeof(struct HeapNode *));
	heap->capacity = capacity;
	heap->size = 0;
	return heap;
}
