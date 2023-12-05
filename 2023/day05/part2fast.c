#include "common.h"
#include "ranges.h"

int main() {

    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read; 

    Range * range = new_range(0, 0), * temp;

    size_t dest_start, src_start, map_length;
    
    read = getline(&line, &length, fp);
    int offset;

    for (size_t i = 7; i < read;) { // 7 is the length of "seeds: "
        sscanf(line + i, "%lu %lu%n", &src_start, &map_length, &offset);
        temp = new_range(src_start, src_start + map_length);
        range_insert(range, temp);
        i += offset + 1; // src_start is the local offset to the next number
    }

    range = range->next;

    temp = range;
    while (temp != NULL) {
        println("[{lu}...{lu}]", temp->start, temp->end);
        temp = temp->next;
    }

    exit(0);

    while ((read = getline(&line, &length, fp)) != -1) {
        if (line[0] == '\n') {
            getline(&line, &length, fp); // discard the description line
            continue;
        }     

    }

}
