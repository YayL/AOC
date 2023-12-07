#include "list.h"

struct List * init_list(size_t item_size) {

	struct List * list = malloc(sizeof(struct List));
	list->size = 0;
	list->capacity = 0;
	list->item_size = item_size;
    list->items = NULL;
	
	return list;
}

void free_list(struct List * list) {
	void * item;
	for(size_t i = 0; i < list->size; ++i) {
		item = list->items[i];
		free(item);
	}
	free(list->items);
	free(list);

}

void list_push(struct List * list, void* item) {
	if (!list->size++) { // Incrementing here as it is just easier and not wasting a line for it
		list->items = malloc(list->item_size);
		list->capacity = 1;
	}

	if(list->size > list->capacity){
        list->capacity = list->size * 2;
		list->items = realloc(list->items, list->capacity * list->item_size);
	}

	list->items[list->size - 1] = item;
}

void * list_pop(struct List * list) {
	if(!list->size) 
		return NULL;

	void * temp = list->items[--list->size];
	list->items[list->size] = NULL;
	return temp;
}

void list_shrink(struct List * list, unsigned int new_size) {	
	if (list->size == 0)
		return;

	while (list->size != new_size) list_pop(list);

}

void* list_at(struct List * list, int index) {

	if(0 <= index && index < list->size)
		return list->items[index];

	return NULL;

}

void list_reserve(struct List * list, unsigned int additions) {
	
	list->capacity += additions;
	list->items = realloc(list->items, list->capacity * list->item_size);

}

char list_contains (struct List * list, void * item) {

	for (int i = 0; i < list->size; ++i) {
		if (list->items[i] == item)
			return 1;
	}
	return 0;
}

void * list_copy(struct List * list) {
	struct List * copy = init_list(list->item_size);
	if (list == NULL || list->size == 0) {
		return copy;
	}

	copy->size = list->size;
	list_reserve(copy, copy->size);	

	for (unsigned int i = 0; i < list->size; ++i) {
		copy->items[i] = list->items[i];
	}

	return copy;
}

void list_sort(struct List * list, int (*f)(const void *, const void *)) {
    void ** items = list->items, * value, * temp;
    long low = 0, high = list->size - 1, pivot, top = -1;
    long stack[(int)log2(high - low) * 5];
 
    stack[++top] = low;
    stack[++top] = high;

    while (top > 0) {
        high = stack[top--];
        low = stack[top--];

        // parition start

        value = items[high];
        pivot = low - 1;

        for (size_t j = low; j < high; ++j) {
            if (f(items[j], value)) {
                pivot += 1;
                temp = items[pivot];
                items[pivot] = items[j];
                items[j] = temp;
            }
        }

        pivot += 1;
        temp = items[pivot];
        items[pivot] = items[high];
        items[high] = temp;

        // partition end

        if (low < pivot - 1) {
            stack[++top] = low;
            stack[++top] = pivot - 1;
        }
        

        if (pivot + 1 < high) {
            stack[++top] = pivot + 1;
            stack[++top] = high;
        }
    }
}
