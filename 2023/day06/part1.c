#include "common.h"
#include "list.h"

typedef struct RaceRecord {
    unsigned long long time;
    unsigned long long distance;
} Record;

size_t get_next_number_in_string(const char * str, int * offset) {
    int temp;
    size_t value;
    sscanf(str + *offset, "%*[^0123456789]%lu%n", &value, &temp);
    *offset += temp;

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
    size_t length = 0;
 
    String * time = new_string_len(line, getline(&line, &length, fp));
    String * distance = new_string_len(line, getline(&line, &length, fp));
    Record * record;
    struct List * records = init_list(sizeof(Record *));

    int time_offset = 0, dist_offset = 0;
    
    while (time_offset < time->length - 1) {
        record = malloc(sizeof(Record));
        record->time = get_next_number_in_string(time->c_str, &time_offset);
        record->distance = get_next_number_in_string(distance->c_str, &dist_offset);
        list_push(records, record);
    }

    double discriminant;
    size_t product = 1;

    for (int i = 0; i < records->size; ++i) {
        record = list_at(records, i);
        
        discriminant = sqrt(record->time * record->time - 4 * record->distance);
        double min = (record->time - discriminant) / 2;
        double max = (record->time + discriminant) / 2;

        product *= 1 + fix_max(max) - fix_min(min);
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lu}", product);
}
