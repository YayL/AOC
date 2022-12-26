#pragma once
#include <stdlib.h>
#include <string.h>

#include "fmt.c"

typedef struct string {
	char const * c_str;
	size_t length;
} String;

String * new_string(const char * str) {

	String * new_str = malloc(sizeof(String));
	new_str->length = strlen(str);
	
	char * temp_str = malloc((new_str->length + 1) * sizeof(char));

	memcpy(temp_str, str, new_str->length);
	temp_str[new_str->length] = 0;
	new_str->c_str = temp_str;

	return new_str;
}

char string_at(String * src, size_t index) {
	if (index < src->length)
		return src->c_str[index];
	
	println("[String]: Index out of bounds");
	exit(1);
}

void string_concat(String * dest, String * src) {

	dest->length += src->length;
	
	char * temp_str = realloc((void *)dest->c_str, (dest->length + 1) * sizeof(char));
	memcpy((void *)(temp_str + (dest->length - src->length)), src->c_str, src->length);
	
	temp_str[dest->length + 1] = 0;
	dest->c_str = temp_str;
}

void string_append (String * dest, char c) {
	++dest->length;

	char * temp_str = realloc((void *)dest->c_str, (dest->length + 1) * sizeof(char));
	temp_str[dest->length - 1] = c;
	temp_str[dest->length] = 0;
	
	dest->c_str = temp_str;
}

void print_string(String * string) {
	println("{s}", string->c_str);
}

