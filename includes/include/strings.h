#pragma once

#include "common.h"

typedef struct string {
	char const * c_str;
	size_t length;
} String;

String * new_string_len(const char * str, size_t length);
String * new_string(const char * str);

char string_at(String * src, size_t index);

void string_concat(String * dest, String * src);

void string_append (String * dest, char c);

void print_string(String * string);

