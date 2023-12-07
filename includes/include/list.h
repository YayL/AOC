#pragma once

#include "common.h"

struct List {
	void** items;
	size_t size;
	size_t capacity;
	size_t item_size;
};

struct List * init_list(size_t item_size);

void free_list(struct List * list);

void list_push(struct List * list, void* item);

void * list_pop(struct List * list);

void list_shrink(struct List * list, unsigned int new_siz);

void* list_at(struct List * list, int index);

void list_reserve(struct List * list, unsigned int additions);

char list_contains (struct List * list, void * item);

void * list_copy(struct List * list);

void list_sort(struct List * list, int (*is_less_or_equal_to)(const void *, const void *));
