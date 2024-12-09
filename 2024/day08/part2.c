#include "common.h"
#include "fmt.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct antenna {
    int x, y;
    char type;
} Antenna;

#define IS_INBOUNDS(x, y, width, height) (0 <= (x) && (x) < (width) && 0 <= (y) && (y) < (height))

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

    size_t width = 0, height = 0;
    
    struct List * antennas = init_list(sizeof(Antenna *));

    while ((read = getline(&line, &length, fp)) != -1) {
        if (width == 0) {
            width = read - 1;
        } else if (width != read - 1){
            println("Differing width...");
            exit(1);
        }

        for (size_t x = 0; x < width; ++x) {
            if (line[x] != '.') {
                Antenna * antenna = malloc(sizeof(*antenna));
                antenna->x = x;
                antenna->y = height;
                antenna->type = line[x];
                list_push(antennas, antenna);
            }
        }


        height += 1;
    }

    char antinode_map[height][width];
    memset(antinode_map, 0, sizeof(antinode_map));

    for (size_t i = 0; i < antennas->size; ++i) {
        Antenna * antenna1 = list_at(antennas, i);
        for (size_t j = i + 1; j < antennas->size; ++j) {
            Antenna * antenna2 = list_at(antennas, j);

            if (antenna1->type != antenna2->type) {
                continue;
            }

            int diff_x = antenna1->x - antenna2->x, diff_y = antenna1->y - antenna2->y;

            for (int x = antenna1->x, y = antenna1->y; IS_INBOUNDS(x, y, width, height); x += diff_x, y += diff_y) {
                if (!antinode_map[y][x]) {
                    result += antinode_map[y][x] = 1;
                }
            }


            for (int x = antenna2->x, y = antenna2->y; IS_INBOUNDS(x, y, width, height); x -= diff_x, y -= diff_y) {
                if (!antinode_map[y][x]) {
                    result += antinode_map[y][x] = 1;
                }
            }
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
