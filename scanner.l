%{
#include <stdio.h>
#include "parser.tab.h"
#include <string.h>
#include "token.h"
#include "encode.h"
#include "utils.h"
#include <math.h>
#define MAXIDSZ 256

int processIdMatch(const char* text) {
  char keywords[][20] = {"array","auto","boolean","char","else","false","float","for","function","if","integer","print","return","string","true","void","while"};
  int i = 0;
  int tokenTypes[] = {ARRAY, AUTO, BOOLEAN, CHAR, ELSE, FALSE, FLOAT, FOR, FUNCTION, IF, INTEGER, PRINT, RETURN, STRING, TRUE, VOID, WHILE};
  
  for(i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++){
    if(strcmp(keywords[i], text) == 0){
      return tokenTypes[i]; // Return the corresponding token type
    }
  }

  return ID_TOKEN; // Default to ID_TOKEN if no match is found
}


%}

IdOrKeyword   [a-zA-Z_][a-zA-Z0-9_]*
Ccomment      \/\*([^\*]|([\*][^\/]))*\*+\/ 
CppComment    \/\/.*[^\n\r]|\/\/
validstr      ["]([^"\\\n]|\\.|\\\n)*["]
basicfloat    [0-9]*\.{validint}
scifloat      [0-9]*(\.{validint})?(E{validint}|e{validint})
validint      [0-9]+
eatmeup       [ \t\n\r]+
validchar     (\')(\\)0x[2-7][A-Fa-f0-9](\')|\'[\x20-\x7E]\'|\'(\\)a\'|\'(\\)b\'|\'(\\)e\'|\'(\\)f\'|\'(\\)n\'|\'(\\)r\'|\'(\\)t\'|\'(\\)v\'|\'(\\)(\\)\'|\'(\\)(\')\'|\'(\\)(\")\'
openParan     \(
closeParan    \)
openBrack     \[
closeBrack    \]
openCurly     \{
closeCurly    \}
greaterOrEq   >=
greater       >
lessOrEq      <=
less          <
assertEq      ==
assign        =
power         \^
addition      \+
increment     \+\+
subtraction   -
decrement     --
comma         ,
division      \/
not_equal     !=
or            [|][|]
and           &&
not           !
multiply      \*
modulo        %
colon         :
semicolon     ;
error         .

%%
{eatmeup} ;

{Ccomment} {
  return CCOMMENT_TOKEN;
}

{CppComment} {
  return CPPCOMMENT_TOKEN;
}

{IdOrKeyword} {
  if(strlen(yytext) > MAXIDSZ){
    return ERROR_TOKEN;
  }
  int token = processIdMatch(yytext);

  yylval.c = strdup(yytext);
 
  return token;
}

{or} {
  return OR_TOKEN;
}

{and} {
  return AND_TOKEN;
}

{not} {
  return NOT_TOKEN;
}

{multiply} {
  return MULTIPLY_TOKEN;
}

{modulo} {
  return MODULO_TOKEN;
}

{colon} {
  return COLON_TOKEN;
}

{openParan} {
  return OPEN_PARAN_TOKEN;
}

{closeParan} {
  return CLOSE_PARAN_TOKEN;
}

{not_equal} {
  return NOT_EQUAL_TOKEN;
}

{division} {
  return DIVISION_TOKEN;
}

{increment} {
  return INCREMENT_TOKEN;
}

{decrement} {
  return DECREMENT_TOKEN;
}

{assign} {
  return ASSIGN_TOKEN;
}

{comma} {
  return COMMA_TOKEN;
}

{subtraction} {
  return SUBTRACTION_TOKEN;
}

{addition} {
  return ADDITION_TOKEN;
}

{power} {
  return POWER_TOKEN;
}

{assertEq} {
  return ASSERT_EQ_TOKEN;
}

{less} {
  return LESS_TOKEN;
}

{lessOrEq} {
  return LESS_OR_EQ_TOKEN;
}

{greater} {
  return GREATER_TOKEN;
}

{semicolon} {
  return SEMICOLON_TOKEN;
}

{validstr} {
  char *output_encoded_str = (char *)malloc(BUFSIZ * sizeof(char));
  if(string_decode(yytext, output_encoded_str)){
    free(output_encoded_str);
    return STRING_TOKEN;
  }

  free(output_encoded_str);
  return ERROR_TOKEN;
}

{basicfloat}|{scifloat} {
  return FLOAT_TOKEN;
}

{validint} {
  return INT_TOKEN;
}

{greaterOrEq} {
  return GREATER_OR_EQ_TOKEN;
}

{openBrack} {
  return OPEN_BRACK_TOKEN;
}

{closeBrack} {
  return CLOSE_BRACK_TOKEN;
}

{openCurly} {
  return OPEN_CURLY_TOKEN;
}

{closeCurly} {
  return CLOSE_CURLY_TOKEN;
}

{validchar} {
  char result = strtochar(yytext);
  if(result != '\0'){
    return CHAR_TOKEN;
  }
  return ERROR_TOKEN;
}

{error} {
  printf("Scan Failed\n");
  return ERROR_TOKEN;
}
%%

int yywrap(void)
{
  return 1;
}
