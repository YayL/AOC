#include "common.h"
#include "deque.h"

#define BOXES 256

Deque * boxes[BOXES] = {NULL};

struct Lens {
    const char * name;
    size_t name_length;
    size_t focal_length;
};

void print_boxes() {
    Deque * box;
    struct Lens * lens;

    for (int i = 0; i < BOXES; ++i) {
        box = boxes[i];
        if (box == NULL || box->size == 0) {
            continue;
        }

        print("Box {i}:", i);
        for (int j = 0; j < box->size; ++j) {
            lens = deque_index(box, j);
            print(" [{s} {i}]", lens->name, lens->focal_length);
        }
        puts("");
    }
}

struct Lens * new_lens(const char * name, size_t name_length, size_t focal_length) {
    struct Lens * lens = malloc(sizeof(struct Lens));
    lens->name = name;
    lens->name_length = name_length;
    lens->focal_length = focal_length;
    return lens;
}

void execute_instruction(char op, const char * name, size_t name_length, int hash, int focal_length) {
    Deque * box = boxes[hash];
    if (box == NULL) {
        boxes[hash] = box = init_deque(sizeof(struct Lens *));
    }

    struct Lens * lens;
    int index = -1;
    
    // look for the correct lens
    for (int i = 0; i < box->size; ++i) {
        lens = deque_index(box, i);
        if (lens->name_length == name_length && !strcmp(lens->name, name)) {
            index = i;
            break;
        }
    }

    switch (op) {
        case '=':
            if (index == -1) {
                push_back(box, new_lens(name, name_length, focal_length));
            } else {
                lens->focal_length = focal_length;
            } break;
        case '-':
            if (index != -1) {
                deque_remove(box, index);
            } break;
        default:
            println("Invalid operator: '{c}'", op);
            exit(1);
    }
}

int main() {
    start_timer();
    FILE * fp = fopen("real_input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    int result = 0, current = 0, focal_length;

    char * line = NULL, * name, op;
    size_t length = 0, read, start = 0, name_length;

    read = getline(&line, &length, fp) - 1;

    for (int i = 0; i < read; ++i) {
        if (line[i] == '=' || line[i] == '-') {
            op = line[i];
            line[i] = '\0';
            name = line + start;
            name_length = i - start;
            focal_length = line[++i] - '0';

            execute_instruction(op, name, name_length, current, focal_length);
 
            current = 0;

            if (line[i] != ',') {
                i += 1;
            }
            start = i + 1;
            continue;
        }

        current += line[i];
        current *= 17;
        current &= 255;
    }

    struct Lens * lens;
    Deque * box;

    for (int box_index = 0; box_index < BOXES; ++box_index) {
        box = boxes[box_index];
        if (box == NULL)
            continue;

        for (int i = 0; i < box->size; ++i) {
            lens = deque_index(box, i);
            result += (box_index + 1) * (i + 1) * lens->focal_length;
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
