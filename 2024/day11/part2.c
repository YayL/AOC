#include "common.h"
#include "fmt.h"
#include "hashmap.h"
#include "hashmap_int.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

#define MAX_BLINKS 75

HashMap_int * cache[MAX_BLINKS] = {0};

unsigned long long count_stones(long long stone, size_t blink) {
    if (blink == MAX_BLINKS) {
        return 1;
    }

    unsigned long long count = HM_int_get(cache[blink], stone);

    if (count != -1) {
        return count;
    }

    if (stone == 0) {
        count = count_stones(1, blink + 1);
    } else {
        int digits = log10(stone) + 1;

        if (digits % 2 == 0) {
            size_t half_pow = pow(10, (int) (digits / 2));
            count = count_stones(stone / half_pow, blink + 1);
            count += count_stones(stone % half_pow, blink + 1);
        } else {
            count = count_stones(stone * 2024, blink + 1);
        }
    }

    HM_int_set(cache[blink], stone, count);

    return count;
}

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    char * line = NULL, * end;
    size_t length = 0, read;
    
    struct Vector * stones = init_vector();

    read = getline(&line, &length, fp);

    char * next = line;
    do {
        long num = strtol(next, &end, 10);
        vector_push(stones, num);
        next = end + 1;
    } while (end - line < read - 1);

    for (size_t i = 0; i < MAX_BLINKS; ++i) {
        cache[i] = new_HashMap_int(10);
    }

    for (size_t i = 0; i < stones->size; ++i) {
        result += count_stones(vector_at(stones, i), 0);
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
