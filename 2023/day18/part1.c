#include "common.h"
#include "deque.h"

#define check(p, c, n, value) if (prev == p && now == c && next == n) return value

typedef struct dig {
    int value;
    enum direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    } dir;
} Dig;

enum direction char_to_direction(char c) {
    switch (c) {
        case 'U':
            return UP;
        case 'R':
            return RIGHT;
        case 'D':
            return DOWN;
        case 'L':
            return LEFT;
    }

    println("Invalid characther: '{c}'", c);
    exit(1);
}

int add_or_subtract(enum direction prev, enum direction now, enum direction next) {
    if (prev == next) {
        return 0;
    }

    check(UP, RIGHT, DOWN, 1);
    check(UP, LEFT, DOWN, -1);

    check(RIGHT, UP, LEFT, -1);
    check(RIGHT, DOWN, LEFT, 1);

    check(LEFT, UP, RIGHT, 1);
    check(LEFT, DOWN, RIGHT, -1);

    check(DOWN, RIGHT, UP, -1);
    check(DOWN, LEFT, UP, 1);

    println("uhoh somehow skipped all checks");
    exit(1);
}

int main() {

    start_timer();
    FILE * fp = fopen("real_input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    char * line = NULL;
    size_t length = 0, read;

    Deque * dug = init_deque(sizeof(Dig *));
    Dig * current, * prev, * next;

    while ((read = getline(&line, &length, fp)) != -1) {
        current = malloc(sizeof(Dig));
        current->value = atoi(line + 2);
        current->dir = char_to_direction(line[0]);
        push_back(dug, current);
    }

    long area = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    next = deque_index(dug, 0);
    
    for (int i = 0; i < dug->size; ++i) {
        prev = current;
        current = next;
        next = deque_index(dug, i + 1);
        
        switch (current->dir) {
            case UP:
                y2 += current->value + add_or_subtract(prev->dir, current->dir, next->dir); break;
            case RIGHT:
                x2 += current->value + add_or_subtract(prev->dir, current->dir, next->dir); break;
            case DOWN:
                y2 -= current->value + add_or_subtract(prev->dir, current->dir, next->dir); break;
            case LEFT:
                x2 -= current->value + add_or_subtract(prev->dir, current->dir, next->dir); break;
        }

        area += x2*y1 - y2*x1;
        x1 = x2, y1 = y2;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", area >> 1);
}
