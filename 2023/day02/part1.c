#include "common.h"

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

int main() {
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read;
    
    int id, red, green, blue, offset, num, temp, sum = 0;

    char c;

    while ((read = getline(&line, &length, fp)) != -1) {
        red = green = blue = 0;
        sscanf(line, "Game %d:%n", &id, &offset);
        while (offset < read) {
            sscanf(line + ++offset, "%d%n", &num, &temp);
            c = line[offset += temp + 1];
            switch (c) {
                case 'r':
                    if (num > MAX_RED)
                        goto next;
                    offset += 4; 
                    break;
                case 'g':
                    if (num > MAX_GREEN)
                        goto next;
                    offset += 6;
                    break;
                case 'b':
                    if (num > MAX_BLUE)
                        goto next;
                    offset += 5;
                    break;
                default:
                    println("Uhoh! Unknown char({i}): {c}", offset, c);
                    exit(1);
            }

            
        }
        sum += id;
next:
    }
    println("Sum: {i}", sum);

}
