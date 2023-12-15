#include "common.h"

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

void flip_north() {
    int temp_y;
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

            if (temp_y != y) {
                map[y][x] = '.';
                map[temp_y + 1][x] = 'O';
            }
        }
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

    flip_north();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (map[y][x] == 'O') {
                result += height - y;
            }
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
