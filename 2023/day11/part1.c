#include "common.h"
#include "vector.h"

#define WIDTH 150
#define HEIGHT WIDTH

#define X_COORD ((1) << 12)
#define GET_X(val) (((val) & (~(X_COORD - 1))) / X_COORD)
#define GET_Y(val) ((val) & (X_COORD - 1))
#define SET_COORD(x,y) ((x * X_COORD) | y)

#define IS_EMPTY 0
#define IS_GALAXY 1

#define MIN(a,b) ((a) < (b) ? (a) : (b))

long width = 0, height = 0;

typedef struct coord {
    long x;
    long y;
} Coord;

char map[HEIGHT][WIDTH];
char empty_rows[HEIGHT];
char empty_columns[WIDTH];

long get_distance_between(long first, long second) {
    long x1 = GET_X(first), y1 = GET_Y(first),
        x2 = GET_X(second), y2 = GET_Y(second), temp;
    if (x2 < x1) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y2 < y1) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    long result = x2 - x1 + y2 - y1;

    for (long x = x1 + 1; x < x2; ++x) {
        result += empty_columns[x] * (1e6 - 1);
    }
    for (long y = y1 + 1; y < y2; ++y) {
        result += empty_rows[y] * (1e6 - 1);
    }
    
    return result;
}

int main() {
    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    struct Vector * coords = init_vector();

    char * line = NULL, is_not_empty = 0;
    size_t length = 0, read;
    
    while ((read = getline(&line, &length, fp)) != -1) {
        width = read - 1;
        is_not_empty = 0;
        for (long x = 0; x < width; ++x) {
            if (line[x] == '#') {
                vector_push(coords, SET_COORD(x, height));
            }
            is_not_empty |= map[height][x] = (line[x] == '#');
        }
        empty_rows[height] = !is_not_empty;
        height += 1;
    }

    for (long x = 0; x < width; ++x) {
        is_not_empty = 0; 
        for (long y = 0; y < height; ++y) {
            is_not_empty |= map[y][x] == IS_GALAXY;
        }
        empty_columns[x] = !is_not_empty;
    }

    long dist;

    for (long i = 0; i < coords->size; ++i) {
        for (long j = i + 1; j < coords->size; ++j) {
            dist = get_distance_between(coords->items[i], coords->items[j]);
            result += dist;
        }
    }

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lli}", result);
}
