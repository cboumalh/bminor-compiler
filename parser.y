
%{
  #include <math.h>
  #include <stdio.h>
  #include "expr.h"
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  int yyerror(const char *s);

%}


%union {
  struct expr * e;
  struct type * t;
  struct param_list * p;
}

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
%token ARRAY
%token AUTO
%token BOOLEAN
%token CHAR
%token ELSE
%token FALSE
%token FLOAT
%token FOR
%token FUNCTION
%token IF
%token INTEGER
%token PRINT
%token RETURN
%token STRING
%token TRUE
%token VOID
%token WHILE

%nonassoc   INCREMENT_TOKEN DECREMENT_TOKEN 

%type <e> expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9 atomic expr_list opt_expr_list opt_expr
%type <p> param param_list opt_param_list
%type <t> all_types basic_types function_type array_type


%%
program: file_cmp_list {}
    |

expr1: expr2 ASSIGN_TOKEN expr1 { $$ = expr_create(EXPR_ASSIGN, $1, $3); } 
    |  expr2                    { $$ = $1; }

expr2: expr2 OR_TOKEN expr3 { $$ = expr_create(EXPR_OR, $1, $3); } 
    |  expr3                { $$ = $1; } 

expr3: expr3 AND_TOKEN expr4 { $$ = expr_create(EXPR_AND, $1, $3); } 
    |  expr4                 { $$ = $1; }

expr4: expr4 NOT_EQUAL_TOKEN expr5     { expr_create(EXPR_NOT_EQUAL, $1, $3); } 
    |  expr4 LESS_TOKEN expr5          { expr_create(EXPR_LESS, $1, $3); } 
    |  expr4 LESS_OR_EQ_TOKEN expr5    { expr_create(EXPR_LESS_OR_EQ, $1, $3); } 
    |  expr4 GREATER_TOKEN expr5       { expr_create(EXPR_GREATER, $1, $3); } 
    |  expr4 GREATER_OR_EQ_TOKEN expr5 { expr_create(EXPR_GREATER_OR_EQ, $1, $3); } 
    |  expr4 ASSERT_EQ_TOKEN expr5     { expr_create(EXPR_EQ, $1, $3); } 
    |  expr5                           { $$ = $1; } 

expr5: expr5 ADDITION_TOKEN expr6    { expr_create(EXPR_ADD, $1, $3); } 
    |  expr5 SUBTRACTION_TOKEN expr6 { expr_create(EXPR_SUB, $1, $3); } 
    |  expr6                         { $$ = $1; } 

expr6: expr6 MULTIPLY_TOKEN expr7 { expr_create(EXPR_MUL, $1, $3); } 
    |  expr6 DIVISION_TOKEN expr7 { expr_create(EXPR_DIV, $1, $3); } 
    |  expr6 MODULO_TOKEN expr7   { expr_create(EXPR_MODULO, $1, $3); } 
    |  expr7                      { $$ = $1; } 

expr7: expr8 POWER_TOKEN expr7  { expr_create(EXPR_POWER, $1, $3); }
    |  expr8                    { $$ = $1; }
    ;

expr8: SUBTRACTION_TOKEN expr8 { expr_create(EXPR_UNARY, $2, NULL); } 
    |  ADDITION_TOKEN expr8    { expr_create(EXPR_PLUS, $2, NULL); }
    |  NOT_TOKEN expr8         { expr_create(EXPR_NOT, $2, NULL); } 
    |  expr9                   { $$ = $1; } 
    ;

expr9: expr9 INCREMENT_TOKEN { expr_create(EXPR_INCREMENT, $1, NULL); } 
    |  expr9 DECREMENT_TOKEN { expr_create(EXPR_DECREMENT, $1, NULL); } 
    |  INCREMENT_TOKEN expr9 { expr_create(EXPR_INCREMENT, $2, NULL); } 
    |  DECREMENT_TOKEN expr9 { expr_create(EXPR_DECREMENT, $2, NULL); } 
    |  atomic                { $$ = $1; }
    ;

atomic: INT_TOKEN                                                 { $$ = expr_create_integer_literal(atoi(yytext)); } 
    |   FLOAT_TOKEN                                               { $$ = expr_create_float_literal(atof(yytext)); }
    |   ID_TOKEN                                                  { $$ = expr_create_name(yytext); }  
    |   STRING_TOKEN                                              { $$ = expr_create_string_literal(yytext); } 
    |   FALSE                                                     { $$ = expr_create_boolean_literal(0); } 
    |   TRUE                                                      { $$ = expr_create_boolean_literal(1); } 
    |   CHAR_TOKEN                                                { $$ = expr_create_char_literal(yytext); } 
    |   OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN                  { $$ = $2; } 
    |   ID_TOKEN OPEN_PARAN_TOKEN opt_expr_list CLOSE_PARAN_TOKEN { $$ = expr_create(EXPR_CALL, expr_create_name(yytext), $3); } 
    |   ID_TOKEN OPEN_BRACK_TOKEN expr1 CLOSE_BRACK_TOKEN         { $$ = expr_create(EXPR_SUBSCRIPT, expr_create_name(yytext), $3); } 
    ;

