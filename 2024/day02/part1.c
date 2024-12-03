#include "common.h"
#include "fmt.h"
#include "vector.h"
#include <stdlib.h>

char is_safe(struct Vector * numbers) {
    char increasing = (vector_at(numbers, 1) - vector_at(numbers, 0)) > 0;

    size_t x = 0;
    for (x = 1; x < numbers->size; ++x) {
        ssize_t diff = vector_at(numbers, x) - vector_at(numbers, x - 1);
        if (!(diff != 0 && diff > 0 == increasing && -3 <= diff && diff <= 3)) {
            return 0;
        }
    }

    return 1;
}

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    char * line = NULL;
    size_t length = 0, read;
    
    struct Vector * row = init_vector();
    size_t rows = 0;

    while ((read = getline(&line, &length, fp)) != -1) {
        vector_push(row, atoll(line));

        for (size_t i = 1; line[i]; ++i) {
            if (line[i - 1] == ' ') {
                vector_push(row, atoll(line + i));
            }
        }

        result += is_safe(row);
        vector_clear(row);
    }
    
    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
