#pragma once
#include "common.h"

#define VALUE_TYPE unsigned long long
#define KEY_TYPE unsigned long long

typedef struct HM_pair_int {
	KEY_TYPE key;
	VALUE_TYPE value;
	struct HM_pair_int * next;
} HM_Pair_int;

typedef struct hashmap_int {
	HM_Pair_int ** bucket_list;
	size_t capacity;
	size_t buckets;
	size_t total;
} HashMap_int;

HashMap_int * new_HashMap_int(size_t pow_capacity);

long HashCode_int(HashMap_int * map, KEY_TYPE key);

VALUE_TYPE HM_int_get(HashMap_int * map, KEY_TYPE key);

char HM_int_has(HashMap_int * map, KEY_TYPE key);

void HM_int_set(HashMap_int * map, KEY_TYPE key, VALUE_TYPE);

VALUE_TYPE HM_int_remove(HashMap_int * map, KEY_TYPE key);

void HM_int_print(HashMap_int * map);

void HM_int_free(HashMap_int * map);
