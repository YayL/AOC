#include "common.h"
#include "hashmap.h"
#include "vector.h"
#include "list.h"
#include "ranges.h"

typedef struct rule {
    char * destination; // where to jump
    int comparision;
    char is_just_jump; // if this is just a straight jump rule
    char reg;
    char op;
} Rule;

typedef struct workflow {
    struct List * rules;
} Workflow;

typedef struct part {
    Range * x;
    Range * m;
    Range * a;
    Range * s;
} Part;

Workflow * new_workflow() {
    Workflow * workflow = malloc(sizeof(Workflow));
    workflow->rules = init_list(sizeof(Rule *));
    return workflow;
}

Rule * new_rule() {
    return calloc(1, sizeof(Rule));
}

Part * copy_part(Part * src) {
    Part * dest = malloc(sizeof(Part));
    dest->x = range_copy(src->x);
    dest->m = range_copy(src->m);
    dest->a = range_copy(src->a);
    dest->s = range_copy(src->s);

    return dest;
}

void set_post_comparision_range(char op, int value, Range * first, Range * second) {
    if (op == '<') {
        first->end = value - 1;
        second->start = value;
    } else {
        first->start = value + 1;
        second->end = value;
    }
}

// 
// OPTIMIZATION:
// store the start and end value instead of creating new ranges everytime that are passed along
void check_part(struct Vector * accepted, Part * part, const char * name, HashMap * workflows) {
    long x = range_get_width(part->x),
         m = range_get_width(part->m),
         a = range_get_width(part->a),
         s = range_get_width(part->s);
    if (x == 0 || m == 0 || a == 0 || s == 0) {
        return;
    }

    switch (name[0]) {
        case 'A':
        {
            vector_push(accepted, x * m * a * s);
            return;
        }
        case 'R':
            return;
    }

    Workflow * workflow = HM_get(workflows, name);
 
    if (workflow == NULL) {
        println("Workflow '{s}' was not found", name);
        exit(1);
    }

    Rule * rule;

    Part * first, * second = part;

    for (int i = 0; i < workflow->rules->size; ++i) {
        rule = list_at(workflow->rules, i);

        /* println("Jump to: {s}", rule->destination); */
        if (rule->is_just_jump) {
            return check_part(accepted, second, rule->destination, workflows);
        }

        first = copy_part(second), second = copy_part(second);

        switch (rule->reg) {
            case 'x':
                set_post_comparision_range(rule->op, rule->comparision, first->x, second->x);
                break;
            case 'm':
                set_post_comparision_range(rule->op, rule->comparision, first->m, second->m);
                break;
            case 'a':
                set_post_comparision_range(rule->op, rule->comparision, first->a, second->a);
                break;
            case 's':
                set_post_comparision_range(rule->op, rule->comparision, first->s, second->s);
                break;
            default:
                println("Invalid register: '{c}'", rule->reg);
                exit(1);
        }

        check_part(accepted, first, rule->destination, workflows);
    }

    println("Invalidly reached end of check_part function");
    exit(1);
}

int main() {

    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }
    
    long long result = 0;

    char * line = NULL, * name;
    size_t length = 0, read;

    HashMap * workflows = new_HashMap(8);

    Workflow * workflow;
    Rule * rule;

    char read_rules = 0;

    // parse workflows
    while ((read = getline(&line, &length, fp)) != -1) {
        if (line[0] == '\n') {
            break;
        }

        read -= 1;

        workflow = new_workflow();
        read_rules = 0;

        name = line;
        for (int i = 0; i < read; ++i) {
            if (!read_rules) {
                if (line[i] == '{') {
                    line[i] = '\0';
                    read_rules = 1;
                }
                continue;
            }

            rule = new_rule();

            if (line[i + 1] != '<' && line[i + 1] != '>') {
                rule->is_just_jump = 1;
                line[read - 1] = '\0';
                rule->destination = line + i;
                list_push(workflow->rules, rule);
                break;
            }

            rule->reg = line[i++];
            rule->op = line[i++];
            rule->comparision = strtol(line + i, &line, 10);
            i = line - name + 1;
            line = name;
            rule->destination = line + i;
            while (i < read) {
                if (line[i] == ',') {
                    line[i] = '\0';
                    break;
                } else if (line[i] == '\0') {
                    println("Encountered EOL/EOF while parsing rule");
                    exit(1);
                }
                i += 1;
            }
            list_push(workflow->rules, rule);
        }


        HM_set(workflows, name, workflow);
        line = NULL;
    }

    Part * part = malloc(sizeof(Part));
    part->x = new_range(1, 4000);
    part->m = new_range(1, 4000);
    part->a = new_range(1, 4000);
    part->s = new_range(1, 4000);

    struct Vector * accepted = init_vector();
    check_part(accepted, part, "in", workflows);

    for (int i = 0; i < accepted->size; ++i) {
        long combination = accepted->items[i];
        result += combination;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
