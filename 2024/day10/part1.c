#include "common.h"
#include "fmt.h"
#include "list.h"
#include "pair.h"
#include <string.h>

#define HEIGHT 47
#define WIDTH 47

char map[HEIGHT][WIDTH];

#define CHECK_SQUARE(list, x, y, value) \
    if (map[y][x] == (value) && 0 <= x && x < WIDTH && 0 <= y && y < HEIGHT) { \
        list_push(list, new_pair(x, y)); \
    }

size_t search_trailhead(size_t x_start, size_t y_start) {
    char visited[HEIGHT][WIDTH] = {0};
    Pair * coord;
    struct List * stack = init_list(sizeof(Pair *));
    list_push(stack, new_pair(x_start, y_start));
    size_t count = 0;

    while (stack->size > 0) {
       coord = list_pop(stack);
       int x = coord->a, y = coord->b;

       if (visited[y][x]) {
           continue;
       }

       visited[y][x] = 1;
       char value = map[y][x];

       if (value == '9') {
           count += 1;
           continue;
       }
    
       CHECK_SQUARE(stack, x, y - 1, value + 1);
       CHECK_SQUARE(stack, x + 1, y, value + 1);
       CHECK_SQUARE(stack, x, y + 1, value + 1);
       CHECK_SQUARE(stack, x - 1, y, value + 1);
    }

    return count;
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

    size_t width = 0, height = 0;
    
    while ((read = getline(&line, &length, fp)) != -1) {
        if (width == 0) {
            width = read - 1;
        }
        memcpy(map[height], line, WIDTH);

        height += 1;
    }

    if (width != WIDTH || height != HEIGHT) {
        println("Missmatched canvas size: {i}x{i} should be {i}x{i}", width, height, WIDTH, HEIGHT);
        exit(1);
    }

    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            if (map[y][x] == '0') {
                result += search_trailhead(x, y);
            }
        }
    }

    

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
