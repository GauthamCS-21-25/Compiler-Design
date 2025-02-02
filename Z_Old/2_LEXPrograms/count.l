%{
#include <stdio.h>

int lines = 0, words = 0, characters = 0;
%}

%%
[$]		{ return 0; }
\n              { lines++; characters++; }
[ \t]+          { characters += yyleng; }
[a-zA-Z]+             { words++; characters += yyleng; }
.               {  }
%%

int yywrap(void) {
    return 1;
}

int main() {
    yylex();
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", characters);
    return 0;
}

