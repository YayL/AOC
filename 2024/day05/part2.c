#include "common.h"
#include "fmt.h"
#include "list.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE 100
int rules[MAX_VALUE][MAX_VALUE] = {0};

int swap_invalid_updates(struct Vector * numbers) {
    char swapped = 0;

    for (size_t i = 0; i < numbers->size; ++i) {
        int value = vector_at(numbers, i);
        for (size_t j = i + 1; j < numbers->size; ++j) {
            int cmp = vector_at(numbers, j);
            if (rules[cmp][value] != 0) {
                swapped = 1;
                numbers->items[i] = cmp;
                numbers->items[j] = value;
                int temp = value;
                value = cmp;
                cmp = temp;
            }
        }
    }

    if (swapped) {
        return vector_at(numbers, numbers->size / 2);
    }

    return 0;
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

        result += swap_invalid_updates(numbers);

        length = 0;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
