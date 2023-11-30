#pragma once

#include "common.h"

typedef struct HM_pair {
	char * key;
	void * value;
	struct HM_pair * next;
} HM_Pair;

typedef struct hashmap {
	HM_Pair ** bucket_list;
	size_t capacity;
	size_t buckets;
	size_t total;
} HashMap;

HashMap * new_HashMap(size_t pow_capacity);

long HM_HashCode(HashMap * map, const char * key);

void * HM_get(HashMap * map, const char * key);

long HM_has(HashMap * map, const char * key);

void HM_set(HashMap * map, char * key, void* value);

void * HM_remove(HashMap * map, const char * key);

void HM_print(HashMap * map);

void HM_clear (HashMap * map);

void HM_free(HashMap * map);
