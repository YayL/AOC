#include "common.h"
#include "list.h"

#define WIDTH 110
#define HEIGHT WIDTH

// mirror map definitions
#define IS_EMPTY (0)
#define IS_ENERGIZED (1 << 0)
#define IS_VERTICAL_SPLITTER (1 << 1)
#define IS_HORIZONTAL_SPLITTER (1 << 2)
#define IS_RIGHT_TILTING_REFLECTOR (1 << 3)
#define IS_LEFT_TILTING_REFLECTOR (1 << 4)
#define ALREADY_SPLIT_HERE (1 << 5)

#define SLOT_TYPE_MASK (IS_LEFT_TILTING_REFLECTOR | IS_RIGHT_TILTING_REFLECTOR | IS_HORIZONTAL_SPLITTER | IS_VERTICAL_SPLITTER)

char map[HEIGHT][WIDTH];
size_t width, height = 0;

typedef struct beam {
    int x;
    int y;
    enum beam_direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    } direction;
} Beam;

#define IS_HORIZONTAL_MOVEMENT(x) ((x) & 1)

Beam * new_beam(int x, int y, enum beam_direction dir) {
    Beam * beam = malloc(sizeof(Beam));
    beam->x = x;
    beam->y = y;
    beam->direction = dir;
    return beam;
}

char char_to_slot(char value) {
    switch (value) {
        case '.':
            return IS_EMPTY;
        case '|':
            return IS_VERTICAL_SPLITTER;
        case '-':
            return IS_HORIZONTAL_SPLITTER;
        case '\\':
            return IS_LEFT_TILTING_REFLECTOR;
        case '/':
            return IS_RIGHT_TILTING_REFLECTOR;
    }
    println("Invalid characther: '{c}'", value);
    exit(1);
}

void simulate_beam(struct List * beams, Beam * beam_ref) {
    Beam beam = *beam_ref;

    while (0 <= beam.x && beam.x < width && 0 <= beam.y && beam.y < height) {
        /* println("Beam: ({i}, {i})", beam.x, beam.y); */
        /* fgetc(stdin); */
        switch (map[beam.y][beam.x] & SLOT_TYPE_MASK) {
            case IS_HORIZONTAL_SPLITTER:
            {
                if (map[beam.y][beam.x] & ALREADY_SPLIT_HERE) {
                    return;
                }

                if (!IS_HORIZONTAL_MOVEMENT(beam.direction)) {
                    list_push(beams, new_beam(beam.x + 1, beam.y, RIGHT));
                    beam.direction = LEFT;
                }
                map[beam.y][beam.x] |= ALREADY_SPLIT_HERE;
                break;
            }
            case IS_VERTICAL_SPLITTER:
            {
                if (map[beam.y][beam.x] & ALREADY_SPLIT_HERE) {
                    return;
                }

                if (IS_HORIZONTAL_MOVEMENT(beam.direction)) {
                    list_push(beams, new_beam(beam.x, beam.y - 1, UP));
                    beam.direction = DOWN;
                }

                map[beam.y][beam.x] |= ALREADY_SPLIT_HERE;
                break;
            }
            case IS_LEFT_TILTING_REFLECTOR:
                beam.direction = LEFT - beam.direction; break;
            case IS_RIGHT_TILTING_REFLECTOR:
                beam.direction = ((LEFT - beam.direction + 2) & 3); break;
        }
        map[beam.y][beam.x] |= IS_ENERGIZED;
        beam.x += IS_HORIZONTAL_MOVEMENT(beam.direction) * (2 - beam.direction);
        beam.y += !IS_HORIZONTAL_MOVEMENT(beam.direction) * (beam.direction - 1);
    }
    /* println("== BEAM DIED ==\n"); */
}

int count_energized_and_reset_board() {
    int count = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            count += map[y][x] & IS_ENERGIZED;
            map[y][x] &= SLOT_TYPE_MASK;
        }
    } 

    return count;
}

int run_simulation_from(struct List * beams, int x, int y, enum beam_direction dir) {
    Beam * beam;
    list_push(beams, new_beam(x, y, dir));

    while (beams->size) {
        beam = list_pop(beams);
        simulate_beam(beams, beam);
    }

    return count_energized_and_reset_board();
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
    
    while ((read = getline(&line, &length, fp)) != -1) {
        width = read - 1;
        if (WIDTH < width) {
            println("WIDTH is too little. Increase to {i}", width);
        }

        for (int x = 0; x < width; ++x) {
            map[height][x] = char_to_slot(line[x]);
        }

        height += 1;
    }

    if (WIDTH < height) {
        println("HEIGHT is too little. Increase to {i}", width);
    }

    int max = 0, count;
    struct List * beams = init_list(sizeof(Beam *));

    for (int x = 0; x < width; ++x) {
        count = run_simulation_from(beams, x, 0, DOWN);
        if (max < count) {
            max = count;
        }
    }

    for (int x = 0; x < width; ++x) {
        count = run_simulation_from(beams, x, height - 1, UP);
        if (max < count) {
            max = count;
        }
    }

    for (int y = 0; y < width; ++y) {
        count = run_simulation_from(beams, 0, y, RIGHT);
        if (max < count) {
            max = count;
        }
    }

    for (int y = 0; y < width; ++y) {
        count = run_simulation_from(beams, width - 1, y, LEFT);
        if (max < count) {
            max = count;
        }
    }


    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", max);
}
