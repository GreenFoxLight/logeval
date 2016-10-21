%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "parser.tab.h"
#include "logic_symbol.h"

#undef yywrap
#define yywrap() 1

%}

%option noyywrap
%option nounput
%option noinput always-interactive
%option batch

DIGIT    [0-9]
WHITESPACE [ \t\n]

%%

{WHITESPACE}            { /* eat up whitespace */ }

";"                    { return ';'; }

    /* symbols */
"A"{DIGIT}+             {
                            int num = atoi(yytext+1);
                            yylval.SYMBOL = get_symbol_value(num);
                            return SYMBOL;
                        }

"0"                     {
                            yylval.SYMBOL = false;
                            return SYMBOL;
                        }

"1"                     {
                            yylval.SYMBOL = true;
                            return SYMBOL;
                        }

"quit"                  { return END; }

"symtab"                { print_symtab(); }

"help"                  {
                            printf("\n--- available commands:\n");
                            printf("   quit - Exit the program\n");
                            printf("   symtab - Print all known symbols and their values\n");
                            printf("   help - Display this help\n");
                            printf("\n--- syntax:\n");
                            printf("   An (eg. A0) - symbol n (replaced by its value)\n");
                            printf("   &  - Logical AND\n");
                            printf("   |  - Logical OR\n");
                            printf("   -> - Implication\n");
                            printf("   0  - Constant false\n");
                            printf("   1  - Constant true\n");
                        }
                        

    /* operators */
"&"                     { return AND; }

"|"                     { return OR; }

"->"                    { return IMPL; }

"!"                     { return NOT; }


"("                     { return LBRACKET; }

")"                     { return RBRACKET; }

    /* catch all */
<<EOF>>                 { return END; }

.                       { printf("Unrecognized character\n"); }

%%
