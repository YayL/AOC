#include "common.h"

#define WIDTH 150
#define HEIGHT WIDTH
#define GROUND 0
#define NORTH (1 << 0)
#define EAST (1 << 1)
#define SOUTH (1 << 2)
#define WEST (1 << 3)
#define CHECKED (1 << 4)
#define DIR_MASK CHECKED - 1
#define LENGTH (1 << 5)
#define LENGTH_MASK ((unsigned int)-1) ^ (LENGTH - 1)
#define GET_LENGTH(slot) ((slot & LENGTH_MASK) / LENGTH)

#define MAX(a,b) ((a) > (b)) ? (a) : (b)
#define MIN(a,b) ((a) < (b)) ? (a) : (b)

unsigned int map[HEIGHT][WIDTH];
int width = 0;
int height = 0;
int start_x, start_y;

void print_map() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            putchar(GET_LENGTH(map[y][x]) + '0');
        }
        puts("");
    }
}

char char_to_map_slot(char c) {
    switch (c) {
        case '|':
            return NORTH | SOUTH;
        case '-':
            return WEST | EAST;
        case 'L':
            return NORTH | EAST;
        case 'J':
            return NORTH | WEST;
        case '7':
            return WEST | SOUTH;
        case 'F':
            return EAST | SOUTH;
        case '.':
            return GROUND;
        case 'S':
            return NORTH | EAST | SOUTH | WEST;
        default:
            println("Invalid map character: '{c}'", c);
            exit(1);
    }
}

void set_distances(int x, int y, int length) {
    unsigned int slot = map[y][x], slot_length = GET_LENGTH(slot);
    if (slot & CHECKED && slot_length < length) {
        return;
    }

    map[y][x] = (slot & DIR_MASK) | CHECKED | (length * LENGTH);

    if (0 < y && slot & NORTH && map[y - 1][x] & SOUTH) {
        set_distances(x, y - 1, length + 1);
    }

    if (x + 1 < width && slot & EAST && map[y][x + 1] & WEST) {
        set_distances(x + 1, y, length + 1);
    }

    if (0 < x && slot & WEST && map[y][x - 1] & EAST) {
        set_distances(x - 1, y, length + 1);
    }

    if (y + 1 < height && slot & SOUTH && map[y + 1][x] & NORTH) {
        set_distances(x, y + 1, length + 1);
    }
}

int main() {
    start_timer();
    FILE * fp = fopen("real_input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    size_t result = 0;

    char * line = NULL;
    size_t length = 0, read;

    while ((read = getline(&line, &length, fp)) != -1) {
        read -= 1;
        if (width < read) {
            width = read;
        }
        for (int x = 0; x < read; ++x) {
            if (line[x] == 'S') {
                start_x = x; start_y = height;
            }
            map[height][x] = char_to_map_slot(line[x]);
        }
        height += 1;
    }

    set_distances(start_x, start_y, 0);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            result = MAX(GET_LENGTH(map[y][x]), result);
        }
    }

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {llu}", result);
}
