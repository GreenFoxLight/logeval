%{
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int yylex(void);

void yyerror(const char*);

extern bool result;
extern bool is_interactive;
%}

%debug

%require "3.0.4"
%code requires
{
    #include "logic_symbol.h"
    #include "parser.tab.h"
}

%define api.value.type union
%token END 0 "end of file"

%token <bool> SYMBOL 

%type <bool> formula 

/* define operator precedence */
%left EQL
%left IMPL
%left AND OR
%precedence NOT

%nonassoc LBRACKET
%nonassoc RBRACKET

%start line 
%%
line:
    %empty
    | line formula[f] ';' { result = $f; if (is_interactive) printf("\nResult: %d\n", result); }
    ;

formula:
       %empty { $$ = false; }
        | formula[l] AND formula[r] { $$ = $l && $r; }
        | formula[l] OR formula[r] { $$ = $l || $r; }
        | formula[l] IMPL formula[r] { 
                if ($l) {
                    $$ = $r;
                } else {
                    $$ = true;
                }
            }
        | formula[l] EQL formula[r] {
                $$ = ( $l == $r );
            }
        | NOT formula %prec NOT { $$ = !$2; }
        | LBRACKET formula RBRACKET { $$ = $2; }
        | SYMBOL    { $$ = $1; }
        ;

        

%%

void yyerror(const char* m) {
    fprintf(stderr, "%s\n", m);
}
