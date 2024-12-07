%{
#include <stdio.h>
int lines=0, words=0, characters=0;
%}

%%
\n { lines++; } //Matches a newline character. Increments lines to track the number of lines. Increments characters ince a newline is also a character.
[ \t]+ { characters+=yyleng; } //Matches one or more whitespace characters (spaces or tabs). Increments characters by yyleng, which is the length of the matched pattern.
[a-zA-Z]+ { words++; characters+=yyleng; } //Matches one or more alphabetic characters. Increments words, as each match represents a word. Increments characters by yyleng/
. { } //Matches any single character that does not match any of the above pattern.
%%

int yywrap(void){ return 1;}

int main(){
	yylex();
	printf("No. of lines: %d\n", lines);
	printf("No. of words: %d\n", words);
	printf("No. of characters: %d\n", characters);
	return 0;
}
