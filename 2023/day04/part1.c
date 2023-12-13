#include "common.h"
#include "timer.h"
#include "vector.h"

int main() {

	start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read;
    
    struct Vector * winnings = init_vector();
    struct Vector * numbers = init_vector();

    char c, number_list;
    int offset = 0, number, temp, sum = 0;

    while ((read = getline(&line, &length, fp)) != -1) {
        sscanf(line, "Card %*d:%n", &offset);
        number_list = 0;
        for (; offset < read; ++offset) {
            c = line[offset];
            if (isdigit(c)) {
                sscanf(line + offset, "%d%n", &number, &temp);
                offset += temp;
                vector_push(number_list ? numbers : winnings, number);
            } else if (c == '|') {
                number_list = 1;
            }
        }
        
        temp = 1;
        for (int i = 0; i < numbers->size; ++i) {
            number = vector_at(numbers, i);
            if (vector_contains(winnings, number)) {
                temp <<= 1;
            }
        }

        sum += temp >> 1;
        
        vector_clear(winnings);
        vector_clear(numbers);

    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Sum: {i}", sum);

}
