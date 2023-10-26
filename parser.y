
%{
  #include <math.h>
  #include <stdio.h>
  #include "decl.h"
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  int yyerror(const char *s);
  #include "parser_result.h"

  struct decl *parser_result = 0;

%}


%union {
  struct expr * e;
  struct type * t;
  struct param_list * p;
  struct decl * d;
  struct stmt * s;
  char * c;
}

%token NOTHING_TOKEN
%token CCOMMENT_TOKEN
%token CPPCOMMENT_TOKEN
%token CHAR_TOKEN
%token STRING_TOKEN
%token INT_TOKEN
%token FLOAT_TOKEN
%token <c> ID_TOKEN
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

%type <e> expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9 atomic expr_list opt_expr_list opt_expr subscript_list array_init_value basic_init array_init array_init_value_T subscript_list_T expr_arg
%type <p> param param_list opt_param_list
%type <t> all_types basic_types function_type array_type
%type <s> decl_body if_dangling stmt flow_ending_if_dangling flow_ending_stmt decl_body_list
%type <d> decl file_body file_body_list
%type <c> comment



%%
program: file_body_list { parser_result = $1; }
    |                   { parser_result = NULL; }

expr1: expr2 ASSIGN_TOKEN expr1 { $$ = expr_create(EXPR_ASSIGN, $1, $3); } 
    |  expr2                    { $$ = $1; }

expr2: expr2 OR_TOKEN expr3 { $$ = expr_create(EXPR_OR, $1, $3); } 
    |  expr3                { $$ = $1; } 

expr3: expr3 AND_TOKEN expr4 { $$ = expr_create(EXPR_AND, $1, $3); } 
    |  expr4                 { $$ = $1; }

expr4: expr4 NOT_EQUAL_TOKEN expr5     { $$ = expr_create(EXPR_NOT_EQUAL, $1, $3); } 
    |  expr4 LESS_TOKEN expr5          { $$ = expr_create(EXPR_LESS, $1, $3); } 
    |  expr4 LESS_OR_EQ_TOKEN expr5    { $$ = expr_create(EXPR_LESS_OR_EQ, $1, $3); } 
    |  expr4 GREATER_TOKEN expr5       { $$ = expr_create(EXPR_GREATER, $1, $3); } 
    |  expr4 GREATER_OR_EQ_TOKEN expr5 { $$ = expr_create(EXPR_GREATER_OR_EQ, $1, $3); } 
    |  expr4 ASSERT_EQ_TOKEN expr5     { $$ = expr_create(EXPR_EQ, $1, $3); } 
    |  expr5                           { $$ = $1; } 

expr5: expr5 ADDITION_TOKEN expr6    { $$ = expr_create(EXPR_ADD, $1, $3); } 
    |  expr5 SUBTRACTION_TOKEN expr6 { $$ = expr_create(EXPR_SUB, $1, $3); } 
    |  expr6                         { $$ = $1; } 

expr6: expr6 MULTIPLY_TOKEN expr7 { $$ = expr_create(EXPR_MUL, $1, $3); } 
    |  expr6 DIVISION_TOKEN expr7 { $$ = expr_create(EXPR_DIV, $1, $3); } 
    |  expr6 MODULO_TOKEN expr7   { $$ = expr_create(EXPR_MODULO, $1, $3); } 
    |  expr7                      { $$ = $1; } 

expr7: expr8 POWER_TOKEN expr7  { $$ = expr_create(EXPR_POWER, $1, $3); }
    |  expr8                    { $$ = $1; }
    ;

expr8: SUBTRACTION_TOKEN expr9 { $$ = expr_create(EXPR_UNARY, NULL, $2); } 
    |  ADDITION_TOKEN expr9    { $$ = expr_create(EXPR_PLUS, NULL, $2); }
    |  NOT_TOKEN expr9         { $$ = expr_create(EXPR_NOT, NULL, $2); } 
    |  expr9                   { $$ = $1; } 
    ;

expr9: atomic INCREMENT_TOKEN { $$ = expr_create(EXPR_INCREMENT, $1, NULL); } 
    |  atomic DECREMENT_TOKEN { $$ = expr_create(EXPR_DECREMENT, $1, NULL); } 
    |  INCREMENT_TOKEN atomic { $$ = expr_create(EXPR_INCREMENT, NULL, $2); } 
    |  DECREMENT_TOKEN atomic { $$ = expr_create(EXPR_DECREMENT, NULL, $2); } 
    |  atomic                 { $$ = $1; }
    ;

