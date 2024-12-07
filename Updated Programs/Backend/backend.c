#include <stdio.h>
#include <string.h>

void generateAssembly(char result, char arg1, char operator, char arg2){
	switch(operator){
		case '+':
			printf("MOV AX, [%c]\n", arg1);
			printf("ADD AX, [%c]\n", arg2);
			printf("MOV [%c], AX\n", result);
			break;
		case '-':
			printf("MOV AX, [%c]\n", arg1);
			printf("SUB AX, [%c]\n", arg2);
			printf("MOV [%c], AX\n", result);
			break;
		case '*':
			printf("MOV AX, [%c]\n", arg1);
			printf("MUL AX, [%c]\n", arg2);
			printf("MOV [%c], AX\n", result);
			break;
		case '/':
			printf("MOV AX, [%c]\n", arg1);
			printf("DIV AX, [%c]\n", arg2);
			printf("MOV [%c], AX\n", result);
			break;
		case '=':
			printf("MOV AX, [%c]\n", arg1);
			printf("MOV [%c], AX\n", result);
			break;
		case '>':
			printf("JUMP [%c]\n", result);
			break;
		default:
			printf("Invalid operator.\n");
			break;
	}
	printf("\n");
}

void parseAndGenerate(char* tac){
	char result, arg1, operator, arg2;
	
	// Check whether tac is in result = arg1 operator arg2 format. Example: a=b+c
	if(sscanf(tac, "%c=%c%c%c", &result, &arg1, &operator, &arg2)==4)
		generateAssembly(result, arg1, operator, arg2);
	// Check whether tac is in result = arg1 format. Example: a=b
	else if(sscanf(tac, "%c=%c", &result, &arg1)==2)
		generateAssembly(result, arg1, '=', '\0');
	// Check whether tac is goto statement. Example goto L
	else if(sscanf(tac, "goto %c", &result)==1)
		generateAssembly(result, '\0', '>', '\0');
	else
		printf("Invalid Three Address Code.\n\n");
}

void main(){
	char tac[10]; //three address code
	
	printf("Enter the three address code (to exit, type 'exit')\n\n");
	
	while(1){
		printf("> ");
		// Read the three address code
		fgets(tac, 10, stdin);
		tac[strlen(tac)-1]='\0';
		
		//Check whether tac is "exit". If yes, break the loop 
		if(strcmp(tac, "exit")==0)
			break;
			
		parseAndGenerate(tac);
	}
}
