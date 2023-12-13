#include "common.h"
#include "list.h"

struct Number {
    int value;
    int length;
    int x;
    int y;
};

struct Gear {
    int x;
    int y;
    struct List * adjecent;
};

int line_length;

struct List * map;
struct List * gears;

void is_gear_at(int x, int y, struct Number * number) {
    if (!(0 <= y && 0 <= x && y < map->size && x < line_length)) {
        return;
    } else if (((char *) list_at(map, y))[x] != '*') {
        return;
    }

    struct Gear * gear;
    for (int i = 0; i < gears->size; ++i) {
        gear = list_at(gears, i);
        if (gear->x == x && gear->y == y) {
            if (!list_contains(gear->adjecent, number)) {
                list_push(gear->adjecent, number);
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

    char * line = NULL;
    size_t length = 0, read;

    struct Number * number = NULL;
    struct Gear * gear = NULL;
    
    map = init_list(sizeof(char *));
    gears = init_list(sizeof(struct Gear *));
    struct List * numbers = init_list(sizeof(struct Number *));

    int y = 0;

    // initial parsing
    while ((read = getline(&line, &length, fp)) != -1) {
        line_length = read - 1;
        char * line_copy = malloc(sizeof(char) * read);
        strcpy(line_copy, line);
        list_push(map, line_copy);
        
        for (int i = 0; i < line_length; ++i) {
            if (!isdigit(line[i])) {
                if (line[i] == '*') {
                    gear = malloc(sizeof(struct Gear));
                    gear->x = i, gear->y = y;
                    gear->adjecent = init_list(sizeof(struct Number *));
                    list_push(gears, gear);
                }
                number = NULL;
                continue;
            }
            line_copy[i] = '.';

            if (number != NULL) {
                number->length++;
                number->value = number->value * 10 + line[i] - '0';
            } else {
                number = malloc(sizeof(struct Number));
                number->x = i, number->y = y;
                number->value = line[i] - '0';
                number->length = 1;
                list_push(numbers, number);
            }

        }

        number = NULL; // to dissallow wrapping numbers
        y += 1;
    }

    // check for gears and numbers
    int end_x, sum = 0;
    for (int i = 0; i < numbers->size; ++i) {
        number = list_at(numbers, i);
        y = number->y, end_x = number->x + number->length;

        for (int x = number->x; x < end_x; ++x) {
            is_gear_at(x - 1, y - 1, number);
            is_gear_at(x, y - 1, number);
            is_gear_at(x + 1, y - 1, number);
            is_gear_at(x - 1, y, number);
            is_gear_at(x, y, number);
            is_gear_at(x + 1, y, number);
            is_gear_at(x - 1, y + 1, number);
            is_gear_at(x, y + 1, number);
            is_gear_at(x + 1, y + 1, number);
        }
    } 

    // loop over gear info
    for (int i = 0; i < gears->size; ++i) {
        struct Gear * gear = list_at(gears, i);
        
        if (gear->adjecent->size == 2) {
            sum += ((struct Number *) list_at(gear->adjecent, 0))->value * ((struct Number *) list_at(gear->adjecent, 1))->value;
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Final: {i}", sum);

}
