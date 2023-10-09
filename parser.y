/* Infix notation calculator. */

%{
  #include <math.h>
  #include <stdio.h>
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  int yyerror(const char *s); 
%}


%token INDEX_SHIFTER_TOKEN
%token NOTHING_TOKEN
%token CCOMMENT_TOKEN
%token CPPCOMMENT_TOKEN
%token CHAR_TOKEN
%token STRING_TOKEN
%token INT_TOKEN
%token FLOAT_TOKEN
%token ID_TOKEN
%token ERROR_TOKEN
%token SEMICOLON_TOKEN
%token GREATER_OR_EQ_TOKEN
%token GREATER_TOKEN
%token LESS_OR_EQ_TOKEN
%token LESS_TOKEN
%token ASSERT_EQ_TOKEN
%token KEYWORD_TOKEN
%token POWER_TOKEN
%token ADDITION_TOKEN
%token SUBTRACTION_TOKEN
%token DIVISION_TOKEN
%token NOT_EQUAL_TOKEN
%token OR_TOKEN
%token AND_TOKEN
%token NOT_TOKEN
%token MULTIPLY_TOKEN
%token MODULO_TOKEN
%token COLON_TOKEN
%token OPEN_PARAN_TOKEN
%token CLOSE_PARAN_TOKEN
%token DECREMENT_TOKEN
%token INCREMENT_TOKEN
%token COMMA_TOKEN
%token OPEN_BRACK_TOKEN
%token CLOSE_BRACK_TOKEN
%token OPEN_CURLY_TOKEN
%token CLOSE_CURLY_TOKEN
%token ASSIGN_TOKEN

%%
program: expr1 SEMICOLON_TOKEN { printf("%d\n", $1); return 0; }
    |
    ;

expr1: expr2 ASSIGN_TOKEN expr1   { $$ = $3; }
    |  expr2                      { $$ = $1; }
    ;

expr2: expr2 OR_TOKEN expr3   { $$ = $1 || $3; }
    |  expr3                    { $$ = $1; }
    ;

expr3: expr3 AND_TOKEN expr4 { $$ = $1 && $3; }
    |  expr4                { $$ = $1; }
    ;

expr4: expr4 NOT_EQUAL_TOKEN expr5 { $$ = $1 != $3; }
    |  expr4 LESS_TOKEN expr5 { $$ = $1 < $3; }
    |  expr4 LESS_OR_EQ_TOKEN expr5 { $$ = $1 <= $3; }
    |  expr4 GREATER_TOKEN expr5 { $$ = $1 > $3; }
    |  expr4 GREATER_OR_EQ_TOKEN expr5 { $$ = $1 >= $3; }
    |  expr4 ASSERT_EQ_TOKEN expr5 { $$ = $1 == $3; }
    |  expr5                        { $$ = $1; }
    ;

expr5: expr5 ADDITION_TOKEN expr6 { $$ = $1 + $3; }
    |  expr5 SUBTRACTION_TOKEN expr6 { $$ = $1 - $3; }
    |  expr6                        { $$ = $1; }
    ;

expr6: expr6 MULTIPLY_TOKEN expr7 { $$ = $1 * $3; }
    |  expr6 DIVISION_TOKEN expr7 { $$ = $1 / $3; }
    |  expr6 MODULO_TOKEN expr7 { $$ = $1 % $3; }
    |  expr7                     { $$ = $1; }
    ;

expr7: expr8 POWER_TOKEN expr7  { $$ = pow($1, $3); }
    |  expr8                    { $$ = $1; }
    ;

expr8: SUBTRACTION_TOKEN expr8 { $$ =  -1 * $2; }
    |  NOT_TOKEN expr8  { $$ = !$2; }
    |  expr9             { $$ = $1; }

expr9: INCREMENT_TOKEN expr8 { $$ =  -1 * $2; }
    |  DECREMENT_TOKEN expr8  { $$ = !$2; }
    |  expr10             { $$ = $1; }

expr10: INT_TOKEN        { $$ = atoi(yytext); }
    ;
%%


int yyerror(const char *msg) {
    fprintf(stderr, "Parser error: %s\n", msg);
    return 1;
}
