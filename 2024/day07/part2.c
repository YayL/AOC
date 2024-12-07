#include "common.h"
#include "fmt.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

#define NUMERIC_TYPE unsigned long long

typedef struct calibration {
    NUMERIC_TYPE result;
    struct Vector * rhs;
} Calibration;

char evaluate(Calibration calibration, size_t index, NUMERIC_TYPE result) {
    if (calibration.result < result) {
        return 0;
    }

    if (calibration.rhs->size == index) {
        return calibration.result == result;
    }


    NUMERIC_TYPE value = vector_at(calibration.rhs, index);
    NUMERIC_TYPE mult = result * value;
    NUMERIC_TYPE comb_mult = pow(10, (NUMERIC_TYPE) log10(value) + 1);
    NUMERIC_TYPE combination = result * comb_mult + value;

    return evaluate(calibration, index + 1, result + value)
        || (index != 0 && mult / value == result && evaluate(calibration, index + 1, mult))
        || ((combination - value) / comb_mult == result && evaluate(calibration, index + 1, combination));
}

int main() {
    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    NUMERIC_TYPE result = 0;

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
        if (evaluate(calibration, 0, 0)) {
            result += calibration.result;
        }
    }

    

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
