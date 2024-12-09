#include "common.h"
#include "fmt.h"
#include <stdio.h>
#include <stdlib.h>

struct file_block {
    size_t length;
    size_t ID;
};

void print_files(struct file_block * files, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        struct file_block file = files[i];
        /* println("File({li}): {u}", file.ID, file.length); */
        if (file.ID == -1) {
            for (size_t j = 0; j < file.length; ++j) {
                putchar('.');
            }
        } else {
            for (size_t j = 0; j < file.length; ++j) {
                printf("%d", file.ID);
            }
        }
    }
    puts("");
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
    read = getline(&line, &length, fp);
    length = read - 1;

    struct file_block * files = malloc(length * sizeof(struct file_block));
    struct file_block file;
    size_t ID = 0;

    for (size_t i = 0; i < length; ++i) {
        file.length = line[i] - '0';
        if (i % 2 == 0) {
            file.ID = ID++;
        } else { // free space
            file.ID = -1;
        }
        files[i] = file;
    }

    /* print_files(files, length); */

    struct file_block * compact_files = malloc(length * sizeof(struct file_block));
    size_t compact_size = 0;

    for (size_t i = 0, end = length - 1; i <= end; ++i) {
        file = files[i];
        if (file.ID != -1) {
            compact_files[compact_size++] = file;
            continue;
        }

        ssize_t size_to_fill = file.length;
        do {
            struct file_block * next = files + end;
            while (next->ID == -1 || next->length == 0) {
                next = &files[--end];
            }

            if (end < i) {
                break;
            }

            if (next->length <= size_to_fill) {
                compact_files[compact_size++] = *next;
                size_to_fill -= next->length;
                next->length = 0;
            } else {
                compact_files[compact_size++] = (struct file_block) {.length = size_to_fill, .ID = next->ID};
                next->length -= size_to_fill;
                size_to_fill = 0;
            }
        } while (size_to_fill > 0);
    }

    /* print_files(compact_files, compact_size); */

    for (size_t i = 0, pos = 0; i < compact_size; ++i) {
        file = compact_files[i];
        for (size_t j = 0; j < file.length; ++j) {
            result += pos * file.ID;
            pos += 1;
        }
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
