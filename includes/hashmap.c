#pragma once
#include "fmt.c"
#include "string.c"

typedef struct pair {
	char * key;
	long value;
	struct pair * next;
} Pair;

typedef struct hashmap {
	Pair ** bucket_list;
	size_t capacity;
	size_t buckets;
	size_t total;
} HashMap;

HashMap * new_HashMap(size_t capacity) {

	HashMap * map = malloc(sizeof(HashMap));
	
	map->capacity = capacity;
	map->buckets = 0;
	map->total = 0;

	map->bucket_list = calloc(map->capacity, sizeof(Pair *));

	return map;
}

long HashCode(HashMap * map, const char * key) {

	const int p = (2 << 7) - 1;
	const int m = (2 << 24) - 1;
	
	long long hash_value = 0;
	long long p_pow = 1;
	int i = 0;

	while (key[i]) {
		hash_value += (hash_value + (key[i++] - ' ' + 1) * p_pow) & m;
		p_pow = (p_pow * p) & m;
	}
	
	return hash_value % map->capacity;
}

long HM_get(HashMap * map, const char * key) {
	Pair * current = map->bucket_list[HashCode(map, key)];

	while (current) {
		if (!strcmp(current->key, key))
			return current->value;
		current = current->next;
	}

	println("[HashMap]: Key '{s}' was not found", key);
	exit(1);
}

void HM_set(HashMap * map, const char * key, long value) {

	long long index = HashCode(map, key);
	Pair * current = map->bucket_list[index];

	while (current) {
		if (!strcmp(current->key, key)) {
			current->value = value;
			return;
		}
		current = current->next;
	}	
	
	size_t str_len = strlen(key);
	char * key_copy = malloc((str_len + 1) * sizeof(char));
	strcpy(key_copy, key);

    Pair * p = malloc(sizeof(Pair));
    p->key = key_copy;
    p->value = value;
    p->next = map->bucket_list[index];
	if (p->next == NULL)
		++map->buckets;
    map->bucket_list[index] = p;
    map->total++;
}

long long HM_remove(HashMap * map, const char * key) {
	
	long long index = HashCode(map, key);
	Pair * current = map->bucket_list[index], * temp;
	size_t depth = 0;

	while (current) {
		if (!strcmp(current->key, key))
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


	long long value = current->value;
	
	free(current->key);
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
			println("\t{s}: {Li}", current->key, current->value);
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
			free(current->key);
			free(current);
			current = next;
		}
	}
	
	free(map->bucket_list);
	free(map);
}

void MH_resize(HashMap * map, size_t capacity) {

	

}
