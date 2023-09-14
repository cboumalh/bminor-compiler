%{
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "encode.h"
#include "utils.h"
#include <math.h>
#define MAXIDSZ 256

int processIdMatch(const char* text) {
  char keywords[][20] = {"array","auto","boolean","char","else","false","float","for","function","if","integer","print","return","string","true","void","while"};
  int i = 0;
  for(i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++){
    if(strcmp(keywords[i], text) == 0){
      return KEYWORD_TOKEN;
    }
  }

  return ID_TOKEN;
}

char strtochar(const char* str){
  char codes[NUMBER_CODES] = {'a', 'b', 'e', 'n', 'r', 't', 'f', 'v', '\\', '\'', '\"'};
  char ascii_chars[NUMBER_CODES] = {'\a', '\b', '\e', '\n', '\r', '\t', '\f', '\v', '\\', '\'', '\"'};
  int length = strlen(str);

  if(length == 3 && char_in_list(str[1], ascii_chars, NUMBER_CODES) == -1) return str[1];
  else if (length == 4) return ascii_chars[char_in_list(str[2], codes, NUMBER_CODES)];
  else if(length == 7){
    char hex[3];
    hex[0] = str[4];
    hex[1] = str[5];
    hex[2] = '\0';
    return (char)strtol(hex, NULL, 16);
  }
  else{
    return '\0';
  }
}

%}

