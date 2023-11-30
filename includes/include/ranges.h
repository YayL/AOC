#pragma once

#include "common.h"

typedef struct range {
	long long start;
	long long end;
	struct range * next;
} Range;

Range * new_range(long long start, long long end);

char range_is_intersect (Range * current, long long point);

Range * range_get_intersection (Range * current, long long point);

void range_sweep(Range * start);

void range_insert (Range * root, Range * range);

void range_insert_p (Range * root, int point);

void range_bisect(Range * root, long long point);

int range_get_width(Range * current);

void print_range(Range * current);
