#include "common.h"
#include "vector.h"
#include "deque.h"

#define STONE (0)
#define GARDEN (1 << 0)

#define X_WIDTH 20

#define add_coord(deque, x, y) push_back(deque, (void *) ((long)((y) << X_WIDTH) | (x)))
#define get_coord(x, y, value) x = (long)(value) & ((1 << X_WIDTH) - 1), y = (long)(value) >> X_WIDTH

#define WIDTH 11
#define HEIGHT WIDTH

#define OFFSET (WIDTH >> 1)

#define STEPS 5000

int map[HEIGHT][WIDTH] = {0};

int width, height = 0;
int start_x, start_y;

char char_to_map_slot(char c, int x, int y) {
    switch (c) {
        case '#':
            return STONE;
        case 'S':
            start_x = x, start_y = y;
            return GARDEN;
        case '.':
            return GARDEN;
    }

    println("Invalid map characther: '{c}'", c);
    exit(1);
}

long find_reachable(Deque * current) {
    int x, y, steps_taken = 0, prev = 0, prev_prev = 0, value;

    while (current->size != prev_prev) {
        prev_prev = prev;
        prev = current->size;

        while (current->size) {
            get_coord(x, y, pop_front(current));
            steps_taken = (map[y][x] >> 1) + 1;
            println("steps = {i}", steps_taken);

            if (x - 1 >= 0 && map[y][x - 1] & GARDEN) {
                value = map[y][x - 1] >> 1;
                if (value == 0 || steps_taken < value) {
                    add_coord(current, x - 1, y);
                    map[y][x - 1] = (steps_taken << 1) | GARDEN;
                }
            }
            if (x + 1 < width && map[y][x + 1] & GARDEN) {
                value = map[y][x + 1] >> 1;
                if (value == 0 || steps_taken < value) {
                    add_coord(current, x + 1, y);
                    map[y][x + 1] = (steps_taken << 1) | GARDEN;
                }
            }
            if (y - 1 >= 0 && map[y - 1][x] & GARDEN) {
                value = map[y - 1][x] >> 1;
                if (value == 0 || steps_taken < value) {
                    add_coord(current, x, y - 1);
                    map[y - 1][x] = (steps_taken << 1) | GARDEN;
                }
            }
            if (y + 1 < height && map[y + 1][x] & GARDEN) {
                value = map[y + 1][x] >> 1;
                if (value == 0 || steps_taken < value) {
                    add_coord(current, x, y + 1);
                    map[y + 1][x] = (steps_taken << 1) | GARDEN;
                }
            }
        }
    }

    int even_corners = 0, odd_corners = 0, even_full = 0, odd_full = 0, step;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x ) {
            step = map[y][x];
            if (!(step & GARDEN) || !(step = step >> 1)) {
                continue;
            }
            println("{i}", step);
            if ((step & 1) == 0) {
                even_corners += step > OFFSET;
                even_full += !(step > OFFSET);
            } else {
                odd_corners += step > OFFSET;
                odd_full += !(step > OFFSET);
            }
        }
    }

    int n = (STEPS / width) - 1/2;

    println("offset = {i}", OFFSET);

    println("N = {i}", n);
    println("even = ({i}, {i})", even_corners, even_full);
    println("odd = ({i}, {i})", odd_corners, odd_full);

    return ((n + 1) * (n + 1) * odd_full + (n * n) * even_full - (n + 1) * odd_corners + n * even_corners);
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
 
    Deque * current = init_deque(sizeof(long));
    
    add_coord(current, start_x, start_y);

    result = find_reachable(current);
    
    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
