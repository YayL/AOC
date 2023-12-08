#include "common.h"

#include "hashmap.h"

typedef struct MapNode {
    char * name;
    struct MapNode * left, * right;
} MapNode;

void print_node(MapNode * node) {
    println("{s} = [{2s:, }]", node->name, node->left->name, node->right->name);
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

    MapNode * node, * temp;
    HashMap * hashmap = new_HashMap(16);
    char * name = NULL, * left = NULL, * right = NULL;

    read = getline(&line, &length, fp);
    String * instructions = new_string_len(line, read);
    getline(&line, &length, fp); // skip empty line

    while((read = getline(&line, &length, fp)) != -1) {
        name = calloc(4, sizeof(char)), left = calloc(4, sizeof(char)), right = calloc(4, sizeof(char));
        sscanf(line, "%3c = (%3c, %3c)", name, left, right);
        node = HM_get(hashmap, name);
        if (node == NULL) {
            node = malloc(sizeof(MapNode));
            node->name = name;
        }
        HM_set(hashmap, name, node);

        temp = HM_get(hashmap, left);
        if (temp == NULL) {
            temp = malloc(sizeof(MapNode));
            temp->name = left;
        }
        HM_set(hashmap, left, temp);
        node->left = temp;

        temp = HM_get(hashmap, right);
        if (temp == NULL) {
            temp = malloc(sizeof(MapNode));
            temp->name = right;
        }
        HM_set(hashmap, right, temp);
        node->right = temp;
    }
 
    // find path length for this route
    node = HM_get(hashmap, "AAA");
    const char * inst = instructions->c_str;
    while (1) {
        for (int i = 0; i < instructions->length - 1; ++i) {
            switch (inst[i]) {
                case 'L':
                    node = node->left; break;
                case 'R':
                    node = node->right; break;
                default:
                    println("Unknown instruction char: '{c}'", inst[i]);
                    exit(1);
            }
            result++;
            if (!strcmp(node->name, "ZZZ"))
                goto end;
        }
    }
    end:;

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lli}", result);
}
