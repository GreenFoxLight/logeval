#ifndef LOGIC_SYMBOL_H
#define LOGIC_SYMBOL_H

#include <stdbool.h>

struct logsym {
    int num;
    bool value;

    struct logsym* prev;
    struct logsym* next;
};
typedef struct logsym logsym;

bool get_symbol_value(int num);
void print_symtab();

void reset_symbols();
void increment_symbols(logsym* start);

#endif
