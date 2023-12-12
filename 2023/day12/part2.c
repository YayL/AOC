#include "common.h"
#include "vector.h"
#include <stdio.h>

struct Vector * groups;

void print_index(char * line, size_t index) {
    puts(line);
    for (int i = 0; i < index; ++i) {
        putchar(' ');
    }
    puts("^");
}

// this can be optimized to use a stack that keeps: index, last_dot and group_index
// it can be combined into one long or something to make it easier
size_t search_possible(char * line, size_t group_index, size_t count, size_t index) {
    while (1) {
        switch (line[index]) {
            case '\0':
            {
                if (count && group_index + 1 == groups->size && count == groups->items[group_index]) {
                    return 1;
                }
                
                if (group_index != groups->size) {
                    return 0;
                }

                return !count;
            }
            case '.':
            {
                if (line[index - 1] == '#') { // only false if this was a ? before
                    if (!(group_index < groups->size && count == groups->items[group_index])) {
                        return 0;
                    }
                    group_index += 1;
                    count = 0;
                }

                while (line[++index] == '.');
                break;
            }
            case '#':
            {
                count -= index;
                while (line[++index] == '#');
                count += index;
                break;
            }
            case '?':
                goto skip_while;
            default:
                println("\nInvalid characther: '{c}'", line[index]);
                exit(1);
        }
    }
skip_while:

    line[index] = '#';
    size_t possibilities = search_possible(line, group_index, count, index);

    line[index] = '.';
    possibilities += search_possible(line, group_index, count, index);

    line[index] = '?';

    return possibilities;
}

int main() {

    start_timer();
    FILE * fp = fopen("real_input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    char * line = NULL, * fixed_line;
    size_t length = 0, read, springs_length;

    int value, temp, i, row = 0;

    groups = init_vector();

    while ((read = getline(&line, &length, fp)) != -1) {
        vector_clear(groups);
        
        i = 0;
        while (line[i++] != ' ');
        springs_length = i;
        line[springs_length - 1] = '?';

        while (i < read) {
            sscanf(line + i, "%d%n", &value, &temp);
            i += temp + 1;
            vector_push(groups, value);
        }

        fixed_line = malloc(sizeof(char) * (5 * springs_length + 1));
        vector_reserve(groups, groups->size * 5);
        
        for (int i = 0; i < 5; ++i) {
            memcpy(fixed_line + springs_length * i, line, sizeof(char) * springs_length);
            memcpy(groups->items + groups->size * i, groups->items, sizeof(long long) * groups->size);
        }
        
        groups->size *= 5;
        springs_length = springs_length * 5 - 1;
        fixed_line[springs_length] = '\0';

        size_t res_temp = search_possible(fixed_line, 0, 0, 0);
        println("{i}: {llu}", ++row, res_temp);
        result += res_temp;
    }

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lli}", result);
}
