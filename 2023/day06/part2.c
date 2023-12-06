#include "common.h"
#include "list.h"

typedef struct RaceRecord {
    unsigned long long time;
    unsigned long long distance;
} Record;

unsigned long long get_number(const char * str, size_t length) {
    unsigned long long value = 0;

    for (int i = 0; i < length; ++i) {
        if (isdigit(str[i])) {
            value = 10 * value + str[i] - '0';
        }
    }

    return value;
}

int fix_min(double value) {
    double fvalue = ceil(value);
    if (fvalue == value)
        return fvalue + 1;
    return fvalue;
}

int fix_max(double value) {
    double fvalue = floor(value);
    if (fvalue == value)
        return fvalue - 1;
    return fvalue;
}

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read;
 
    Record record;
 
    read = getline(&line, &length, fp);
    record.time = get_number(line, read);

    read = getline(&line, &length, fp);
    record.distance = get_number(line, read);

    double discriminant = sqrt(record.time * record.time - 4 * record.distance);
    double min = (record.time - discriminant) / 2;
    double max = (record.time + discriminant) / 2;

    size_t product = 1 + fix_max(max) - fix_min(min);

    printf("Execution time: %.3fms\n", (double)stop_timer() / 1000);
    println("Result: {lu}", product);
}
