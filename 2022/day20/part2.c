#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/deque.c"

// 15297
// 2897373276210

void _print_function (long val) {
	print("{li}, ", val);
}

long long sort(Deque * values) {

	Deque * indexes = init_deque(sizeof(size_t));
	for (size_t i = 0; i < values->size; ++i) {
		push_back(indexes, (void*) i);
	}
	
    long long index, value;
    
    for (int _ = 0; _ < 10; ++_) {
        for (size_t i = 0; i < indexes->size; ++i) {
            long position = deque_find(indexes, (void*) i);

            deque_rotate(values, -position);
            deque_rotate(indexes, -position);
            
            value = (long long) pop_front(values);
            index = (long long) pop_front(indexes);

            deque_rotate(values, -value);
            deque_rotate(indexes, -value); 

            push_front(values, (void*) value);
            push_front(indexes, (void*) index);
        }
    }

	size_t zero = deque_find(values, 0);

    long long a = (long long) deque_index(values, zero + 1000),
              b = (long long) deque_index(values, zero + 2000),
              c = (long long) deque_index(values, zero + 3000);

	println("[{Li}, {Li}, {Li}]", a, b, c);
	
	return a + b + c;
}

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	int size;
	Deque * deque = init_deque(sizeof(long long));


	while((read = getline(&line, &len, fp)) != -1) {
		push_back(deque, (void*) (strtol(line, (char **)NULL, 10) * 811589153));
	}

	long long sum = sort(deque);

	println("---\n{Li}", sum);

	unsigned long end = micros();
	println("Execution Time: {lu}", (end - start));

}
