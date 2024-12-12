#include "common.h"
#include "fmt.h"
#include <stdio.h>
#include <string.h>

#define HEIGHT 140
#define WIDTH 140

char map[HEIGHT][WIDTH] = {0};
char visited[HEIGHT][WIDTH] = {0};
char region[HEIGHT][WIDTH] = {0};

int mark_out_area(size_t x, size_t y, char ID) {
    // check is inbounds, the correct ID and not already marked
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || map[y][x] != ID || region[y][x] == 1) {
        return 0;
    }

    region[y][x] = 1;
    visited[y][x] = 1;

    // mark the surrounding area
    return 1 + mark_out_area(x + 1, y, ID) + mark_out_area(x - 1, y, ID) + mark_out_area(x, y + 1, ID) + mark_out_area(x, y - 1, ID);
}

unsigned long long calculate_fence_cost(size_t x_start, size_t y_start) {
    char ID = map[y_start][x_start];
    int sides = 0, area = 0;

    // scan area
    memset(region, 0, sizeof(region));
    area = mark_out_area(x_start, y_start, ID);

    char first, second;

    // scan sides horizontally
    for (size_t y = 0; y < HEIGHT; ++y) {
        first = second = 0;
        for (size_t x = 0; x < WIDTH; ++x) {
            if (region[y][x] == 1) {
                if (y == 0 || region[y - 1][x] == 0) {
                    first = 1;
                } else if (first) {
                    sides += 1;
                    first = 0;
                }
                if (y + 1 == HEIGHT || region[y + 1][x] == 0) {
                    second = 1;
                } else if (second) {
                    sides += 1;
                    second = 0;
                }
            } else {
                sides += first + second;
                first = second = 0;
            }
        }
        sides += first + second;
    }

    // scan sides vertically
    for (size_t x = 0; x < WIDTH; ++x) {
        first = second = 0;
        for (size_t y = 0; y < HEIGHT; ++y) {
            if (region[y][x] == 1) {
                if (x == 0 || region[y][x - 1] == 0) {
                    first = 1;
                } else if (first) {
                    sides += 1;
                    first = 0;
                }
                if (x + 1 == WIDTH || region[y][x + 1] == 0) {
                    second = 1;
                } else if (second) {
                    sides += 1;
                    second = 0;
                }
            } else {
                sides += first + second;
                first = second = 0;
            }
        }
        sides += first + second;
    }

    return sides * area;
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
    size_t height = 0, width = 0;
    
    while ((read = getline(&line, &length, fp)) != -1) {
        if (width == 0) {
            width = read - 1;
        }

        memcpy(map[height], line, width);
        height += 1;
    }

    if (width != WIDTH || height != HEIGHT) {
        println("Invalid map size: {i}x{i}, should be {i}x{i}", width, height, WIDTH, HEIGHT);
        exit(1);
    }
    
    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            if (visited[y][x]) {
                continue;
            }

            result += calculate_fence_cost(x, y);
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
