#include "common.h"
#include "hashmap.h"
#include "list.h"

long low = 0, high = 0;

typedef enum signal {
    LOW,
    HIGH
} Signal;

typedef struct module {
    struct List * sendees;
    int storage;
    enum module_type {
        BROADCASTER,
        FLIP_FLOP,
        CONJUNCTION
    } type;
} Module;

Module * new_module(enum module_type type) {
    Module * module = malloc(sizeof(Module));
    module->storage = type == CONJUNCTION;
    module->type = type;
    module->sendees = init_list(sizeof(const char *));

    return module;
}

void update_module(Module * module, Signal signal) {

}

void send_pulse(HashMap * modules, const char * dest, Signal signal) {
    switch (signal) {
        case LOW:
            low += 1; break;
        case HIGH:
            high += 1; break;
    }
    Module * module = HM_get(modules, dest);

    if (module == NULL) {
        println("Module '{s}' not found", dest);
        exit(1);
    }

    switch (module->type) {
        case FLIP_FLOP:
            if (signal == HIGH) {
                return;
            }
            signal = module->storage = !module->storage;
            break;
        case CONJUNCTION:
        {
            Signal temp = module->storage;
            signal = !module->storage;
            module->storage = temp;
            break;
        }
    }

    for (int i = 0; i < module->sendees->size; ++i) {
        update_module(module, signal);
    }

    for (int i = 0; i < module->sendees->size; ++i) {
        send_pulse(modules, list_at(module->sendees, i), dest, signal);
    }
}

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    char * line = NULL, * name, * temp;
    size_t length = 0, read;

    HashMap * modules = new_HashMap(1 << 16);
    Module * module;

    while ((read = getline(&line, &length, fp)) != -1) {
        switch (line[0]) {
            case '%':
                module = new_module(FLIP_FLOP); name = line + 1; break;
            case '&':
                module = new_module(CONJUNCTION); name = line + 1; break;
            default:
                module = new_module(BROADCASTER); name = line; break;
        }

        int i = name - line;
        while (line[++i] != ' ');
        line[i] = '\0';

        i += 3;

        while (i < read) {
            switch (line[i]) {
                case ' ':
                    temp = line + i + 1; break;
                case '\n':
                case ',':
                    list_push(module->sendees, temp);
                    line[i] = '\0'; break;
            }
            i += 1;
        }

        /* println("Name: '{s}'", name); */
        
        HM_set(modules, name, module);
        line = NULL;
    }

    /* for (int i = 0; i < 1000; ++i) { */
        send_pulse(modules, "broadcaster", "button", LOW);
    /* } */

    println("Low: {i}", low);
    println("High: {i}", high);

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", low * high);
}
