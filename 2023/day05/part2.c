#include "common.h"
#include "vector.h"
#include "list.h"

#define read_map(list)  sscanf(line, "%ld %ld %ld", &dest_start, &src_start, &map_length);\
                        range = init_range(src_start, map_length, dest_start);\
                        list_push(list, range)

struct Range {
    size_t src_start;
    size_t src_end;
    size_t dest_start;
};

struct Range * init_range(size_t src_start, size_t map_length, size_t dest_start) {
    struct Range * range = malloc(sizeof(struct Range));
    range->src_start = src_start;
    range->src_end = src_start + map_length;
    range->dest_start = dest_start;
    return range;
}

struct List * seed_to_soil;
struct List * soil_to_fert;
struct List * fert_to_water;
struct List * water_to_light;
struct List * light_to_temp;
struct List * temp_to_hum;
struct List * hum_to_loc;

size_t convert(size_t value, struct List * list) {
    struct Range * range;
    for (size_t i = 0; i < list->size; ++i) {
        range = list_at(list, i);
        if (range->src_start <= value && value < range->src_end) {
            return range->dest_start + (value - range->src_start);
        }
    }
    
    return value;
}

size_t convert_seed_to_location(size_t seed) {
    seed = convert(seed, seed_to_soil);
    seed = convert(seed, soil_to_fert);
    seed = convert(seed, fert_to_water);
    seed = convert(seed, water_to_light);
    seed = convert(seed, light_to_temp);
    seed = convert(seed, temp_to_hum);
    return convert(seed, hum_to_loc);
}

int main() {

    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read;
    
    struct List * seeds = init_list(sizeof(struct Range *));
    seed_to_soil = init_list(sizeof(struct Range *));
    soil_to_fert = init_list(sizeof(struct Range *));
    fert_to_water = init_list(sizeof(struct Range *));
    water_to_light = init_list(sizeof(struct Range *));
    light_to_temp = init_list(sizeof(struct Range *));
    temp_to_hum = init_list(sizeof(struct Range *));
    hum_to_loc = init_list(sizeof(struct Range *));

    struct Range * range;

    enum mode_t {
        SEED,
        SEED_TO_SOIL,
        SOIL_TO_FERT,
        FERT_TO_WATER,
        WATER_TO_LIGHT,
        LIGHT_TO_TEMP,
        TEMP_TO_HUM,
        HUM_TO_LOC,
    } mode = 0;

    size_t dest_start, src_start, map_length;

    while ((read = getline(&line, &length, fp)) != -1) {
        if (line[0] == '\n') {
            mode += 1;
            getline(&line, &length, fp); // discard the description line
            continue;
        }
        
        switch (mode) {
            case SEED:
            {
                int offset;
                for (size_t i = 7; i < read;) { // 7 is the length of "seeds: "
                    sscanf(line + i, "%ld %ld%n", &src_start, &map_length, &offset);
                    range = init_range(src_start, map_length, src_start);
                    list_push(seeds, range); // dest_start is the read seed in this case
                    i += offset + 1; // src_start is the local offset to the next number
                }
                break;
            }
            case SEED_TO_SOIL:
                read_map(seed_to_soil); break;
            case SOIL_TO_FERT:
                read_map(soil_to_fert); break;
            case FERT_TO_WATER:
                read_map(fert_to_water); break;
            case WATER_TO_LIGHT:
                read_map(water_to_light); break;
            case LIGHT_TO_TEMP:
                read_map(light_to_temp); break;
            case TEMP_TO_HUM:
                read_map(temp_to_hum); break;
            case HUM_TO_LOC:
                read_map(hum_to_loc); break;
            default:
                println("Error parsing mappings. Gone above HUM_TO_LOC");
                exit(1);
        }
    }
    
    size_t min = ULONG_MAX;

    println("Seeds: {i}", seeds->size);


    for (size_t i = 0; i < seeds->size; ++i) {
        struct Range r = *((struct Range *) list_at(seeds, i));
        for (int j = r.src_start; j < r.src_end; ++j) {
            size_t location = convert_seed_to_location(j);
            if (location < min) 
                min = location;
        }
        println("{i}: {i}", i, min);
    }

    println("Min: {i}", min);
}
