#include "common.h"

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    unsigned long long result = 0, current = 0;

    char * line = NULL;
    size_t length = 0, read;

    read = getline(&line, &length, fp);

    for (int i = 0; i < read; ++i) {
        if (line[i] == ',' || line[i] == '\n') {
            result += current;
            current = 0;
            continue;
        }
        current += line[i];
        current *= 17;
        current &= 255;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
