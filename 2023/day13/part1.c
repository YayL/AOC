#include "common.h"

#define HEIGHT 50
#define WIDTH HEIGHT

char map[HEIGHT][WIDTH];

int height, width;

int is_palindromatic(const char * s1, const char * s2, size_t n) {
    if (n == 0) {
        return 0;
    }
 
    const char * p1 = s1;
    const char * p2 = s2 + n - 1;

    while (*(p1++) == *(p2--)) {
        if (p1 >= p2) {
            return 1;
        }
    }

    return 0;
}

int find_vertical_symmetry() {
    int offset = 0;

    for (int line = 1; line < width; ++line) {
        offset = MIN(line, width - line);
        for (int y = 0; y < height; ++y) {
            if (!is_palindromatic(map[y] + line - offset, map[y] + line, offset)) {
                goto next;
            }
        }
        return line;
next:;
    }

    return -1;
}

int find_horizontal_symmetry() {
    int max_offset;

    for (int line = 0; line < height - 1; ++line) {
        max_offset = MIN(line + 1, height - line - 1);
        for (int offset = 0; offset < max_offset; ++offset) {
            if (memcmp(map[line - offset], map[line + offset + 1], width) != 0) {
                goto next;
            }
        }
        return line + 1;
next:;
    }

    return -1;
}

int calc_symmetry_score() {
    int symmetry = find_vertical_symmetry();
    if (symmetry != -1) {
        return symmetry;
    }

    if ((symmetry = find_horizontal_symmetry()) != -1) {
        return 100 * symmetry;
    }

    println("None found?");
    exit(1);
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

    height = 0;

    while ((read = getline(&line, &length, fp)) != -1) {
        if (line[0] == '\n') {
            result += calc_symmetry_score();
            height = 0;
            continue;
        }
        width = read - 1;
        memcpy(map[height], line, width);
        height += 1;
    }

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lli}", result);
}
