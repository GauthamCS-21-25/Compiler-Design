%{
#include <stdio.h>

int vowels = 0, consonants = 0;

%}

%%
[aeiouAEIOU]    { vowels++; }
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z]  { consonants++; }
.              { }
\n		{return 0; }
%%

int yywrap(void) {
    return 1;
}

int main() {
    yylex();
    printf("Total number of vowels: %d\n", vowels);
    printf("Total number of consonants: %d\n", consonants);
    return 0;
}

