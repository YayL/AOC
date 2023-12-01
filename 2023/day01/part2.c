#include "common.h"
#include <stdio.h>

#define SET_NUM(num) if(first==0)\
                        first = num;\
                     last = num

int main() {

    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
    }

    char * line = NULL;
    size_t length = 0, read;
    
    int first = 0, last = 0, sum = 0;

    while ((read = getline(&line, &length, fp)) != -1) {
        for (int i = 0; i < read - 1; ++i) {
            if (isdigit(line[i])) {
                if (first == 0)
                    first = line[i] - '0';
                last = line[i] - '0';
            } else if (!strncmp(line + i, "one", 3)) {
                SET_NUM(1);
            } else if (!strncmp(line + i, "two", 3)) {
                SET_NUM(2);
            } else if (!strncmp(line + i, "three", 5)) {
                SET_NUM(3);
            } else if (!strncmp(line + i, "four", 4)) {
                SET_NUM(4);
            } else if (!strncmp(line + i, "five", 4)) {
                SET_NUM(5);
            } else if (!strncmp(line + i, "six", 3)) {
                SET_NUM(6);
            } else if (!strncmp(line + i, "seven", 5)) {
                SET_NUM(7);
            } else if (!strncmp(line + i, "eight", 5)) {
                SET_NUM(8);
            } else if (!strncmp(line + i, "nine", 4)) {
                SET_NUM(9);
            }
        }
        sum += first * 10 + last;
        first = 0, last = 0;
    }

    println("Number: {u}", sum);
}
