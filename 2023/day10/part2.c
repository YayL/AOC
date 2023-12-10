#include "common.h"

#define WIDTH 150
#define HEIGHT WIDTH

#define GROUND (0)
#define NORTH (1 << 0)
#define EAST (1 << 1)
#define SOUTH (1 << 2)
#define WEST (1 << 3)

#define CHECKED (1 << 4)
#define DIR_MASK (CHECKED - 1)

#define IS_LOOP (1 << 5)
#define IS_ENCLOSED (1 << 6)

#define LENGTH (1 << 7)
#define LENGTH_MASK (((int) -1) ^ (LENGTH - 1))
#define GET_LENGTH(slot) ((slot & LENGTH_MASK) / LENGTH)

#define MAX(a,b) ((a) > (b)) ? (a) : (b)
#define MIN(a,b) ((a) < (b)) ? (a) : (b)

unsigned int map[HEIGHT][WIDTH];
int width = 0;
int height = 0;
int start_x, start_y;

void print_map() {
    int slot; 
    char * s;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            slot = map[y][x];
            switch (slot & DIR_MASK) {
                case GROUND:
                    s = " "; break;
                case NORTH | SOUTH:
                    s = "║"; break;
                case NORTH | EAST:
                    s = "╚"; break;
                case NORTH | WEST:
                    s = "╝"; break;
                case SOUTH | WEST:
                    s = "╗"; break;
                case SOUTH | EAST:
                    s = "╔"; break;
                case WEST | EAST:
                    s = "═"; break;
                case NORTH | EAST | WEST | SOUTH:
                    s = "╬"; break;
                default:
                    println("\nInvalid slot value: {i}", slot & DIR_MASK);
                    exit(1);
            }
            if (map[y][x] & IS_LOOP) {
                print("\033[1;31;1m{s}\033[0m", s);
            } else if (map[y][x] & IS_ENCLOSED) {
                print("\033[1;44;1m{s}\033[0m", s);
            } else {
                print(s);
            }
        }
        puts("");
    }
}

int char_to_map_slot(char c) {
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

int find_loop(int x, int y, int length) {
    int slot = map[y][x], slot_length = GET_LENGTH(slot), is_loop = 0;
    if (slot & CHECKED) {
        if (length > 2 && x == start_x && y == start_y) {
            return 1;
        } else if (slot_length < length) {
            return 0;
        }
    }

    map[y][x] = (slot & (LENGTH - 1)) | CHECKED | (length * LENGTH);

    if (0 < y && slot & NORTH && map[y - 1][x] & SOUTH) {
        is_loop |= find_loop(x, y - 1, length + 1);
    }

    if (!is_loop && x + 1 < width && slot & EAST && map[y][x + 1] & WEST) {
        is_loop |= find_loop(x + 1, y, length + 1);
    }

    if (!is_loop && y + 1 < height && slot & SOUTH && map[y + 1][x] & NORTH) {
        is_loop |= find_loop(x, y + 1, length + 1);
    }

    if (!is_loop && 0 < x && slot & WEST && map[y][x - 1] & EAST) {
        is_loop |= find_loop(x - 1, y, length + 1);
    }

    if (is_loop) {
        map[y][x] |= IS_LOOP;
        return 1;
    }

    return 0;
}

int is_up(int x, int y, int slot) {
    if ((slot & SOUTH) && GET_LENGTH(map[y + 1][x]) < GET_LENGTH(slot)) {
        return 1;
    } else if ((slot & NORTH) && GET_LENGTH(slot) < GET_LENGTH(map[y - 1][x])) {
        return 1;
    }

    return 0;
}

int is_down(int x, int y, int slot) {
    if ((slot & NORTH) && GET_LENGTH(map[y - 1][x]) < GET_LENGTH(slot)) {
        return 1;
    } else if ((slot & SOUTH) && GET_LENGTH(slot) < GET_LENGTH(map[y + 1][x])) {
        return 1;
    }

    return 0;
}

int find_enclosed() {
    int enclosed = 0, set_enclosed, slot;
    for (int y = 0; y < height; ++y) {
        set_enclosed = 0;
        for (int x = 0; x < width; ++x) {
            slot = map[y][x];
            if (slot & IS_LOOP) {
                if (!set_enclosed && is_up(x, y, slot)) {
                    set_enclosed = 1;
                } else if (set_enclosed && is_down(x, y, slot)) {
                    set_enclosed = 0;
                }
            } else if (set_enclosed) {
                map[y][x] |= IS_ENCLOSED;
                enclosed += 1;
            }
        }
    }
    
    return enclosed;
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

    find_loop(start_x, start_y, 0);
    result = find_enclosed();

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {llu}", result);
}
