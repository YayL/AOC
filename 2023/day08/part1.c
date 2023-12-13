#include "common.h"

#include "hashmap.h"
#include "list.h"
#include "timer.h"
#include "vector.h"

#define PARSE_NODE_NAME(dest, src, offset) memcpy(dest, src + offset, 3); dest[3] = 0
#define ENCODE(name) (((long) (name[0] - 'A') & 0b11111) << 12) | (((long) (name[1] - 'A') & 0b11111) << 6) | (((long) (name[2] - 'A') & 0b11111))
#define IS_END(name) (name[0] == 'Z' && name[1] == 'Z' && name[2] == 'Z')

typedef struct MapNode {
    struct MapNode * left, * right;
    char is_end;
} MapNode;

MapNode * storage[104026];
char * instructions;
size_t instruction_length;

void parse_nodes(FILE * fp) {
    char * line = NULL, * name, * left, * right;
    size_t read, length;
    
    MapNode * node, * temp;

    instruction_length = getline(&instructions, &length, fp) - 1;
    getline(&line, &length, fp); line = NULL;

    while((read = getline(&line, &length, fp)) != -1) {
        if (line[0] == '\n') {
            continue;
        }
        name = malloc(4 * sizeof(char)), left = malloc(4 * sizeof(char)), right = malloc(4 * sizeof(char));
        PARSE_NODE_NAME(name, line, 0);
        PARSE_NODE_NAME(left, line, 7);
        PARSE_NODE_NAME(right, line, 12);

        node = storage[ENCODE(name)];
        if (node == NULL) {
            node = malloc(sizeof(MapNode));
            node->is_end = IS_END(name);
            storage[ENCODE(name)] = node;
        }

        temp = storage[ENCODE(left)];
        if (temp == NULL) {
            temp = malloc(sizeof(MapNode));
            temp->is_end = IS_END(left);
            storage[ENCODE(left)] = temp;
        }
        node->left = temp;

        temp = storage[ENCODE(right)];
        if (temp == NULL) {
            temp = malloc(sizeof(MapNode));
            temp->is_end = IS_END(right);
            storage[ENCODE(right)] = temp;
        }
        node->right = temp;
    }
}

size_t find_length() {
    MapNode * node;
    size_t count = 0;

    node = storage[ENCODE("AAA")];

    while (1) {
        for (int j = 0; j < instruction_length; ++j) {
            switch (instructions[j]) {
                case 'L':
                    node = node->left; break;
                case 'R':
                    node = node->right; break;
                default:
                    println("Unknown instruction char: '{c}'", instructions[j]);
                    exit(1);
            }

            count += 1;
            if (node->is_end) {
                return count;
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

    parse_nodes(fp);

    unsigned long long result = find_length();

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
