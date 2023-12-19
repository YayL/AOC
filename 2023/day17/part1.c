#include "common.h"
#include "heap_node.h"
#include "priorityqueue_min.h"

#define HEIGHT 141
#define WIDTH HEIGHT

typedef struct path {
    int x;
    int y;
    int heat;
    enum direction {
        UP, LEFT, DOWN, RIGHT, DIRECTIONS
    } dir;
} Path;

char map[HEIGHT][WIDTH];
int heat_map[HEIGHT][WIDTH][DIRECTIONS] = {0};
int width, height = 0;

struct HeapNode * new_path(int x, int y, int heat, enum direction dir) {
    Path * path = malloc(sizeof(Path));

    path->x = x;
    path->y = y;
    path->heat = heat;
    path->dir = dir;

    return init_heap_node(path, heat);
}

void traverse_path(Heap * heap, Path * prev, int x, int y, int heat, enum direction dir) {
    for (int i = 0; i < 3; ++i) {
        switch (dir) {
            case UP:
                y -= 1; break;
            case RIGHT:
                x += 1; break;
            case DOWN:
                y += 1; break;
            case LEFT:
                x -= 1; break;
        }

        if (x < 0 || x >= width || y < 0 || y >= height) {
            break;
        }
        
        heat += map[y][x];

        if (heat_map[y][x][dir] == 0 || heat_map[y][x][dir] > heat) {
            heat_map[y][x][dir] = heat;
            minheap_insert(heap, new_path(x, y, heat, dir));
        }
    }
}


int find_minimum_heat_path() {
    struct Heap * heap = init_Heap(1 << 8);
    traverse_path(heap, NULL, 0, 0, 0, RIGHT);
    traverse_path(heap, NULL, 0, 0, 0, DOWN);

    Path * path = NULL;
    struct HeapNode * node;

    while (heap->size) {
        node = minheap_pop(heap);
        path = node->ptr;
        if (path->x == width - 1 && path->y == height - 1) {
            return path->heat;
        }
        if (path->heat != heat_map[path->y][path->x][path->dir]) {
            continue;
        }

        traverse_path(heap, path, path->x, path->y, path->heat, (path->dir + DIRECTIONS - 1) % DIRECTIONS);
        traverse_path(heap, path, path->x, path->y, path->heat, (path->dir + DIRECTIONS + 1) % DIRECTIONS);
    }

    return -1;
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
    

    while ((read = getline(&line, &length, fp)) != -1) {
        width = read - 1;
        if (width > WIDTH) {
            println("Increase WIDTH to {i}", width);
            exit(1);
        }
        for (int x = 0; x < width; ++x) {
            if (!isdigit(line[x])) {
                println("Non digit char found: '{c}'", line[x]);
                exit(1);
            }
            map[height][x] = line[x] - '0';
        }
        height += 1;
    }

    result = find_minimum_heat_path();

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
