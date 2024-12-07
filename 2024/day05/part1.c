#include "common.h"
#include "fmt.h"
#include "list.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE 100
int rules[MAX_VALUE][MAX_VALUE] = {0};

char is_valid_update(struct Vector * numbers) {
    for (size_t i = 0; i < numbers->size; ++i) {
        long value = vector_at(numbers, i);
        for (size_t j = i + 1; j < numbers->size; ++j) {
            long cmp = vector_at(numbers, j);
            if (rules[cmp][value] != 0) {
                return 0;
            }
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

    // parse rules
    while ((read = getline(&line, &length, fp)) != -1) {
        if (read == 1) {
            break;
        }

        int first, second;
        sscanf(line, "%d|%d", &first, &second);
        rules[first][second] = 1;

        length = 0;
    }

    struct Vector * numbers = init_vector();
    // parse updates
    while ((read = getline(&line, &length, fp)) != -1) {
        vector_clear(numbers);
        char * temp = line;
        while (temp < line + read) {
            vector_push(numbers, strtol(temp, &temp, 10));
            temp += 1;
        }

        if (is_valid_update(numbers)) {
            result += vector_at(numbers, numbers->size / 2);
        }

        length = 0;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
