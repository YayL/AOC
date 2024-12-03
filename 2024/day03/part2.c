#include "common.h"
#include "fmt.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define NO_DIFFERENCE 0
#define MAX_GROUPS 3

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

    regex_t regex;
    regmatch_t groups[MAX_GROUPS];
    int rcval = regcomp(&regex, "^mul\\(([0-9]{1,3}),([0-9]{1,3})\\)", REG_EXTENDED);

    if (rcval) {
        println("Unable to compile regex");
        exit(1);
    }
    
    int first, second;
    char mul_enabled = 1;

    while ((read = getline(&line, &length, fp)) != -1) {
        for (size_t i = 0; i < length; ++i) {
            if (!mul_enabled) {
                if (strncmp(line + i, "do()", 4) == NO_DIFFERENCE) {
                    mul_enabled = 1;
                    i += 3;
                }
                continue;
            } else if (strncmp(line + i, "don't()", 7) == NO_DIFFERENCE) {
                mul_enabled = 0;
                i += 6;
                continue;
            }

            if (line[i] != 'm') {
                continue;
            }

            if (regexec(&regex, line + i, MAX_GROUPS, groups, 0) != 0) {
                continue;
            }

            first = atoi(line + i + groups[1].rm_so);
            second = atoi(line + i + groups[2].rm_so);

            result += first * second;
        }

        length = 0;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
