#pragma once

#include "common.h"
#include "heap.h"
#include "heap_node.h"

void minheap_heapify(struct Heap * heap);

void minheap_insert(struct Heap * heap, struct HeapNode * value);

struct HeapNode * minheap_pop(struct Heap * heap);

void free_minheap(struct Heap * minheap);
