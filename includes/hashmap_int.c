#pragma once
#include "fmt.c"
#include "string.c"

typedef struct pair {
	long key;
	long value;
	struct pair * next;
} Pair;

typedef struct hashmap {
	Pair ** bucket_list;
	size_t capacity;
	size_t buckets;
	size_t total;
} HashMap;

HashMap * new_HashMap(size_t pow_capacity) {

	HashMap * map = malloc(sizeof(HashMap));
	
	map->capacity = (1 << pow_capacity) - 1;
	map->buckets = 0;
	map->total = 0;

	map->bucket_list = calloc(map->capacity + 1, sizeof(Pair *));

	return map;
}

long HashCode(HashMap * map, long key) {
	return key & map->capacity;
}

long * HM_get(HashMap * map, long key) {
	Pair * current = map->bucket_list[HashCode(map, key)];

	while (current) {
		if (current->key == key)
			return &current->value;
		current = current->next;
	}

	println("[HashMap]: Key '{li}' was not found", key);
	exit(1);
}

long HM_has(HashMap * map, long key) {
	Pair * current = map->bucket_list[HashCode(map, key)];

	while (current) {
		if (current->key == key)
			return 1;
		current = current->next;
	}

	return 0;
}

void HM_set(HashMap * map, long key, long value) {

	long long index = HashCode(map, key);
	Pair * current = map->bucket_list[index];

	while (current) {
		if (current->key == key) {
			current->value = value;
			return;
		}
		current = current->next;
	}	
	

    Pair * p = malloc(sizeof(Pair));
    p->key = key;
    p->value = value;
    p->next = map->bucket_list[index];
	if (p->next == NULL)
		++map->buckets;
    map->bucket_list[index] = p;
    map->total++;
}

long long HM_remove(HashMap * map, long key) {
	
	long long index = HashCode(map, key);
	Pair * current = map->bucket_list[index], * temp;
	size_t depth = 0;

	while (current) {
		if (current->key == key)
			break;
		++depth;
		current = current->next;
	}
	
	if (current == NULL) {
		println("[HashMap]: Key '{s}' was not found", key);
		exit(1);
	}

	if (depth == 0) {
		map->bucket_list[index] = current->next;
	} else {
		temp = map->bucket_list[index];	
		for (int i = 0; i < depth - 1; ++i)
			temp = temp->next;
		temp->next = current->next;	
	}
	if (map->bucket_list[index] == NULL)
		--map->buckets;
	--map->total;


	long value = current->value;
	
	free(current);
	
	return value;
}

void HM_print(HashMap * map) {
	Pair * bucket, * current;
	println("[Buckets: {lu}, Total: {lu}]:", map->buckets, map->total);
	for (int i = 0; i < map->capacity; ++i) {
		bucket = map->bucket_list[i];
		current = bucket;
		while (current) {
			println("\t{li}: {Li}", current->key, current->value);
			current = current->next;
		}
	}
}

void HM_free(HashMap * map) {
	
	Pair * current, * next;

	for (int i = 0; i < map->capacity; ++i) {
		current = map->bucket_list[i];
		while (current) {
			next = current->next;
			free(current);
			current = next;
		}
	}
	
	free(map->bucket_list);
	free(map);
}