expr_list: expr_list COMMA_TOKEN expr1 { $$ = $1; $$->right = $3; } 
    |      expr1 { $$ = expr_create(EXPR_ARG, $1, NULL); } 
    ;

opt_expr_list: expr_list { $$ = $1; } 
    |                    { $$ = NULL; } 
    ;

opt_expr: expr1 { $$ = $1; }
    |           { $$ = NULL; }
    ;

all_types: AUTO     { $$ = type_create(TYPE_AUTO, NULL, NULL); } 
    | BOOLEAN       { $$ = type_create(TYPE_BOOLEAN, NULL, NULL); } 
    | CHAR          { $$ = type_create(TYPE_CHARACTER, NULL, NULL); } 
    | FLOAT         { $$ = type_create(TYPE_FLOAT, NULL, NULL); }
    | VOID          { $$ = type_create(TYPE_VOID, NULL, NULL); } 
    | INTEGER       { $$ = type_create(TYPE_INTEGER, NULL, NULL); } 
    | STRING        { $$ = type_create(TYPE_STRING, NULL, NULL); } 
    | function_type { $$ = $1; }
    | array_type    { $$ = $1; }
    ;

basic_types: AUTO  { $$ = type_create(TYPE_AUTO, NULL, NULL); } 
    | BOOLEAN      { $$ = type_create(TYPE_BOOLEAN, NULL, NULL); } 
    | CHAR         { $$ = type_create(TYPE_CHARACTER, NULL, NULL); } 
    | FLOAT        { $$ = type_create(TYPE_FLOAT, NULL, NULL); }
    | VOID         { $$ = type_create(TYPE_VOID, NULL, NULL); } 
    | INTEGER      { $$ = type_create(TYPE_INTEGER, NULL, NULL); } 
    | STRING       { $$ = type_create(TYPE_STRING, NULL, NULL); } 
    ;

function_type: FUNCTION all_types OPEN_PARAN_TOKEN opt_param_list CLOSE_PARAN_TOKEN { $$ = type_create(TYPE_FUNCTION, $2, $4); } 
    ;

array_type: ARRAY OPEN_BRACK_TOKEN opt_expr CLOSE_BRACK_TOKEN all_types { $$ = type_create(TYPE_ARRAY, $5, NULL); }
    ;

param: ID_TOKEN COLON_TOKEN all_types { $$ = param_list_create(yytext, $3, NULL); } 
    ;

param_list: param_list COMMA_TOKEN param  { $$ = $1; $$->next = $3; } 
    |       param                         { $$ = $1; }
    ;

opt_param_list: param_list { $$ = $1; } 
    |                      { $$ = NULL; } 
    ;


init: ASSIGN_TOKEN expr1 {}
    | ASSIGN_TOKEN OPEN_CURLY_TOKEN opt_expr_list CLOSE_CURLY_TOKEN  {}
    | 
    ;

decl: ID_TOKEN COLON_TOKEN basic_types init SEMICOLON_TOKEN  {}
    | ID_TOKEN COLON_TOKEN array_type init SEMICOLON_TOKEN  {}
    | ID_TOKEN COLON_TOKEN function_type ASSIGN_TOKEN OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN {}
    | ID_TOKEN COLON_TOKEN function_type SEMICOLON_TOKEN
    ;

comment: CPPCOMMENT_TOKEN
    | CCOMMENT_TOKEN
    ;


file_cmp: decl
    | comment
    ;

file_cmp_list: file_cmp_list file_cmp {}
    | file_cmp                        {}
    ;


flow_ending_stmt: SEMICOLON_TOKEN
    | stmt
    ;

flow_ending_if_dangling: SEMICOLON_TOKEN
    | if_dangling
    ;

stmt: FOR OPEN_PARAN_TOKEN opt_expr SEMICOLON_TOKEN opt_expr SEMICOLON_TOKEN opt_expr CLOSE_PARAN_TOKEN flow_ending_stmt
    | expr1 SEMICOLON_TOKEN
    | RETURN opt_expr SEMICOLON_TOKEN
    | decl
    | PRINT opt_expr_list SEMICOLON_TOKEN
    | OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN
    | IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN stmt
    | IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling ELSE stmt
    | WHILE OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN flow_ending_stmt
    ;


if_dangling: IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling ELSE if_dangling
    | decl
    | expr1 SEMICOLON_TOKEN
    | RETURN opt_expr SEMICOLON_TOKEN
    | PRINT opt_expr_list SEMICOLON_TOKEN
    | FOR OPEN_PARAN_TOKEN opt_expr SEMICOLON_TOKEN opt_expr SEMICOLON_TOKEN opt_expr CLOSE_PARAN_TOKEN flow_ending_if_dangling
    | WHILE OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN flow_ending_if_dangling
    | OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN
    ;


decl_body: comment
    | stmt

decl_body_list: decl_body_list decl_body {}
    |
    ;

%%


int yyerror(const char *msg) {
    fprintf(stderr, "Parser error: %s\n", msg);
    return 1;
}
