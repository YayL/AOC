#pragma once
#include <stdlib.h>
#include <string.h>

struct MH_Node {
	void * ptr;
	char * history;
	size_t history_size;
	int compare;
};

struct MinHeap {
	struct MH_Node ** list;
	int size;
	int capacity;
};

#define MH_right(i) ((i << 1) + 1)
#define MH_left(i) ((i << 1) + 2)
#define MH_parent(i) ((i - 1) >> 1)

struct MH_Node * init_mh_node(void * value, char * history, size_t h_size, int compare) {
	struct MH_Node * ret = malloc(sizeof(struct MH_Node));
	ret->ptr = value;
	ret->history = history;
	ret->history_size = h_size;
	ret->compare = compare;
	return ret;
}

struct MinHeap * init_minheap(unsigned int capacity) {
	if (capacity == 0) {
		printf("Initial minheap capacity should be greater than 0");
		capacity = 1;
	}
	struct MinHeap * minheap = malloc(sizeof(struct MinHeap));
	minheap->list = calloc(capacity, sizeof(struct MH_Node *));
	minheap->capacity = capacity;
	minheap->size = 0;
	return minheap;
}

void heapify(struct MinHeap * heap) {
	if (heap == NULL) {
		puts("Min-Heap not initialized!");
		return;
	} else if (heap->size <= 0) {
		return;
	}
	int smallest = 0, index = 0, left, right;
	struct MH_Node * temp;

	heapify_loop_start:
		left = MH_left(index), right = MH_right(index);
		if (left < heap->size && heap->list[left]->compare < heap->list[index]->compare)
			smallest = left;
		if (right < heap->size && heap->list[right]->compare < heap->list[smallest]->compare)
			smallest = right;

		if (smallest == index)
			return;

		temp = heap->list[index];
		heap->list[index] = heap->list[smallest];
		heap->list[smallest] = temp;
		index = smallest;
		goto heapify_loop_start;
}

void insert(struct MinHeap * heap, struct MH_Node * value) {
	if (heap == NULL) {
		puts("Min-Heap not initialized!");
		return;
	}

	if (heap->size == heap->capacity) {	
		heap->capacity = heap->capacity << 1;
		heap->list = realloc(heap->list, sizeof(struct MH_Node) * heap->capacity);
	}

	int curr = heap->size;
	struct MH_Node * temp;

	heap->list[curr] = value;
	++heap->size;

	while (curr > 0 && (temp = heap->list[MH_parent(curr)])->compare > heap->list[curr]->compare) {
		heap->list[MH_parent(curr)] = heap->list[curr];
		heap->list[curr] = temp;
		curr = MH_parent(curr);
	}
}

struct MH_Node * pop(struct MinHeap * heap) {

	if (heap == NULL) {
		puts("Min-Heap not initialized!");
		return 0;
	}
	
	if (heap->size == 0)
		return 0;
	
	struct MH_Node * last_element = heap->list[heap->size - 1], * ret = heap->list[0];
	
	heap->list[heap->size - 1] = NULL;
	heap->list[0] = last_element;
	heap->size--;

	heapify(heap);
	return ret;
}

void free_mh_node(struct MH_Node * node) {
	if (node == NULL)
		return;
	
	if (node->history != NULL)
		free(node->history);

	free(node);
}

void free_minheap(struct MinHeap * minheap) {

	if (minheap == NULL)
		return;
	
	for (int i = 0; i < minheap->size; ++i) {
		free_mh_node(pop(minheap));
	}

	free(minheap->list);
	free(minheap);
}
