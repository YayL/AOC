#include "ranges.h"

Range * new_range(long long start, long long end) {
	Range * range = malloc(sizeof(Range));
	range->start = start;
	range->end = end;
	range->next = NULL;
	return range;
}

char range_is_intersect (Range * current, long long point) {
	while (current->start <= point) {
		if (point <= current->end)
			return 1;
		if (!current->next)
			return 0;
		current = current->next;
	}
	return 0;
}

Range * range_get_intersection (Range * current, long long point) {
	while (current->start <= point) {
		if (point <= current->end)
			return current;
		else if (!current->next)
			return NULL;
		current = current->next;
	}
	return NULL;
}

void range_sweep(Range * start) {
	Range * current = start, * next = NULL, * temp;
	
	while ((next = current->next) != NULL) {
		if (current->start <= next->start && next->end <= current->end) 
		{ // if next is contained in this current
			temp = next->next;
			free(next);
			current->next = temp;
		} else if (next->start <= (current->end + 1))
		{ // combined if next start is within current
			temp = next->next;
			current->end = next->end;
			free(next);
			current->next = temp;
		} else {
			current = current->next;
		}
	}
}

void range_insert (Range * root, Range * range) {
	Range * current = root, * next;
	while ((next = current->next) != NULL && next->start < range->start) {
		current = next;
	}

	if (current == root && range->start < root->start) {
		Range * temp = malloc(sizeof(Range));
		memcpy(temp, root, sizeof(Range));
		memcpy(root, range, sizeof(Range));
		root->next = temp;
	} else {
		range->next = current->next;
		current->next = range;
	}

	range_sweep(root);
}

void range_insert_p (Range * root, int point) {
	range_insert(root, new_range(point, point));
}

void range_bisect(Range * root, long long point) {

	Range * first = range_get_intersection(root, point);
	if (first == NULL)
		return;

	if (first->start == point) { // at the start
		++first->start;
	} else if (first->end == point) { // at the end
		--first->end;
	} else { // somewhere in the middle
		Range * second = malloc(sizeof(Range));
		second->next = first->next;
		second->end = first->end;
		second->start = point + 1;
		first->end = point - 1;
		first->next = second;
	}
}

int range_get_width(Range * current) {
	int width = 0;
	while (current != NULL) {
		width += (current->end - current->start) + 1;
		current = current->next;
	}
	return width;
}

void print_range(Range * current) {
	int i = 0;

	println("Range({i}):", range_get_width(current));
	_print_loop:
	println("\t{i}: {i} - {i}", ++i, current->start, current->end);
	if (current->next) {
		current = current->next;
		goto _print_loop;
	}
}