atomic: INT_TOKEN                                                 { $$ = expr_create_integer_literal(atoi(yytext)); } 
    |   FLOAT_TOKEN                                               { $$ = expr_create_float_literal(atof(yytext)); }
    |   ID_TOKEN                                                  { $$ = expr_create_name($1); }  
    |   STRING_TOKEN                                              { $$ = expr_create_string_literal(yytext); } 
    |   FALSE                                                     { $$ = expr_create_boolean_literal(0); } 
    |   TRUE                                                      { $$ = expr_create_boolean_literal(1); } 
    |   CHAR_TOKEN                                                { $$ = expr_create_char_literal(yytext); } 
    |   OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN                  { $$ = $2; } 
    |   ID_TOKEN OPEN_PARAN_TOKEN opt_expr_list CLOSE_PARAN_TOKEN { $$ = expr_create(EXPR_CALL, expr_create_name($1), $3); }
    |   ID_TOKEN subscript_list                                   {
                                                                    struct expr * e = $2;
                                                                    struct expr * fixed_e = e;

                                                                    if(e->kind == EXPR_SUBSCRIPT){
                                                                        while(e->left && e->left->kind == EXPR_SUBSCRIPT)
                                                                            e = e->left;

                                                                        struct expr * val = e->left;

                                                                        e->left = expr_create(EXPR_SUBSCRIPT, expr_create_name($1), val);

                                                                        $$ = fixed_e;  
                                                                    }
                                                                    else{
                                                                        $$ = expr_create(EXPR_SUBSCRIPT, expr_create_name($1), $2);
                                                                    }
                                                                  }
    ;

subscript_list: subscript_list subscript_list_T    { $$ = expr_create(EXPR_SUBSCRIPT, $1, $2); }
    | subscript_list_T                             { $$ = $1; }
    ;   

subscript_list_T: OPEN_BRACK_TOKEN expr1 CLOSE_BRACK_TOKEN    { $$ = $2; }
    ;

expr_list: expr_arg COMMA_TOKEN expr_list { $1->right = $3; $$ = $1; } 
    |      expr_arg                       { $$ = $1; } 
    ;

expr_arg: expr1 { $$ = expr_create(EXPR_ARG, $1, NULL); } 
    ;

opt_expr_list: expr_list { $$ = $1; } 
    |                    { $$ = NULL; } 
    ;

array_init_value: array_init_value_T COMMA_TOKEN array_init_value                              { $1->right = $3; $$ = $1; }
    | array_init_value_T                                                                       { $$ = $1; }
    ;

array_init_value_T: OPEN_CURLY_TOKEN expr_list CLOSE_CURLY_TOKEN                              { $$ = expr_create(EXPR_ARRAY_DECL, $2, NULL); }
    | OPEN_CURLY_TOKEN array_init_value CLOSE_CURLY_TOKEN                                     { $$ = expr_create(EXPR_ARRAY_DECL, $2, NULL); }
    ;

opt_expr: expr1 { $$ = $1; }
    |           { $$ = NULL; }
    ;

all_types: AUTO     { $$ = type_create(TYPE_AUTO, NULL, NULL, NULL); } 
    | BOOLEAN       { $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); } 
    | CHAR          { $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL); } 
    | FLOAT         { $$ = type_create(TYPE_FLOAT, NULL, NULL, NULL); }
    | VOID          { $$ = type_create(TYPE_VOID, NULL, NULL, NULL); } 
    | INTEGER       { $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL); } 
    | STRING        { $$ = type_create(TYPE_STRING, NULL, NULL, NULL); } 
    | function_type { $$ = $1; }
    | array_type    { $$ = $1; }
    ;

basic_types: AUTO  { $$ = type_create(TYPE_AUTO, NULL, NULL, NULL); } 
    | BOOLEAN      { $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); } 
    | CHAR         { $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL); } 
    | FLOAT        { $$ = type_create(TYPE_FLOAT, NULL, NULL, NULL); }
    | VOID         { $$ = type_create(TYPE_VOID, NULL, NULL, NULL); } 
    | INTEGER      { $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL); } 
    | STRING       { $$ = type_create(TYPE_STRING, NULL, NULL, NULL); } 
    ;

function_type: FUNCTION all_types OPEN_PARAN_TOKEN opt_param_list CLOSE_PARAN_TOKEN { $$ = type_create(TYPE_FUNCTION, $2, $4, NULL); } 
    ;

array_type: ARRAY OPEN_BRACK_TOKEN opt_expr CLOSE_BRACK_TOKEN all_types { $$ = type_create(TYPE_ARRAY, $5, NULL, $3); }
    ;

