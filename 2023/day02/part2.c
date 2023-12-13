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
    
    int id, red = 0, green = 0, blue = 0, offset, num, temp, sum = 0;

    char c;

    while ((read = getline(&line, &length, fp)) != -1) {
        red = green = blue = 0;
        sscanf(line, "Game %d:%n", &id, &offset);
        while (offset < read) {
            sscanf(line + ++offset, "%d%n", &num, &temp);
            c = line[offset += temp + 1];

            switch (c) {
                case 'r':
                    if (num > red)
                        red = num;
                    offset += 4; 
                    break;
                case 'g':
                    if (num > green)
                        green = num;
                    offset += 6;
                    break;
                case 'b':
                    if (num > blue)
                        blue = num;
                    offset += 5;
                    break;
                default:
                    println("Uhoh! Unknown char({i}): {c}", offset, c);
                    exit(1);
            }   
        }
        sum += red * green * blue;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Sum: {i}", sum);
}
