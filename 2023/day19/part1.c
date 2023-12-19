#include "common.h"
#include "hashmap.h"
#include "list.h"

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
    int x;
    int m;
    int a;
    int s;
} Part;

Workflow * new_workflow() {
    Workflow * workflow = malloc(sizeof(Workflow));
    workflow->rules = init_list(sizeof(Rule *));
    return workflow;
}

Rule * new_rule() {
    return calloc(1, sizeof(Rule));
}

void print_rules(struct List * rules) {
    Rule * rule;
    for (int i = 0; i < rules->size; ++i) {
        rule = list_at(rules, i);
        if (rule->is_just_jump) {
            println("Jump: '{s}'", rule->destination);
        } else {
            println("Reg: '{c}', Op: '{c}', Comp: {i}, Dest: '{s}'", rule->reg, rule->op, rule->comparision, rule->destination);
        }
    }
}

int check_part(Part * part, const char * name, HashMap * workflows) {
    switch (name[0]) {
        case 'A':
            return 1;
        case 'R':
            return 0;
    }

    Workflow * workflow = HM_get(workflows, name);

    /* println("(x={i}, m={i}, a={i}, s={i})", part->x, part->m, part->a, part->s); */
    
    if (workflow == NULL) {
        println("Workflow '{s}' was not found", name);
        exit(1);
    }

    Rule * rule;

    for (int i = 0; i < workflow->rules->size; ++i) {
        rule = list_at(workflow->rules, i);

        /* println("Jump to: {s}", rule->destination); */
        if (rule->is_just_jump) {
            return check_part(part, rule->destination, workflows);
        }

        int value;

        switch (rule->reg) {
            case 'x':
                value = part->x; break;
            case 'm':
                value = part->m; break;
            case 'a':
                value = part->a; break;
            case 's':
                value = part->s; break;
            default:
                println("Invalid register: '{c}'", rule->reg);
                exit(1);
        }

        /* println("{2c}{i}", rule->reg, rule->op, rule->comparision); */

        if ((rule->op == '<' && value < rule->comparision) || (rule->op == '>' && value > rule->comparision)) {
            return check_part(part, rule->destination, workflows);
        }
    }

    println("Invalidly reached end of check_part function");
    exit(1);
}

void check_parts(struct List * accepted, HashMap * workflows, struct List * parts) {
    for (int i = 0; i < parts->size; ++i) {
        if (check_part(parts->items[i], "in", workflows)) {
            list_push(accepted, parts->items[i]);
        }
    }
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
    struct List * parts = init_list(sizeof(Part *)); 
    struct List * accepted = init_list(sizeof(Part *));

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

    Part * part;
    int x, m, a, s;
    // parse parts
    while ((read = getline(&line, &length, fp)) != -1) {
        sscanf(line, "{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s);
        part = malloc(sizeof(Part));
        part->x = x, part->m = m, part->a = a, part->s = s;
        list_push(parts, part);
    }

    check_parts(accepted, workflows, parts);

    for (int i = 0; i < accepted->size; ++i) {
        part = accepted->items[i];
        result += part->x + part->m + part->a + part->s;
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {lli}", result);
}
