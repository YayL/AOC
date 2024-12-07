#include "common.h"
#include "fmt.h"
#include <stdio.h>
#include <string.h>

#define WIDTH 130
#define HEIGHT 130

typedef struct guard {
    size_t x, y;
    enum direction {
        UP, RIGHT, DOWN, LEFT, DIRECTION_SIZE
    } dir;
} Guard;

char map[HEIGHT][WIDTH] = {0};
size_t height = 0, width = 0;

int guard_tick(Guard * guard) {
    switch (guard->dir) {
        case 1: case 3:
            guard->x += (int) (2 - guard->dir); break;
        case 0: case 2:
            guard->y += (int) (guard->dir - 1); break;
    }

    return guard->x < WIDTH && guard->y < HEIGHT;
}

void guard_inverse_tick(Guard * guard) {
    switch (guard->dir) {
        case 1: case 3:
            guard->x -= (int) (2 - guard->dir); break;
        case 0: case 2:
            guard->y -= (int) (guard->dir - 1); break;
    }
}

int guard_count_cycles(Guard guard) {
    char move_map[HEIGHT][WIDTH] = {0};
    int visited = 1;
    move_map[guard.y][guard.x] = 1 << guard.dir;

    while (guard_tick(&guard) && !(move_map[guard.y][guard.x] & (1 << guard.dir))) { 
        switch (map[guard.y][guard.x]) {
            default:
                if (move_map[guard.y][guard.x] == 0) {
                    visited += 1;
                }
                move_map[guard.y][guard.x] |= 1 << guard.dir;

                break;
            case '#':
                guard_inverse_tick(&guard);
                guard.dir = (guard.dir + 1) % (DIRECTION_SIZE);
                break;
        }
    }

    return visited;
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

    Guard guard;
    
    for (size_t y = 0; (read = getline(&line, &length, fp)) != -1; ++y) {
        memcpy(map[y], line, read * sizeof(char));
        if (width == 0) {
            width = read - 1;
            if (width != WIDTH) {
                println("WIDTH too small, increase to: {i}", width);
                exit(1);
            }
        }

        for (size_t x = 0; x < width; ++x) {
            if (line[x] != '.' && line[x] != '#') {
                guard.x = x; guard.y = y;
                switch (line[x]) {
                    case '^': guard.dir = UP; break;
                    case '>': guard.dir = RIGHT; break;
                    case 'v': guard.dir = DOWN; break;
                    case '<': guard.dir = LEFT; break;
                }
            }
        }

        height += 1;
    }

    if (height != HEIGHT) {
        println("HEIGHT too small, increase to: {i}", height);
        exit(1);
    }

    result = guard_count_cycles(guard);

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
