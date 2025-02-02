/*
S -> (L) | a
L -> L,S | S
input string: (a,(a,a))
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10
#define MAX_LENGTH 20

int no_prod;
char production[MAX_SIZE][MAX_LENGTH];
char input[MAX_LENGTH], stack[MAX_LENGTH];
int ptr=0, top=-1;

void main(){
	printf("Enter the total no. of productions: ");
	scanf("%d", &no_prod);
	printf("Enter the productions: \n");
	for(int i=0; i<no_prod; i++)
		scanf("%s", production[i]);
		
	printf("Enter the input string: ");
	scanf("%s", input);
	
	printf("\nStack\t\tInput\t\t\tAction\n");
	
	int shift, reduce;
	do {
		shift=0; reduce=1;
		if(stack[0]==production[0][0] && strlen(stack)==1 && input[strlen(input)-1]==' '){
			printf("String accepted.\n");
			exit(0);
		}
		int i;
		for(i=0; i<no_prod; i++){
			reduce=1;
			for(int j=top, k=strlen(production[i])-1; j>=0, k>=2; j--, k--)
				if(stack[j]!=production[i][k])
					reduce=0;
			if(reduce==1)
				break;
		}
		if(reduce==1){
			int j, k;
			for(j=top, k=strlen(production[i])-1; j>=0, k>=2; j--, k--);
			stack[j+1]=production[i][0];
			stack[j+2]='\0';
			top -= strlen(production[i])-2-1;
			printf("%s\t\t%s\t\t%s\n", stack, input, production[i]);
		} else {
			if(input[ptr]!='\0'){
				shift=1;
				top++;
				stack[top]=input[ptr];
				stack[top+1]='\0';
				input[ptr++]=' ';
				printf("%s\t\t%s\t\t%s\n", stack, input, "Shift");
			}
		}
	} while(shift==1 || reduce==1);
	printf("String rejected.\n");
}
