#include "common.h"
#include "fmt.h"
#include <stdio.h>
#include <string.h>

// answer < 1354

#define WIDTH 130
#define HEIGHT 130

typedef struct guard {
    size_t x, y;
    enum direction {
        UP, RIGHT, DOWN, LEFT, DIRECTION_SIZE
    } dir;
} Guard;

char map[HEIGHT][WIDTH] = {0};
char obstruction_map[HEIGHT][WIDTH] = {0};
size_t height = 0, width = 0;

void print_map(char movement_map[HEIGHT][WIDTH]) {
    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            if (movement_map[y][x]) {
                putchar('X');
            } else {
                switch (map[y][x]) {
                    case '.':
                        putchar('.'); break;
                    case '#':
                        putchar('#'); break;
                }
            }
        }
        puts("");
    }
}

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

char is_guard_cycle(Guard guard) {
    char move_map[HEIGHT][WIDTH] = {0};
    move_map[guard.y][guard.x] = 1 << guard.dir;

    while (1) {
        if (!guard_tick(&guard)) { // is out of bounds
            return 0; // does not cycle
        } else if (move_map[guard.y][guard.x] & (1 << guard.dir)) {
            return 1; // cycle detected
        }
 
        switch (map[guard.y][guard.x]) {
            default:
                move_map[guard.y][guard.x] |= 1 << guard.dir;
                break;
            case '#':
                guard_inverse_tick(&guard);
                guard.dir = (guard.dir + 1) % (DIRECTION_SIZE);
                break;
        }
    }
}

int guard_count_cycles(Guard guard) {
    char move_map[HEIGHT][WIDTH] = {0};
    int cycles = 0;
    move_map[guard.y][guard.x] = 1 << guard.dir;

    while (guard_tick(&guard) && !(move_map[guard.y][guard.x] & (1 << guard.dir))) { 
        switch (map[guard.y][guard.x]) {
            default:
                move_map[guard.y][guard.x] |= 1 << guard.dir;
                break;
            case '#':
                guard_inverse_tick(&guard);
                guard.dir = (guard.dir + 1) % (DIRECTION_SIZE);
                break;
        }
        
        // check if obstacle infront would cause a cycle

        Guard ghost_guard = guard;
        // is out of bounds or if guard has been here on it's stroll
        if (!guard_tick(&ghost_guard) || move_map[ghost_guard.y][ghost_guard.x]) {
            continue;
        }
        size_t x = ghost_guard.x, y = ghost_guard.y;

        // check if cycle structure has already been checked
        if (obstruction_map[y][x] == 0 && (map[y][x] == '#' || map[y][x] == '.')) {
            ghost_guard = guard;
            ghost_guard.dir = (guard.dir + 1) % DIRECTION_SIZE;
            char c = map[y][x];
            map[y][x] = '#';

            if (is_guard_cycle(ghost_guard)) {
                map[y][x] = c;
                ghost_guard = guard;
                if (!guard_tick(&ghost_guard)) {
                    continue;
                }
                obstruction_map[y][x] = 1;
                cycles += 1;
            }

            map[y][x] = c;
        }
    }

    return cycles;
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

    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            if (obstruction_map[y][x]) {
                putchar('O');
            } else {
                putchar(map[y][x]);
            }
        }
        puts("");
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
