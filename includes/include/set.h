#pragma once

#include "common.h"

typedef struct pair {
	char * key;
	struct pair * next;
} Pair;

typedef struct set {
	Pair ** bucket_list;
	size_t capacity;
	size_t buckets;
	size_t total;
} Set;

Set * new_set(size_t pow_capacity);

long HashCode(Set * set, const char * key);

int set_has(Set * set, const char * key);

void set_add(Set * set, const char * key);

void set_remove(Set * set, const char * key);

void set_print(Set * set);

void set_clear (Set * set);

void set_free(Set * set);
