%{
#include <stdio.h>
%}

%%
^[a-zA-Z0-9]+@[a-zA-Z0-9]+\.[a-z]{2,} { return 1;}
. { return 0; }
%%

int yywrap(void) { return 1; }

int main(){
	if(yylex()) printf("Valid email id.\n");
	else printf("Invalid email id.\n");
}
