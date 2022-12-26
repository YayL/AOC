#pragma once
#include "fmt.c"
#include "string.c"

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

Set * new_set(size_t pow_capacity) {

	Set * set = malloc(sizeof(Set));
	
	set->capacity = (1 << pow_capacity) - 1;
	set->buckets = 0;
	set->total = 0;

	set->bucket_list = calloc(set->capacity + 1, sizeof(Pair *));

	return set;
}

long HashCode(Set * set, const char * key) {

	const int p = (2 << 7) - 1;
	const int m = (2 << 24) - 1;
	
	long long hash_value = 0;
	long long p_pow = 1;
	int i = 0;

	while (key[i]) {
		hash_value += (hash_value + (key[i++] - ' ' + 1) * p_pow) & m;
		p_pow = (p_pow * p) & m;
	}
	
	return hash_value & set->capacity;
}

int set_has(Set * set, const char * key) {
	Pair * current = set->bucket_list[HashCode(set, key)];

	while (current) {
		if (!strcmp(current->key, key))
			return 1;
		current = current->next;
	}

	return 0;
}

void set_add(Set * set, const char * key) {

	long long index = HashCode(set, key);
	Pair * current = set->bucket_list[index];

	while (current) {
		if (!strcmp(current->key, key)) {
			return;
		}
		current = current->next;
	}	
	
	size_t str_len = strlen(key);
	char * key_copy = malloc((str_len + 1) * sizeof(char));
	strcpy(key_copy, key);

    Pair * p = malloc(sizeof(Pair));
    p->key = key_copy;
    p->next = set->bucket_list[index];
	if (p->next == NULL)
		++set->buckets;
    set->bucket_list[index] = p;
    set->total++;
}

void set_remove(Set * set, const char * key) {
	
	long long index = HashCode(set, key);
	Pair * current = set->bucket_list[index], * temp;
	size_t depth = 0;

	while (current) {
		if (!strcmp(current->key, key))
			break;
		++depth;
		current = current->next;
	}
	
	if (current == NULL) {
		println("[Set]: Key '{s}' was not found", key);
		exit(1);
	}

	if (depth == 0) {
		set->bucket_list[index] = current->next;
	} else {
		temp = set->bucket_list[index];	
		for (int i = 0; i < depth - 1; ++i)
			temp = temp->next;
		temp->next = current->next;	
	}
	if (set->bucket_list[index] == NULL)
		--set->buckets;
	--set->total;
	
	free(current->key);
	free(current);
}

void set_print(Set * set) {
	Pair * bucket, * current;
	println("[Buckets: {lu}, Total: {lu}]:", set->buckets, set->total);
	for (int i = 0; i < set->capacity; ++i) {
		bucket = set->bucket_list[i];
		current = bucket;
		while (current) {
			println("\t{s}", current->key);
			current = current->next;
		}
	}
}

void set_clear (Set * set) {

	Pair * current, * next;
	for (int i = 0; i < set->capacity; ++i) {
		current = set->bucket_list[i];
		while (current) {
			next = current->next;
			free(current->key);
			free(current);
			current = next;
			--set->total;
		}
		set->bucket_list[i] = NULL;
	}
}

void set_free(Set * set) {
	
	Pair * current, * next;

	for (int i = 0; i < set->capacity; ++i) {
		current = set->bucket_list[i];
		while (current) {
			next = current->next;
			free(current->key);
			free(current);
			current = next;
		}
	}
	
	free(set->bucket_list);
	free(set);
}
