#include "common.h"
#include "list.h"

struct Number {
    int value;
    int length;
    int x;
    int y;
};

int line_length;

char is_gear_at(struct List * map, int x, int y) {
    if (0 <= y && 0 <= x && y < map->size && x < line_length) {
        return ((char *) list_at(map, y))[x] != '.';
    }

    return 0;
}

int main() {

    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read;

    struct Number * number = NULL;
    
    struct List * map = init_list(sizeof(char *));
    struct List * numbers = init_list(sizeof(struct Number *));

    int y = 0;

    while ((read = getline(&line, &length, fp)) != -1) {
        line_length = read - 1;
        char * line_copy = malloc(sizeof(char) * read);
        strcpy(line_copy, line);
        list_push(map, line_copy);
        
        for (int i = 0; i < line_length; ++i) {
            if (!isdigit(line[i])) {
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


    int end_x, final = 0;
    for (int i = 0; i < numbers->size; ++i) {
        number = list_at(numbers, i);
        y = number->y, end_x = number->x + number->length;

        for (int x = number->x; x < end_x; ++x) {
            if (is_gear_at(map, x - 1, y - 1) || is_gear_at(map, x, y - 1) || is_gear_at(map, x + 1, y - 1) ||
                is_gear_at(map, x - 1, y)     || is_gear_at(map, x, y)     || is_gear_at(map, x + 1, y)     ||
                is_gear_at(map, x - 1, y + 1) || is_gear_at(map, x, y + 1) || is_gear_at(map, x + 1, y + 1)) 
            {
                final += number->value;
                break;
            }
        }

    }

    println("Final: {i}", final);

}
