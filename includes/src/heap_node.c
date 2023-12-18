#include "heap_node.h"

struct HeapNode * init_heap_node(void * value, long compare) {
    struct HeapNode * ret = malloc(sizeof(struct HeapNode));
    ret->ptr = value;
    ret->compare = compare;
    return ret;
}
void free_heap_node(struct HeapNode * node) {
    if (node == NULL)
        return;

    free(node);
}
