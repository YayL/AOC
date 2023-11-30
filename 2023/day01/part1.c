#include "fmt.h"

int main() {

    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
    }

    char * line = NULL;
    size_t length = 0, read;

    while ((read = getline(&line, &length, fp)) != -1) {
        println("Line: {s}", line);
    }
}