param: ID_TOKEN COLON_TOKEN all_types { $$ = param_list_create($1, $3, NULL); } 
    ;

param_list: param COMMA_TOKEN param_list  { $1->next = $3; $$ = $1; } 
    |       param                         { $$ = $1; }
    ;

opt_param_list: param_list { $$ = $1; } 
    |                      { $$ = NULL; } 
    ;

basic_init: ASSIGN_TOKEN expr1 { $$ = $2; }
    |                          { $$ = NULL; }
    ;

array_init: ASSIGN_TOKEN array_init_value  { $$ = $2; }
    |                                      { $$ = NULL; }
    ;

decl: ID_TOKEN COLON_TOKEN basic_types basic_init SEMICOLON_TOKEN                                       { $$ = decl_create($1, $3, $4, NULL, NULL); }
    | ID_TOKEN COLON_TOKEN array_type array_init SEMICOLON_TOKEN                                        { $$ = decl_create($1, $3, $4, NULL, NULL); }
    | ID_TOKEN COLON_TOKEN function_type ASSIGN_TOKEN OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN { $$ = decl_create($1, $3, NULL, $6, NULL); }
    | ID_TOKEN COLON_TOKEN function_type SEMICOLON_TOKEN                                                { $$ = decl_create($1, $3, NULL, NULL, NULL); }
    ;

comment: CPPCOMMENT_TOKEN { $$ = NULL; }
    |    CCOMMENT_TOKEN   { $$ = NULL; }
    ;


file_body: decl { $$ = $1; }
    | comment   { $$ = decl_create(NULL, NULL, NULL, NULL, NULL); }
    ;

file_body_list: file_body file_body_list { $1->next = $2; $$ = $1; }
    | file_body                          { $$ = $1; }
    ;


flow_ending_stmt: SEMICOLON_TOKEN { $$ = stmt_create(STMT_SEMICOLON, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
    | stmt                        { $$ = $1; }
    ;

flow_ending_if_dangling: SEMICOLON_TOKEN { $$ = stmt_create(STMT_SEMICOLON, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
    | if_dangling                        { $$ = $1; }
    ;

stmt: FOR OPEN_PARAN_TOKEN opt_expr SEMICOLON_TOKEN opt_expr SEMICOLON_TOKEN opt_expr CLOSE_PARAN_TOKEN flow_ending_stmt { $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL); }
    | expr1 SEMICOLON_TOKEN                                                                                              { $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
    | RETURN opt_expr SEMICOLON_TOKEN                                                                                    { $$ = stmt_create(STMT_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
    | decl                                                                                                               { $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL); }
    | PRINT opt_expr_list SEMICOLON_TOKEN                                                                                { $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
    | OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN                                                                  { $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, $2, NULL, NULL); }
    | IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN stmt                                                                   { $$ = stmt_create(STMT_IF, NULL, NULL, $3, NULL, $5, NULL, NULL); }
    | IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling ELSE stmt                                                  { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL); }
    | WHILE OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN flow_ending_stmt                                                    { $$ = stmt_create(STMT_WHILE, NULL, NULL, $3, NULL, $5, NULL, NULL); }
    ;


if_dangling: IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling ELSE if_dangling                                           { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL); }
    | decl                                                                                                                      { $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL); }
    | expr1 SEMICOLON_TOKEN                                                                                                     { $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
    | RETURN opt_expr SEMICOLON_TOKEN                                                                                           { $$ = stmt_create(STMT_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
    | PRINT opt_expr_list SEMICOLON_TOKEN                                                                                       { $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
    | FOR OPEN_PARAN_TOKEN opt_expr SEMICOLON_TOKEN opt_expr SEMICOLON_TOKEN opt_expr CLOSE_PARAN_TOKEN flow_ending_if_dangling { $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL); }
    | WHILE OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN flow_ending_if_dangling                                                    { $$ = stmt_create(STMT_WHILE, NULL, NULL, $3, NULL, $5, NULL, NULL); } 
    | OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN                                                                         { $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, $2, NULL, NULL); }
    ;


decl_body: comment { $$ = stmt_create(STMT_COMMENT, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
    | stmt         { $$ = $1; }

decl_body_list: decl_body decl_body_list { $1->next = $2; $$ = $1; }
    |                                    { $$ = NULL; }
    ;

%%


int yyerror(const char *msg) {
    fprintf(stderr, "Parser error: %s\n", msg);
    return 1;
}
