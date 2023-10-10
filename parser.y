
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

%%
program: file_cmp_list {}
    |

expr1: expr2 ASSIGN_TOKEN expr1 {} 
    |  expr2 {} 

expr2: expr2 OR_TOKEN expr3 {} 
    |  expr3 {} 

expr3: expr3 AND_TOKEN expr4 {} 
    |  expr4 {} 

expr4: expr4 NOT_EQUAL_TOKEN expr5 {} 
    |  expr4 LESS_TOKEN expr5 {} 
    |  expr4 LESS_OR_EQ_TOKEN expr5 {} 
    |  expr4 GREATER_TOKEN expr5 {} 
    |  expr4 GREATER_OR_EQ_TOKEN expr5 {} 
    |  expr4 ASSERT_EQ_TOKEN expr5 {} 
    |  expr5 {} 

expr5: expr5 ADDITION_TOKEN expr6 {} 
    |  expr5 SUBTRACTION_TOKEN expr6 {} 
    |  expr6 {} 

expr6: expr6 MULTIPLY_TOKEN expr7 {} 
    |  expr6 DIVISION_TOKEN expr7 {} 
    |  expr6 MODULO_TOKEN expr7 {} 
    |  expr7 {} 

expr7: expr8 POWER_TOKEN expr7  {}
    |  expr8 {}
    ;

expr8: SUBTRACTION_TOKEN expr8 {} 
    |  ADDITION_TOKEN expr8 {}
    |  NOT_TOKEN expr8 {} 
    |  expr9 {} 
    ;

expr9: expr9 INCREMENT_TOKEN {} 
    |  expr9 DECREMENT_TOKEN {} 
    |  INCREMENT_TOKEN expr9 {} 
    |  DECREMENT_TOKEN expr9 {} 
    |  atomic {}
    ;

atomic: INT_TOKEN {} 
    |   FLOAT_TOKEN {}
    |   ID_TOKEN {} 
    |   STRING_TOKEN {} 
    |   FALSE {} 
    |   TRUE {} 
    |   CHAR_TOKEN {} 
    |   OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN {} 
    |   ID_TOKEN OPEN_PARAN_TOKEN opt_expr_list CLOSE_PARAN_TOKEN {} 
    |   ID_TOKEN OPEN_BRACK_TOKEN expr1 CLOSE_BRACK_TOKEN {} 
    ;

expr_list: expr_list COMMA_TOKEN expr1 {} 
    |      expr1 {} 
    ;

opt_expr_list: expr_list {} 
    |
    ;

opt_expr: expr1 {}
    |
    ;

all_types: AUTO  {} 
    | BOOLEAN  {} 
    | CHAR  {} 
    | FLOAT  {}
    | VOID  {} 
    | INTEGER {} 
    | STRING {} 
    | function_type
    | array_type
    ;

basic_types: AUTO  {} 
    | BOOLEAN  {} 
    | CHAR  {} 
    | FLOAT  {}
    | VOID  {} 
    | INTEGER {} 
    | STRING {} 
    ;

function_type: FUNCTION all_types OPEN_PARAN_TOKEN opt_param_list CLOSE_PARAN_TOKEN {} 
    ;

array_type: ARRAY OPEN_BRACK_TOKEN opt_expr CLOSE_BRACK_TOKEN all_types {}
    ;

param: ID_TOKEN COLON_TOKEN all_types {} 
    ;

param_list: param_list COMMA_TOKEN param  {} 
    |       param  {} 
    ;

opt_param_list: param_list {} 
    |
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

stmt: FOR OPEN_PARAN_TOKEN opt_expr SEMICOLON_TOKEN opt_expr SEMICOLON_TOKEN opt_expr CLOSE_PARAN_TOKEN stmt
    | opt_expr SEMICOLON_TOKEN
    | RETURN opt_expr SEMICOLON_TOKEN
    | decl
    | PRINT opt_expr_list SEMICOLON_TOKEN
    | OPEN_CURLY_TOKEN decl_body_list CLOSE_CURLY_TOKEN
    | IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN stmt
    | IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling ELSE stmt
    | WHILE OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN stmt
    ;


if_dangling: IF OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling ELSE if_dangling
    | decl
    | opt_expr SEMICOLON_TOKEN
    | RETURN opt_expr SEMICOLON_TOKEN
    | PRINT opt_expr_list SEMICOLON_TOKEN
    | FOR OPEN_PARAN_TOKEN opt_expr SEMICOLON_TOKEN opt_expr SEMICOLON_TOKEN opt_expr CLOSE_PARAN_TOKEN if_dangling
    | WHILE OPEN_PARAN_TOKEN expr1 CLOSE_PARAN_TOKEN if_dangling
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
