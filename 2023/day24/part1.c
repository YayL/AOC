#include "common.h"

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
    

    while ((read = getline(&line, &length, fp)) != -1) {

    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
