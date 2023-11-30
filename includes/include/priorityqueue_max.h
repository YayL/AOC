#pragma once

#include "common.h"
#include "heap.h"
#include "heap_node.h"

void maxheap_heapify(struct Heap * heap);

void maxheap_insert(struct Heap * heap, struct HeapNode * value);

struct HeapNode * maxheap_pop(struct Heap * heap);

void free_maxheap(struct Heap * maxheap);
