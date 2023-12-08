#include "common.h"

#include "hashmap.h"

#define PARSE_NODE_NAME(dest, src, offset) memcpy(dest, src + offset, 3); dest[3] = 0
#define ENCODE(name) (((long) (name[0] - 'A') & 0b11111) << 12) | (((long) (name[1] - 'A') & 0b11111) << 6) | (((long) (name[2] - 'A') & 0b11111))
#define IS_END(name) (name[0] == 'Z' && name[1] == 'Z' && name[2] == 'Z')

typedef struct MapNode {
    struct MapNode * left, * right;
    char is_end;
} MapNode;

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    unsigned long long result = 0;
    char * line = NULL, * instructions;
    size_t length = 0, read, instruction_length;

    MapNode * node, * temp;
    MapNode * storage[104026] = {0}; // ENC("ZZZ") + 1
    char * name = NULL, * left = NULL, * right = NULL;

    // Setup Input

    instruction_length = getline(&instructions, &length, fp) - 1;
    getline(&line, &length, fp); // skip empty line

    while((read = getline(&line, &length, fp)) != -1) {
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

    // find path length for AAA to ZZZ
    
    node = storage[ENCODE("AAA")];
    while (1) {
        for (int i = 0; i < instruction_length; ++i) {
            switch (instructions[i]) {
                case 'L':
                    node = node->left; break;
                case 'R':
                    node = node->right; break;
                default:
                    println("Unknown instruction char: '{c}'", instructions[i]);
                    exit(1);
            }
            result++;
            if (node->is_end)
                goto end;
        }
    }
    end:;

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lli}", result);
}
