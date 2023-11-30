#include "priorityqueue_max.h"

void maxheap_heapify(struct Heap * heap) {
	if (heap == NULL) {
		puts("Heap not icmake pass nitialized!");
		return;
	} else if (heap->size <= 0) {
		return;
	}
	int biggest = 0, index = 0, left, right;
	struct HeapNode * temp;

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

void maxheap_insert(struct Heap * heap, struct HeapNode * value) {
	if (heap == NULL) {
		puts("Heap not initialized!");
		return;
	}

	if (heap->size == heap->capacity) {	
		heap->capacity = heap->capacity << 1;
		heap->list = realloc(heap->list, sizeof(struct HeapNode *) * heap->capacity);
	}

	int curr = heap->size;
	struct HeapNode * temp;

	heap->list[curr] = value;
	++heap->size;

	while (curr > 0 && (temp = heap->list[MH_parent(curr)])->compare < heap->list[curr]->compare) {
		heap->list[MH_parent(curr)] = heap->list[curr];
		heap->list[curr] = temp;
		curr = MH_parent(curr);
	}
}

struct HeapNode * maxheap_pop(struct Heap * heap) {

	if (heap == NULL) {
		puts("Heap not initialized!");
		return 0;
	}
	
	if (heap->size == 0)
		return 0;
	
	struct HeapNode * last_element = heap->list[heap->size - 1], * ret = heap->list[0];
	
	heap->list[heap->size - 1] = NULL;
	heap->list[0] = last_element;
	heap->size--;

	maxheap_heapify(heap);
	return ret;
}

void free_maxheap(struct Heap * maxheap) {

	if (maxheap == NULL)
		return;
	
	for (int i = 0; i < maxheap->size; ++i) {
		free_heap_node(maxheap_pop(maxheap));
	}

	free(maxheap->list);
	free(maxheap);
}
