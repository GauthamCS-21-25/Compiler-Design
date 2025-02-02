%{
#include <stdio.h>
int valid=1;
%}

%token letter digit

%%
start: letter s
s: letter s | digit s | ;
%%

int yyerror(){
	printf("Invalid identifier.\n");
	valid=0; return 0;
}

int main(){
	printf("Enter the identifier: ");
	yyparse();
	if(valid) printf("Valid identifier.\n");
	return 0;
}
