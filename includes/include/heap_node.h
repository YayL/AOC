#pragma once

#include "common.h"

struct HeapNode {
    void * ptr;
    long compare;
};
#define MH_right(i) ((i << 1) + 1)
#define MH_left(i) ((i << 1) + 2)
#define MH_parent(i) ((i - 1) >> 1)

struct HeapNode * init_heap_node(void * value, long compare);
void free_heap_node(struct HeapNode * node);
