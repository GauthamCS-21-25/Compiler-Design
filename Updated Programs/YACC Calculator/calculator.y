%{
#include <stdio.h>
int valid=1;
%}

%token NUMBER
%left '+' '-'
%left '*' '/' '%'
%left '(' ')'

%%
ArithmeticExpression: E { printf("Result: %d\n", $$); return 0;};
E:E'+'E { $$=$1+$3; } | E'-'E { $$=$1-$3; } | E'*'E { $$=$1*$3; } | E'/'E { $$=$1/$3; } | E'%'E { $$=$1%$3; } | '('E')' { $$=$2; } | NUMBER { $$=$1; };
%%

int yyerror(){
	printf("Invalid expression.\n");
	valid=0;
}

int main(){
	printf("Enter the expression: ");
	yyparse();
	if(valid) printf("Valid expression.\n");
}
