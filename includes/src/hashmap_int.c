#include "hashmap_int.h"

HashMap_int * new_HashMap_int(size_t pow_capacity) {

	HashMap_int * map = malloc(sizeof(HashMap_int));
	
	map->capacity = (1 << pow_capacity) - 1;
	map->buckets = 0;
	map->total = 0;

	map->bucket_list = calloc(map->capacity + 1, sizeof(HM_Pair_int *));

	return map;
}

long HashCode_int(HashMap_int * map, long key) {
	return key & map->capacity;
}

long * HM_int_get(HashMap_int * map, long key) {
	HM_Pair_int * current = map->bucket_list[HashCode_int(map, key)];

	while (current) {
		if (current->key == key)
			return &current->value;
		current = current->next;
	}

	println("[HashMap_int]: Key '{li}' was not found", key);
	exit(1);
}

long HM_int_has(HashMap_int * map, long key) {
	HM_Pair_int * current = map->bucket_list[HashCode_int(map, key)];

	while (current) {
		if (current->key == key)
			return 1;
		current = current->next;
	}

	return 0;
}

void HM_int_set(HashMap_int * map, long key, long value) {

	long long index = HashCode_int(map, key);
	HM_Pair_int * current = map->bucket_list[index];

	while (current) {
		if (current->key == key) {
			current->value = value;
			return;
		}
		current = current->next;
	}	
	

    HM_Pair_int * p = malloc(sizeof(HM_Pair_int));
    p->key = key;
    p->value = value;
    p->next = map->bucket_list[index];
	if (p->next == NULL)
		++map->buckets;
    map->bucket_list[index] = p;
    map->total++;
}

long long HM_int_remove(HashMap_int * map, long key) {
	
	long long index = HashCode_int(map, key);
	HM_Pair_int * current = map->bucket_list[index], * temp;
	size_t depth = 0;

	while (current) {
		if (current->key == key)
			break;
		++depth;
		current = current->next;
	}
	
	if (current == NULL) {
		println("[HashMap_int]: Key '{s}' was not found", key);
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

void HM_int_print(HashMap_int * map) {
	HM_Pair_int * bucket, * current;
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

void HM_int_free(HashMap_int * map) {
	
	HM_Pair_int * current, * next;

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
