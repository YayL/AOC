#include "priorityqueue_min.h"

void minheap_heapify(struct Heap * heap) {
	if (heap == NULL) {
		puts("Heap not initialized!");
		return;
	} else if (heap->size <= 0) {
		return;
	}
	int smallest = 0, index = 0, left, right;
	struct HeapNode * temp;

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

void minheap_insert(struct Heap * heap, struct HeapNode * value) {
	if (heap == NULL) {
		puts("Heap not initialized!");
		return;
	}

	if (heap->size == heap->capacity) {	
		heap->capacity = heap->capacity << 1;
		heap->list = realloc(heap->list, sizeof(struct HeapNode *) * heap->capacity);
	}

	int curr = heap->size++;
	struct HeapNode * temp;

	heap->list[curr] = value;

	while (curr > 0 && (temp = heap->list[MH_parent(curr)])->compare > heap->list[curr]->compare) {
		heap->list[MH_parent(curr)] = heap->list[curr];
		heap->list[curr] = temp;
		curr = MH_parent(curr);
	}
}

struct HeapNode * minheap_pop(struct Heap * heap) {

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

	minheap_heapify(heap);
	return ret;
}

void free_minheap(struct Heap * minheap) {

	if (minheap == NULL)
		return;
	
	for (int i = 0; i < minheap->size; ++i) {
		free_heap_node(minheap_pop(minheap));
	}

	free(minheap->list);
	free(minheap);
}
