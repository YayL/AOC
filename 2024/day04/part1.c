#include "common.h"
#include "fmt.h"
#include "list.h"
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>

#define WORD_LENGTH 4

size_t width = 0;
size_t height = 0;

char check_for_xmas(const char * str, const size_t index, const int dx, const int dy) {
    const int x = index % width, y = index / width;
    const int x_max = x + dx * (WORD_LENGTH - 1), y_max = y + dy * (WORD_LENGTH - 1);
    char next_char = 'X';

    if (!(0 <= x_max && x_max < width) || !(0 <= y_max && y_max < height)) {
        return 0;
    }

    for (size_t step = 1; step < WORD_LENGTH; ++step) {
        switch (next_char) {
            case 'X': next_char = 'M'; break;
            case 'M': next_char = 'A'; break;
            case 'A': next_char = 'S'; break;
            default: 
                println("Invalid char");
                return 0;
        }
        
        /* println("(x,y) = ({i}, {i}) | {i}", dx * step + x, dy * step + y, step); */

        if (str[index + dx * step + dy * step * width] != next_char) {
            return 0;
        }
    }

    return 1;
}

int count_xmas(const char * str, const size_t index) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            /* println("(dx, dy) = ({i}, {i})", dx, dy); */
            count += check_for_xmas(str, index, dx, dy);
        }
    }

    return count;
}

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

    String * source = NULL;

    while ((read = getline(&line, &length, fp)) != -1) {
        String * temp = new_string_len(line, read - 1);
        if (source == NULL) {
            source = temp;
            width = source->length;
        } else {
            string_concat(source, temp);
        }
        length = 0;
        height += 1;
    }

    for (size_t i = 0; i < source->length; ++i) {
        if (source->c_str[i] != 'X') {
            continue;
        }

        result += count_xmas(source->c_str, i);

    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
