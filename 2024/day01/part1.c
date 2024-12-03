#include "common.h"
#include "fmt.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

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

    struct Vector * list1 = init_vector(),
                  * list2 = init_vector(); 

    while ((read = getline(&line, &length, fp)) != -1) {
        char * end;
        vector_push(list1, strtol(line, &end, 10));
        vector_push(list2, strtol(end, NULL, 10));
    }

    vector_sort(list1);
    vector_sort(list2);

    for (size_t i = 0; i < list1->size; ++i) {
        long long diff = vector_at(list1, i) - vector_at(list2, i);
        if (diff < 0) {
            result -= diff;
        } else {
            result += diff;
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
