%{
#include <stdio.h>
%}

%%
^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}    { return 1; }
.                                                   { return 0; }
%%

int yywrap(void) {
    return 1;
}

int main() {
    if(yylex())
    	printf("Valid email address.\n");
    else
    	printf("Invalid email address.\n");
    	
    return 0;
}

