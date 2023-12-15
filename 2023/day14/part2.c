#include "common.h"
#include "vector.h"
#include "hashmap_int.h"

#define ENCODE(a,b,c,d,e) ((a) << 32 | (b) << 24 | (c) << 16 | (d) << 8 | (e))

#define WIDTH 101
#define HEIGHT WIDTH

char map[HEIGHT][WIDTH];

size_t width, height = 0;

void print_map() {
    for (int y = 0; y < height; ++y) {
        println(map[y]);
    }
    puts("");
}

int flip_north() {
    int temp_y, moved = 0;
    char * row;

    for (int y = 0; y < height; ++y) {
        row = map[y];
        for (int x = 0; x < width; ++x) {
            if (row[x] != 'O') {
                continue;
            }

            temp_y = y - 1;
            while (temp_y >= 0 && map[temp_y][x] == '.') {
                temp_y -= 1;
            }

            if (temp_y + 1 != y) {
                moved += 1;
                map[y][x] = '.';
                map[temp_y + 1][x] = 'O';
            }
        }
    }

    return moved;
}


int flip_south() {
    int temp_y, moved = 0;
    char * row;

    for (int y = height - 1; y >= 0; --y) {
        row = map[y];
        for (int x = 0; x < width; ++x) {
            if (row[x] != 'O') {
                continue;
            }

            temp_y = y + 1;
            while (temp_y < height && map[temp_y][x] == '.') {
                temp_y += 1;
            }

            if (temp_y - 1!= y) {
                moved += 1;
                map[y][x] = '.';
                map[temp_y - 1][x] = 'O';
            }
        }
    }

    return moved;
}


int flip_east() {
    int temp_x, moved = 0;
    char * row;

    for (int y = 0; y < height; ++y) {
        row = map[y];
        for (int x = width; x >= 0; --x) {
            if (row[x] != 'O') {
                continue;
            }

            temp_x = x + 1;
            while (temp_x < width && map[y][temp_x] == '.') {
                temp_x += 1;
            }

            if (temp_x - 1 != x) {
                moved += 1;
                map[y][x] = '.';
                map[y][temp_x - 1] = 'O';
            }
        }
    }

    return moved;
}

int flip_west() {
    int temp_x, moved = 0;
    char * row;

    for (int y = 0; y < height; ++y) {
        row = map[y];
        for (int x = 0; x < width; ++x) {
            if (row[x] != 'O') {
                continue;
            }

            temp_x = x - 1;
            while (0 <= temp_x && map[y][temp_x] == '.') {
                temp_x -= 1;
            }

            if (temp_x + 1 != x) {
                moved += 1;
                map[y][x] = '.';
                map[y][temp_x + 1] = 'O';
            }
        }
    }

    return moved;
}

int calc_weight() {
    int weight = 0, inc = height;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (map[y][x] == 'O') {
                weight += inc;
            }
        }
        inc -= 1;
    }

    return weight;
}

int flip() {
    size_t north, west, south, east, weight,
           key, value, iteration = 0;
    HashMap_int * map = new_HashMap_int(16);
    struct Vector * vec = init_vector();

    // find a cycle here then return that something 1000000000
    while (1) {
        north = flip_north();
        west = flip_west();
        south = flip_south();
        east = flip_east();
 
        weight = calc_weight();

        key = ENCODE(north, west, south, east, weight);
        value = HM_int_get(map, key);

        if (value != -1) {
            int index = (((long long)1e9 - value) % (iteration - value));
            return vec->items[index + value - 1];
        } else {
            vector_push(vec, weight);
            HM_int_set(map, key, iteration);
        }
        iteration += 1;
    }
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
        memcpy(map[height++], line, width);
    }

    result = flip();

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
