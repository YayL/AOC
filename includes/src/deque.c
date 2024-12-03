#include "deque.h"

Deque * init_deque (size_t item_size) {

	Deque * deque = calloc(1, sizeof(Deque));
	deque->capacity = 1 << 2;
	deque->item_size = item_size;
	deque->start = 0;
	deque->end = 0;
	deque->items = malloc(item_size * deque->capacity);

	return deque;
}

size_t deque_find (Deque * deque, void * comp) {
	for (size_t i = deque->start, j = 0; i != deque->end; i = (i + 1) & (deque->capacity - 1)) {
		if (deque->items[i] == comp)
			return j;
		++j;
	}

	println("[Deque Error]: Unable to find element {li}", comp);
	exit(1);
}

void deque_expand (Deque * deque) {
	
	int previous_cap = deque->capacity;
	deque->capacity = deque->capacity << 1;
	deque->items =
        realloc(deque->items, deque->item_size * deque->capacity);
	if (deque->end < deque->start) {
		int length = previous_cap - deque->start;
		memmove(deque->items + deque->capacity - length, 
				 deque->items + deque->start, length * deque->item_size);
		deque->start = deque->capacity - length;
	}
}

void push_back (Deque * deque, void * item) {
	if (++deque->size == deque->capacity) {
		deque_expand(deque);
	}

	deque->items[deque->end] = item;
	deque->end = (deque->end + deque->capacity + 1) & (deque->capacity - 1);
}

void push_front (Deque * deque, void * item) {
    deque->start += 1;
	if (deque->size == deque->capacity) {
		deque_expand(deque);
	}
	
	deque->start = (deque->start + deque->capacity - 1) & (deque->capacity - 1);
	deque->items[deque->start] = item;
}

void * pop_back (Deque * deque) {

	if (!deque->size)
		return NULL;

	deque->size -= 1;
	deque->end = (deque->end + deque->capacity - 1) & (deque->capacity - 1);
	return deque->items[deque->end];
}

void * pop_front (Deque * deque) {
	if (!deque->size)
		return NULL;
	
	void * item = deque->items[deque->start];
	deque->size -= 1;
	deque->start = (deque->start + deque->capacity + 1) & (deque->capacity - 1);
	return item;
}

void * deque_front (Deque * deque) {
	if (!deque->size)
		return NULL;
	
	return deque->items[deque->start];
}

void * deque_back (Deque * deque) {
	if (!deque->size)
		return NULL;
	
	return deque->items[(deque->end + deque->capacity - 1) & (deque->capacity - 1)];
}

void deque_rotate (Deque * deque, long long rotate) {
    if (!rotate)
        return; 
    
    void ** items = malloc(deque->item_size * deque->capacity);

    if (deque->end < deque->start) {
        int offset = deque->capacity - deque->start;
        memcpy(items, deque->items + deque->start, deque->item_size * offset);
        memcpy(items + offset, deque->items, deque->item_size * deque->end);
    } else {
        memcpy(items, deque->items + deque->start, deque->item_size * deque->size);
    }
    
    rotate = (deque->size + ((deque->size + rotate) % deque->size)) % deque->size;

    memcpy(deque->items + rotate, items, deque->item_size * (deque->size - rotate));
    memcpy(deque->items, items + deque->size - rotate, deque->item_size * rotate);

    free(items);
    deque->start = 0;
    deque->end = deque->size;
}

void * deque_index (Deque * deque, size_t index) {
    if (deque->end < deque->start)
        deque_rotate(deque, deque->size);

    int offset = deque->end - deque->start;
    index = (offset + index) % offset;
    int new_index = (((offset + index) % (offset)) + deque->start) & (deque->capacity - 1);
    return deque->items[new_index];
}

void * deque_remove (Deque * deque, int index) {
	deque_rotate(deque, -index);
	void * item = pop_front(deque);
	deque_rotate(deque, index);
	return item;
}

Deque * deque_copy (Deque * src) {

	Deque * copy = init_deque(src->item_size);
	if (src == NULL) {
		return NULL;
	}
	memcpy(copy, src, sizeof(Deque));
	copy->items = malloc(src->item_size * src->capacity);
	
	if (src->size == 0) {
		return copy;
	}

	for (size_t i = 0, j = src->start; i < src->size; j = (j + 1) & (src->capacity - 1)) {
		copy->items[i++] = src->items[j];
	}
	copy->start = 0;
	copy->end = src->size;

	return copy;
}

void deque_print (Deque * deque, void (*print_item)()) {

	for (size_t i = deque->start; i != deque->end; i = (i + 1) & (deque->capacity - 1)) {
		print_item(deque->items[i]);
	}
	putc('\n', stdout);
}
