#include "common.h"
#include "fmt.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    char * line = NULL, * end;
    size_t length = 0, read;
    
    struct Vector * nums = init_vector();

    read = getline(&line, &length, fp);

    char * next = line;
    do {
        long num = strtol(next, &end, 10);
        vector_push(nums, num);
        next = end + 1;
    } while (end - line < read - 1);

    for (size_t epoch = 0; epoch < 25; ++epoch) {
        size_t max_index = nums->size;
        for (size_t i = 0; i < max_index; ++i) {
            size_t value = vector_at(nums, i);

            if (value == 0) {
                nums->items[i] = 1;
                continue;
            }

            int digits = log10(value) + 1;

            if (digits % 2 == 0) {
                size_t half_pow = pow(10, (int) (digits / 2));
                nums->items[i] = value / half_pow;
                vector_push(nums, value % half_pow);
            } else {
                nums->items[i] = value * 2024;
            }
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", nums->size);
}
