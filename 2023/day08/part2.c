#include "common.h"

#include "hashmap.h"
#include "list.h"
#include "vector.h"

typedef struct MapNode {
    struct MapNode * left, * right;
    char is_end;
} MapNode;

unsigned long long gcd(unsigned long long a, unsigned long long int b) 
{ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
} 
  
unsigned long long lcm(unsigned long long a, unsigned long long b) { return (a / gcd(a, b)) * b; } 

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    unsigned long long result = 0;
    char * line = NULL;
    size_t length = 0, read;

    MapNode * node, * temp;
    HashMap * hashmap = new_HashMap(16);
    struct List * nodes = init_list(sizeof(MapNode *));
    char * name = NULL, * left = NULL, * right = NULL;

    // Parse input

    read = getline(&line, &length, fp);
    String * instructions = new_string_len(line, read);
    getline(&line, &length, fp); // skip empty line

    while((read = getline(&line, &length, fp)) != -1) {
        name = calloc(4, sizeof(char)), left = calloc(4, sizeof(char)), right = calloc(4, sizeof(char));
        sscanf(line, "%3c = (%3c, %3c)", name, left, right);
        node = HM_get(hashmap, name);
        if (node == NULL) {
            node = malloc(sizeof(MapNode));
            node->is_end = name[2] == 'Z';
        }
        HM_set(hashmap, name, node);
        if (name[2] == 'A') {
            list_push(nodes, node);
        }

        temp = HM_get(hashmap, left);
        if (temp == NULL) {
            temp = malloc(sizeof(MapNode));
            temp->is_end = left[2] == 'Z';
        }
        HM_set(hashmap, left, temp);
        node->left = temp;

        temp = HM_get(hashmap, right);
        if (temp == NULL) {
            temp = malloc(sizeof(MapNode));
            temp->is_end = right[2] == 'Z';
        }
        HM_set(hashmap, right, temp);
        node->right = temp;
    }
 
    char c;
    const char * inst = instructions->c_str;
    length = instructions->length - 1;

    size_t count = 0, finished = 0;

    size_t path_lengths[nodes->size];

    MapNode ** ref, * value;

    // Find the path lengths for each start node

    while (1) {
        for (int i = 0; i < length; ++i) {
            c = inst[i];
            count += 1;
            for (int j = 0; j < nodes->size; ++j) {
                ref = &nodes->items[j], value = *ref;
                if (value->is_end)
                    continue;

                switch (c) {
                    case 'L':
                        *ref = value->left; break;
                    case 'R':
                        *ref = value->right; break;
                    default:
                        println("Unknown instruction char: '{c}'", inst[i]);
                        exit(1);
                }

                if ((*ref)->is_end) {
                    finished += 1;
                    path_lengths[j] = count;
                }
            }
            if (finished == nodes->size)
                goto end;
        }
    }
    end:
    // Fined the lowest common multiple of all path lengths

    result = path_lengths[0];
    for (int i = 1; i < nodes->size; ++i) {
        result = lcm(result, path_lengths[i]);
    }

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lli}", result);
}
