#pragma once
#include "common.h"

typedef struct deque {
	void** items;
	int start;
	int end;
	int size;
	size_t capacity;
	size_t item_size;
} Deque;

Deque * init_deque (size_t item_size);

size_t deque_find (Deque * deque, void * comp);

void deque_expand (Deque * deque);

void push_back (Deque * deque, void * item);

void push_front (Deque * deque, void * item);

void * pop_back (Deque * deque);

void * pop_front (Deque * deque);

void * deque_front (Deque * deque);

void * deque_back (Deque * deque);

void deque_rotate (Deque * deque, long long rotate);

void * deque_index (Deque * deque, size_t index);

void * deque_remove (Deque * deque, int index);

Deque * deque_copy (Deque * src);

void deque_print (Deque * deque, void (*print_item)());
