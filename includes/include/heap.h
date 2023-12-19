#pragma once

#include "common.h"
#include "heap_node.h"

typedef struct Heap {
	struct HeapNode ** list;
	int size;
	int capacity;
} Heap;

struct Heap * init_Heap(unsigned int capacity);
