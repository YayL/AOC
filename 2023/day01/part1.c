#include "common.h"

int main() {
    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read;
    
    int first = 0, last = -1, sum = 0;

    while ((read = getline(&line, &length, fp)) != -1) {
        for (int i = 0; i < read; ++i) {
            if (isdigit(line[i])) {
                if (first == 0)
                    first = line[i] - '0';
                last = line[i] - '0';
            }
        }

        sum += first * 10 + last;
        first = 0;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Number: {u}", sum);
}
