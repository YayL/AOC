#include "common.h"
#include "list.h"
#include "vector.h"
#include "macros.h"

#define IS_OVERLAP(first, second) (first->end_x >= second->start_x && second->end_x >= first->start_x && first->end_y >= second->start_y && second->end_y >= first->start_y)

#define WIDTH 30
#define HEIGHT WIDTH

int height_map[HEIGHT][WIDTH] = {0};

typedef struct block {
    int start_x, end_x;
    int start_y, end_y;
    int start_z, end_z;
    struct Vector * below;
    struct Vector * above;
    char c;
} Block;

void print_block(const Block * block) {
    println("{c}: {3i:,}~{3i:,}", block->c, block->start_x, block->start_y, block->start_z, block->end_x, block->end_y, block->end_z);
}

Block * parse_block(const char * line, char c) {
    Block * block = malloc(sizeof(Block));
    sscanf(line, "%d,%d,%d~%d,%d,%d", &block->start_x, &block->start_y, &block->start_z, &block->end_x, &block->end_y, &block->end_z);
    block->c = c + 'A';
    int temp;
    MIN_SWAP(block->start_x, block->end_x, temp);
    MIN_SWAP(block->start_y, block->end_y, temp);
    MIN_SWAP(block->start_z, block->end_z, temp);
    block->below = init_vector();
    block->above = init_vector();
    return block;
}

int sort_by_z(const void * left, const void * right) {
    const Block * b1 = left, * b2 = right;
    return b1->start_z < b2->start_z;
}

int main() {

    start_timer();
    FILE * fp = fopen("real_input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    char * line = NULL;
    size_t length = 0, read, number = 0;

    struct List * blocks = init_list(sizeof(Block *));

    while ((read = getline(&line, &length, fp)) != -1) {
        list_push(blocks, parse_block(line, number++));
        line = NULL;
    }

    list_sort(blocks, sort_by_z);
    Block * block, * support;
    int max;

    for (int i = 0; i < blocks->size; ++i) {
        block = blocks->items[i];
        /* print_block(block); */
        
        max = 0;
        for (int y = block->start_y; y <= block->end_y; ++y) {
            for (int x = block->start_x; x <= block->end_x; ++x) {
                max = MAX(height_map[y][x], max);
            }
        }

        /* println("Max = {i}", max); */
        max += 1;
        if (block->start_z < max) {
            println("A problem with heights. Block: {i}, Max: {i}", block->start_z, max);
            exit(1);
        } else if (max < block->start_z) {
            /* print_block(block); */
            block->end_z = (block->end_z - block->start_z) + max;
            block->start_z = max;
            /* print_block(block); */
        }


        for (int y = block->start_y; y <= block->end_y; ++y) {
            for (int x = block->start_x; x <= block->end_x; ++x) {
                height_map[y][x] = max;
            }
        }
    }

    list_sort(blocks, sort_by_z);

    for (int i = 0; i < blocks->size; ++i) {
        support = blocks->items[i];
        for (int j = i + 1; j < blocks->size; ++j) {
            block = blocks->items[j];
            if (block->start_z == support->end_z) {
                continue;
            } else if (block->start_z > support->end_z && block->start_z != support->end_z + 1) {
                break;
            }

            if (IS_OVERLAP(block, support)) {
                /* println("Overlap: {c}: {c}", j + 'A', i + 'A'); */
                vector_push(block->below, i);
                vector_push(support->above, j);
            }
        }
    }

    println("Blocks Above:");
    for (int i = 0; i < blocks->size; ++i) {
        block = blocks->items[i];
        print("{c}:", i + 'A');
        for (int j = 0; j < block->above->size; ++j) {
            print(" {c} |", block->above->items[j] + 'A');
        }
        puts("");
    }
    
    puts("");
    
    println("Blocks Below:");
    for (int i = 0; i < blocks->size; ++i) {
        block = blocks->items[i];
        print("{c}:", i + 'A');
        for (int j = 0; j < block->below->size; ++j) {
            print(" {c} |", block->below->items[j] + 'A');
        }
        puts("");
    }
    
    puts("");

    result = 0;
    for (int i = 0; i < blocks->size; ++i) {
        block = blocks->items[i];

        for (int j = 0; j < block->above->size; ++j) {
            support = blocks->items[block->above->items[j]];
            if (support->below->size == 1) {
                goto next;
            }
        }

        result += 1;
next:;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
