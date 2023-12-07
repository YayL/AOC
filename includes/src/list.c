#include "list.h"

struct List * init_list(size_t item_size) {

	struct List * list = calloc(1, sizeof(struct List));
	list->size = 0;
	list->capacity = 0;
	list->item_size = item_size;
	
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
		list->items = calloc(1, list->item_size);
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

void _list_sort(struct List * list, char (*f)(void *, void *), int first, int last) {
    
    long i, j, pivot;
    void * temp;

    if (first >= last)
        return;

    pivot = first, i = first, j = last;

    while (i < j) {
        while (f(list->items[i], list->items[pivot]) && i < last) {
            i += 1;
        }
        
        while (!f(list->items[j], list->items[pivot]) && j > first) {
            j -= 1;
        }

        if (i < j) {
            temp = list->items[i];
            list->items[i] = list->items[j];
            list->items[j] = temp;
        }
    }

    temp = list->items[pivot];
    list->items[pivot] = list->items[j];
    list->items[j] = temp;

    _list_sort(list, f, first, j - 1);
    _list_sort(list, f, j + 1, last);
}

void list_sort(struct List * list, char (*f)(void *, void *)) {
    _list_sort(list, f, 0, list->size - 1);
}
