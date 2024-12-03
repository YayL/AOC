#include "common.h"
#include "vector.h"

#define STONE (1 << 1)
#define GARDEN (1 << 2)
#define VISITED (1 << 3)

#define X_WIDTH 20

#define add_coord(vec, x, y) vector_push(vec, ((y) << X_WIDTH) | (x))
#define get_coord(x, y, value) x = (value) & ((1 << X_WIDTH) - 1), y = (value) >> X_WIDTH

#define WIDTH 131
#define HEIGHT WIDTH

#define STEPS 132

char map[HEIGHT][WIDTH];

int width, height = 0;
int start_x, start_y;

char char_to_map_slot(char c, int x, int y) {
    switch (c) {
        case '#':
            return STONE;
        case 'S':
            start_x = x, start_y = y;
            return GARDEN | VISITED;
        case '.':
            return GARDEN;
    }

    println("Invalid map characther: '{c}'", c);
    exit(1);
}

void print_map() {
    char value;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            value = map[y][x];
            if (value & VISITED) {
                putchar('O');
            } else if (value & GARDEN) {
                putchar('.');
            } else if (value & STONE) {
                putchar('#');
            }
        }
        puts("");
    }
}

int find_reachable(struct Vector * current) {
    int x, y, steps_taken = 0;
    struct Vector * visited = init_vector(), * temp;

    while (steps_taken < STEPS) {
        vector_clear(visited);
        for (int i = 0; i < current->size; ++i) {
            get_coord(x, y, current->items[i]);
            map[y][x] &= ~VISITED;

            if (x - 1 >= 0 && map[y][x - 1] == GARDEN) {
                add_coord(visited, x - 1, y);
                map[y][x - 1] |= VISITED;
            }
            if (x + 1 < width && map[y][x + 1] == GARDEN) {
                add_coord(visited, x + 1, y);
                map[y][x + 1] |= VISITED;
            }
            if (y - 1 >= 0 && map[y - 1][x] == GARDEN) {
                add_coord(visited, x, y - 1);
                map[y - 1][x] |= VISITED;
            }
            if (y + 1 < height && map[y + 1][x] == GARDEN) {
                add_coord(visited, x, y + 1);
                map[y + 1][x] |= VISITED;
            }
        }

        temp = visited;
        visited = current;
        current = temp;
        steps_taken += 1;
    }

    /* print_map(); */

    return current->size;
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
    

    while ((read = getline(&line, &length, fp)) != -1) {
        width = read - 1;
        if (WIDTH < width) {
            println("Increase WIDTH to: {i}", width);
            exit(1);
        }

        for (int x = 0; x < width; ++x) {
            map[height][x] = char_to_map_slot(line[x], x, height);
        }

        height += 1;
    }
 
    struct Vector * current = init_vector();
    
    add_coord(current, start_x, start_y);

    result = find_reachable(current);
    
    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
