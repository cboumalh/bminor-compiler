%{
#include <stdio.h>
#include <string.h>
#include "token.h"

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

%}

IdOrKeyword [a-zA-Z_][a-zA-Z0-9]*
Ccomment      \/\*([^\*]|([\*][^\/]))*\*+\/ 
CppComment    \/\/.*[^\n\r]
validstr      ["]([^"\\\n]|\\.|\\\n)*["]
validint      [-\+]?[0-9]+
validfloat    [-\+]?[0-9]*\.[0-9]+(E[0-9]+|e-[0-9]+)?
eatmeup       [ \t\n\r]+
validchar     \'[ -~]\'
openParan     \(
closeParan    \)
greaterOrEq   >=
greater       >
lessOrEq      <=
less          <
assertEq     ==
power         \^
addition      \+
subtraction   -
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
{eatmeup}      { return NOTHING_TOKEN; }
{Ccomment}     { return CCOMMENT_TOKEN; }
{CppComment}   { return CPPCOMMENT_TOKEN; }
{IdOrKeyword}  { return processIdMatch(yytext); }
{or}           { return OR_TOKEN; }
{and}          { return AND_TOKEN; }
{not}          { return NOT_TOKEN; }
{multiply}     { return MULTIPLY_TOKEN; }
{modulo}       { return MODULO_TOKEN; }
{colon}        { return COLON_TOKEN; }
{openParan}    { return OPEN_PARAN_TOKEN; }
{closeParan}   { return CLOSE_PARAN_TOKEN; }
{not_equal}    { return NOT_EQUAL_TOKEN; }
{division}     { return DIVISION_TOKEN; }
{subtraction}  { return SUBTRACTION_TOKEN; }
{addition}     { return ADDITION_TOKEN; }
{power}        { return POWER_TOKEN; }
{assertEq}     { return ASSERT_EQ_TOKEN; }
{less}         { return LESS_TOKEN; }
{lessOrEq}     { return LESS_OR_EQ_TOKEN; }
{greater}      { return GREATER_TOKEN; }
{semicolon}    { return SEMICOLON_TOKEN; }
{validstr}     { return STRING_TOKEN; }
{validfloat}   { return FLOAT_TOKEN; }
{validint}     { return INT_TOKEN; }
{greaterOrEq}  { return GREATER_OR_EQ_TOKEN; }
{error}        { return ERROR_TOKEN; }
%%

int yywrap(void)
{
  return 1;
}
