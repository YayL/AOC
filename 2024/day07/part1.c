#include "common.h"
#include "fmt.h"
#include "vector.h"
#include <stdlib.h>

typedef struct calibration {
    unsigned long long result;
    struct Vector * rhs;
} Calibration;

char evaluate(Calibration calibration, size_t index, unsigned long long result) {
    if (calibration.result < result) {
        return 0;
    }

    if (calibration.rhs->size == index) {
        return calibration.result == result;
    }


    long value = vector_at(calibration.rhs, index);
    unsigned long long mult = result * value;

    return evaluate(calibration, index + 1, result + value)
        || (mult / value == result && evaluate(calibration, index + 1, mult));
}

int main() {
    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    unsigned long long result = 0;

    char * line = NULL, * end, * next;
    size_t length = 0, read;

    Calibration calibration = {0};
    calibration.rhs = init_vector();
    
    while ((read = getline(&line, &length, fp)) != -1) {
        calibration.result = strtol(line, &end, 10);
        next = end + 2;
        vector_clear(calibration.rhs);

        while (end - line < read - 1) {
            vector_push(calibration.rhs, strtol(next, &end, 10));
            next = end + 1;
        }

        // start post first element
        if (evaluate(calibration, 1, vector_at(calibration.rhs, 0))) {
            result += calibration.result;
        }
    }

    

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
