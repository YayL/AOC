#include "common.h"
#include "vector.h"
#include "list.h"

long long get_next_in_sequence(struct Vector * sequence) {
    struct List * differences = init_list(sizeof(struct Vector *));
    struct Vector * vec = sequence, * temp;
    char allzero = 0;

    while (!allzero) { // end when all are 0
        list_push(differences, vec);
        temp = init_vector();
        vector_reserve(temp, vec->size - 1);
        allzero = 1;
        for (int i = 1; i < vec->size; ++i) {
            vector_push(temp, vec->items[i] - vec->items[i - 1]);
            allzero &= !temp->items[i - 1];
        }
        vec = temp;
    }
    
    long long next_number = 0;

    for (int i = differences->size - 1; i >= 0; --i) {
        vec = list_at(differences, i);
        next_number = vec->items[0] - next_number;
    }

    return next_number;
}

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long number = 0, sum = 0;

    char * line = NULL, * endptr = NULL;
    size_t length = 0, read;

    struct Vector * sequence = init_vector();
    
    while ((line = NULL) || (read = getline(&line, &length, fp)) != -1) {
        vector_clear(sequence);
        do {
            number = strtol(line, &endptr, 10);
            line = endptr + 1;
            vector_push(sequence, number); 
        } while (line[0] != '\0');
        sum += get_next_in_sequence(sequence);
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {llu}", sum);
}
