%{
#include <stdio.h>
int vowels=0, consonants=0;
%}

%%
[aeiouAEIOU]+ { vowels+=yyleng; }
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z]+ { consonants+=yyleng; }
. { }
%%

int yywrap(void){ return 1; }

int main(){
	yylex();
	printf("No. of vowels: %d\n", vowels);
	printf("No. of consonants: %d\n", consonants);
	return 0;
}
