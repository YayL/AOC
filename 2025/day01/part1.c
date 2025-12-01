#include "common.h"
#include "fmt.h"

#define MAX_LINE_LENGTH 20
#define DIAL_TARGET 0
#define DIAL_END 100

int main() {
    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    char buf[MAX_LINE_LENGTH];

    int dial = 50, count = 0;

    while (fgets(buf, MAX_LINE_LENGTH, fp)) {
        int num = atoi(&buf[1]);
        switch (buf[0]) {
            case 'L': dial = (DIAL_END + dial - (num % DIAL_END)) % DIAL_END; break;
            case 'R': dial = (DIAL_END + dial + (num % DIAL_END)) % DIAL_END; break;
        }

        count += dial == DIAL_TARGET;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", count);
}
