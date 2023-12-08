#pragma once

#include "common.h"
#include "heap_node.h"

struct Heap {
	struct HeapNode ** list;
	int size;
	int capacity;
};

struct Heap * init_Heap(unsigned int capacity);
