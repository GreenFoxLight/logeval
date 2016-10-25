#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "logic_symbol.h"
#include "parser.tab.h"

logsym* symtab;
int num_symbols;

void add_symbol(int num, bool value);
void read_symtab(const char* file);
void destroy_symtab(logsym* sym);

extern FILE* yyin;

void usage(const char* p) {
    printf("Usage: %s <symbol_table> [-e|--enumerate] [-f|--file formula-file]\n", p);
    exit(0);
}

int main(int argc, char** argv) {
    bool do_enumerate = false;
    num_symbols = 0;
    if (argc < 2)
        usage(argv[0]);
    else if (argc >= 4) {
        yyin = stdin;
        for (int i = 2; i < (argc - 1); i++) {
            if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
                FILE* f = fopen(argv[i+1], "r");
                if (!f) {
                    fprintf(stderr, "Error while opening file %s!\n", argv[i+1]);
                    exit(1);
                }
                yyin = f; 
            } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--enumerate") == 0) {
                do_enumerate = true;
            }    
        }
    } else {
        yyin = stdin;
    }

    if (do_enumerate) {
        if (yyin == stdin) {
            printf("ERROR: Option --enumerate requires formula-file!\n");
            exit(1);
        }
        symtab = NULL;
        read_symtab(argv[1]);
        reset_symbols();
        for (unsigned long i = 0; i < (1 << num_symbols); i++) {
            print_symtab();
            yyparse();
            rewind(yyin);
            increment_symbols(symtab);
        } 
    } else {
        symtab = NULL;
        read_symtab(argv[1]);
        yyparse();
    }

    if (yyin != stdin)
        fclose(yyin);
    if (symtab)
        destroy_symtab(symtab);

    return 0;
}

void destroy_symtab(logsym* sym) {
    if (sym->next)
        destroy_symtab(sym->next);
    free(sym);
}

bool get_symbol_value(int num) {
    if (!symtab) {
        fprintf(stderr, "-- Trying to access non-existing symbol %d!\n", num);
        exit(1);
    }
    logsym* t = symtab;
    do {    
        if (t->num == num)
            return t->value;
        t = t->next;
    } while (t);
    fprintf(stderr, "++ Trying to access non-existing symbol %d!\n", num);
    exit(1);
}

void add_symbol(int num, bool value) {
    if (!symtab) {
        symtab = (logsym*)malloc(sizeof(logsym));
        if (!symtab) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
        symtab->num = num;
        symtab->value = value;
        symtab->next = NULL;
    } else {
        logsym* new = (logsym*)malloc(sizeof(logsym));
        if (!new) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
        new->num = num;
        new->value = value;
        new->next = symtab;
        symtab = new;
    }
    num_symbols++;
}

void read_symtab(const char* file) {
    FILE* f = fopen(file, "r");
    if (!f) {
        fprintf(stderr, "Error while reading file %s\n", file);
        exit(1);
    }

    int num, value;
    for (; fscanf(f, "%d: %d", &num, &value) == 2;) {
        if (value > 0)
            add_symbol(num, true);
        else
            add_symbol(num, false);
    }

    fclose(f);
}

void print_symtab() {
    logsym* sym = symtab;
    printf("\n------ Symbol Table ------\n");
    while (sym) {
        printf("A%d: %d\n", sym->num, sym->value);
        sym = sym->next;
    }
    printf("------ End of Table ------\n");
}

void reset_symbols() {
    logsym* sym = symtab;
    while (sym) {
        sym->value = false;
        sym = sym->next;
    }
}

void increment_symbols(logsym* start) {
    logsym* sym = start;
    if (!sym)
        return;
    if (sym->value) {
        sym->value = false;
        increment_symbols(sym->next);
    } else {
        sym->value = true;
    }
}
