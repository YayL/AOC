#include "common.h"
#include "fmt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct file_block {
    size_t length;
    size_t ID;
};

typedef struct files {
    struct file_block * files;
    size_t size;
    size_t capacity;
} Files;

Files init_files() {
    return (Files) {0};
}

void push_file(Files * files, struct file_block file) {
	if (!files->size) {
		files->files = malloc(sizeof(struct file_block));
		files->capacity = 1;
	}

	if(files->capacity <= files->size) {
        files->capacity = files->size * 2;
		files->files = realloc(files->files, files->capacity * sizeof(struct file_block));
	}

	files->files[files->size++] = file;
}

void add_file(Files * files, struct file_block file, size_t index) {
    if (!files->size) {
        files->files = malloc(sizeof(struct file_block));
        files->capacity = 1;
    }

    files->size += 1;
	if(files->capacity < files->size) {
        files->capacity = files->size * 2;
		files->files = realloc(files->files, files->capacity * sizeof(struct file_block));
	}

    for (size_t i = files->size - 1; index < i; --i) {
        files->files[i] = files->files[i - 1];
    }

    files->files[index] = file;
}

struct file_block get_file(Files files, size_t index) {
    if (0 <= index && index < files.size) {
        return files.files[index];
    }

    println("Invalid index: {i}. Files {i}", index, files.size);
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
    read = getline(&line, &length, fp);
    length = read - 1;

    Files files = init_files();
    struct file_block file, free_file;
    size_t ID = 0;

    for (size_t i = 0; i < length; ++i) {
        file.length = line[i] - '0';
        if (i % 2 == 0) {
            file.ID = ID++;
        } else { // free space
            file.ID = -1;
        }
        push_file(&files, file);
    }

    for (ssize_t i = length - 1; 0 <= i; --i) {
        file = get_file(files, i);
        if (file.ID == -1) {
            continue;
        }

        for (size_t j = 0; j < i; ++j) {
            free_file = get_file(files, j);
            if (free_file.ID != -1 || free_file.length < file.length) {
                continue;
            }

            files.files[i].ID = -1;
            if (free_file.length == file.length) {
                files.files[j].ID = file.ID;
            } else {
                files.files[j].length -= file.length;
                add_file(&files, file, j);
            }

            break;
        }
    }

    for (size_t i = 0, pos = 0; i < files.size; ++i) {
        file = files.files[i];
        if (file.ID != -1) {
            result += file.ID * (file.length * pos + (file.length * (file.length - 1)) / 2);
        }
        pos += file.length;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