IdOrKeyword   [a-zA-Z_][a-zA-Z0-9]*
Ccomment      \/\*([^\*]|([\*][^\/]))*\*+\/ 
CppComment    \/\/.*[^\n\r]|\/\/
validstr      ["]([^"\\\n]|\\.|\\\n)*["]
basicfloat    [-\+]?[0-9]*\.{validint}
scifloat      [-\+]?[0-9]*(\.{validint})?(E{validint}|e{validint})
validint      [-\+]?[0-9]+
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
{eatmeup} {
  return NOTHING_TOKEN;
}

{Ccomment} {
  printf("%s\n", TokenNames[CCOMMENT_TOKEN]);
  return CCOMMENT_TOKEN;
}

{CppComment} {
  printf("%s\n", TokenNames[CPPCOMMENT_TOKEN]);
  return CPPCOMMENT_TOKEN;
}

{IdOrKeyword} {
  if(strlen(yytext) > MAXIDSZ){
    printf("%s: %s is not a valid token\n", TokenNames[ERROR_TOKEN], yytext);
    return ERROR_TOKEN;
  }
  int token = processIdMatch(yytext);
  if(token == ID_TOKEN)
    printf("%s %s\n", TokenNames[token], yytext);
  else{
    char output[50];
    toUpperCase(yytext, output);
    printf("%s\n", output);
  }

  return token;
}

{or} {
  printf("%s\n", TokenNames[OR_TOKEN]);
  return OR_TOKEN;
}

{and} {
  printf("%s\n", TokenNames[AND_TOKEN]);
  return AND_TOKEN;
}

{not} {
  printf("%s\n", TokenNames[NOT_TOKEN]);
  return NOT_TOKEN;
}

{multiply} {
  printf("%s\n", TokenNames[MULTIPLY_TOKEN]);
  return MULTIPLY_TOKEN;
}

{modulo} {
  printf("%s\n", TokenNames[MODULO_TOKEN]);
  return MODULO_TOKEN;
}

{colon} {
  printf("%s\n", TokenNames[COLON_TOKEN]);
  return COLON_TOKEN;
}

{openParan} {
  printf("%s\n", TokenNames[OPEN_PARAN_TOKEN]);
  return OPEN_PARAN_TOKEN;
}

{closeParan} {
  printf("%s\n", TokenNames[CLOSE_PARAN_TOKEN]);
  return CLOSE_PARAN_TOKEN;
}

{not_equal} {
  printf("%s\n", TokenNames[NOT_EQUAL_TOKEN]);
  return NOT_EQUAL_TOKEN;
}

{division} {
  printf("%s\n", TokenNames[DIVISION_TOKEN]);
  return DIVISION_TOKEN;
}

{increment} {
  printf("%s\n", TokenNames[INCREMENT_TOKEN]);
  return INCREMENT_TOKEN;
}

{decrement} {
  printf("%s\n", TokenNames[DECREMENT_TOKEN]);
  return DECREMENT_TOKEN;
}

{assign} {
  printf("%s\n", TokenNames[ASSIGN_TOKEN]);
  return ASSIGN_TOKEN;
}

{comma} {
  printf("%s\n", TokenNames[COMMA_TOKEN]);
  return COMMA_TOKEN;
}

{subtraction} {
  printf("%s\n", TokenNames[SUBTRACTION_TOKEN]);
  return SUBTRACTION_TOKEN;
}

{addition} {
  printf("%s\n", TokenNames[ADDITION_TOKEN]);
  return ADDITION_TOKEN;
}

{power} {
  printf("%s\n", TokenNames[POWER_TOKEN]);
  return POWER_TOKEN;
}

{assertEq} {
  printf("%s\n", TokenNames[ASSERT_EQ_TOKEN]);
  return ASSERT_EQ_TOKEN;
}

{less} {
  printf("%s\n", TokenNames[LESS_TOKEN]);
  return LESS_TOKEN;
}

{lessOrEq} {
  printf("%s\n", TokenNames[LESS_OR_EQ_TOKEN]);
  return LESS_OR_EQ_TOKEN;
}

{greater} {
  printf("%s\n", TokenNames[GREATER_TOKEN]);
  return GREATER_TOKEN;
}

{semicolon} {
  printf("%s\n", TokenNames[SEMICOLON_TOKEN]);
  return SEMICOLON_TOKEN;
}

{validstr} {
  char *output_encoded_str = (char *)malloc(BUFSIZ * sizeof(char));
  if(string_decode(yytext, output_encoded_str)){
    printf("%s %s\n", TokenNames[STRING_TOKEN], output_encoded_str);
    free(output_encoded_str);
    return STRING_TOKEN;
  }

  free(output_encoded_str);
  printf("%s: %s is not a valid token\n", TokenNames[ERROR_TOKEN], yytext);
  return ERROR_TOKEN;
}

{basicfloat}|{scifloat} {
  printf("%s %f\n", TokenNames[FLOAT_TOKEN], atof(yytext));
  return FLOAT_TOKEN;
}

{validint} {
  printf("%s %d\n", TokenNames[INT_TOKEN], atoi(yytext));
  return INT_TOKEN;
}

{greaterOrEq} {
  printf("%s\n", TokenNames[GREATER_OR_EQ_TOKEN]);
  return GREATER_OR_EQ_TOKEN;
}

{openBrack} {
  printf("%s\n", TokenNames[OPEN_BRACK_TOKEN]);
  return OPEN_BRACK_TOKEN;
}

{closeBrack} {
  printf("%s\n", TokenNames[CLOSE_BRACK_TOKEN]);
  return CLOSE_BRACK_TOKEN;
}

{openCurly} {
  printf("%s\n", TokenNames[OPEN_CURLY_TOKEN]);
  return OPEN_CURLY_TOKEN;
}

{closeCurly} {
  printf("%s\n", TokenNames[CLOSE_CURLY_TOKEN]);
  return CLOSE_CURLY_TOKEN;
}

{validchar} {
  char result = strtochar(yytext);
  if(result != '\0'){
    printf("%s %c\n", TokenNames[CHAR_TOKEN], result);
    return CHAR_TOKEN;
  }
  printf("%s: %s is not a valid token\n", TokenNames[ERROR_TOKEN], yytext);
  return ERROR_TOKEN;
}

{error} {
  printf("%s: %s is not a valid token\n", TokenNames[ERROR_TOKEN], yytext);
  return ERROR_TOKEN;
}
%%

int yywrap(void)
{
  return 1;
}
