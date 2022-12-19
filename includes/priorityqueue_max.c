#pragma once
#include <stdlib.h>
#include <string.h>

#ifndef MH_right
	struct MH_Node {
		void * ptr;
		int compare;
	};
	#define MH_right(i) ((i << 1) + 1)
	#define MH_left(i) ((i << 1) + 2)
	#define MH_parent(i) ((i - 1) >> 1)

	struct MH_Node * init_mh_node(void * value, int compare) {
		struct MH_Node * ret = malloc(sizeof(struct MH_Node));
		ret->ptr = value;
		ret->compare = compare;
		return ret;
	}
	void free_mh_node(struct MH_Node * node) {
		if (node == NULL)
			return;

		free(node);
	}
#endif

struct MaxHeap {
	struct MH_Node ** list;
	int size;
	int capacity;
};

struct MaxHeap * init_MaxHeap(unsigned int capacity) {
	struct MaxHeap * minheap = malloc(sizeof(struct MaxHeap));
	minheap->list = calloc(capacity, sizeof(struct MH_Node *));
	minheap->capacity = capacity;
	minheap->size = 0;
	return minheap;
}

void MaxH_heapify(struct MaxHeap * heap) {
	if (heap == NULL) {
		puts("MaxHeap not initialized!");
		return;
	} else if (heap->size <= 0) {
		return;
	}
	int biggest = 0, index = 0, left, right;
	struct MH_Node * temp;

	heapify_loop_start:
		left = MH_left(index), right = MH_right(index);
		if (left < heap->size && heap->list[left]->compare > heap->list[index]->compare)
			biggest = left;
		if (right < heap->size && heap->list[right]->compare > heap->list[biggest]->compare)
			biggest = right;

		if (biggest == index)
			return;

		temp = heap->list[index];
		heap->list[index] = heap->list[biggest];
		heap->list[biggest] = temp;
		index = biggest;
		goto heapify_loop_start;
}

void MaxH_insert(struct MaxHeap * heap, struct MH_Node * value) {
	if (heap == NULL) {
		puts("MaxHeap not initialized!");
		return;
	}

	if (heap->size == heap->capacity) {	
		heap->capacity = heap->capacity << 1;
		heap->list = realloc(heap->list, sizeof(struct MH_Node *) * heap->capacity);
	}

	int curr = heap->size;
	struct MH_Node * temp;

	heap->list[curr] = value;
	++heap->size;

	while (curr > 0 && (temp = heap->list[MH_parent(curr)])->compare < heap->list[curr]->compare) {
		heap->list[MH_parent(curr)] = heap->list[curr];
		heap->list[curr] = temp;
		curr = MH_parent(curr);
	}
}

struct MH_Node * MaxH_pop(struct MaxHeap * heap) {

	if (heap == NULL) {
		puts("MaxHeap not initialized!");
		return 0;
	}
	
	if (heap->size == 0)
		return 0;
	
	struct MH_Node * last_element = heap->list[heap->size - 1], * ret = heap->list[0];
	
	heap->list[heap->size - 1] = NULL;
	heap->list[0] = last_element;
	heap->size--;

	MaxH_heapify(heap);
	return ret;
}

void free_maxheap(struct MaxHeap * maxheap) {

	if (maxheap == NULL)
		return;
	
	for (int i = 0; i < maxheap->size; ++i) {
		free_mh_node(MaxH_pop(maxheap));
	}

	free(maxheap->list);
	free(maxheap);
}
