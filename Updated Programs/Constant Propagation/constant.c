#include <stdio.h>
#include <string.h>

int value[26]; //To store values to variables

void compute(char result, int arg1, char operator, int arg2){
	int valid=1; //To check whether operator is valid
	switch(operator){
		case '+':
			value[result-97]=arg1+arg2;
			break;
		case '-':
			value[result-97]=arg1-arg2;
			break;
		case '*':
			value[result-97]=arg1*arg2;
			break;
		case '/':
			value[result-97]=arg1/arg2;
			break;
		default:
			printf("Invalid operator.\n");
			valid=0;
	}
	if(valid) printf("Constant Propagated: %c=%d\n", result, value[result-97]);
}

void main(){
	int lines;
	
	char instruction[10];
	char result, arg1, operator, arg2;
	int iarg1, iarg2; //To store integer argument
	
	printf("Enter the total number of lines in the program: ");
	scanf("%d", &lines);
	getchar();//Consume the leftover newline character otherwise fgets will take it as the first instruction
	
	printf("Enter the program: \n\n");
	
	for(int i=0; i<lines; i++){
		printf(">");
		
		fgets(instruction, 10, stdin);
		instruction[strlen(instruction)-1]='\0';
		//Check whether instruction is like a=1+2
		if(sscanf(instruction, "%c=%d%c%d", &result, &iarg1, &operator, &iarg2)==4)
			compute(result, iarg1, operator, iarg2);
		//Check whether instruction is like a=1+b
		else if(sscanf(instruction, "%c=%d%c%c", &result, &iarg1, &operator, &arg2)==4)
			compute(result, iarg1, operator, value[arg2-97]);
		//Check whether instruction is like a=b+1
		else if(sscanf(instruction, "%c=%c%c%d", &result, &arg1, &operator, &iarg2)==4)
			compute(result, value[arg1-97], operator, iarg2);
		//Check whether instruction is like a=b+c
		else if(sscanf(instruction, "%c=%c%c%c", &result, &arg1, &operator, &arg2)==4)
			compute(result, value[arg1-97], operator, value[arg2-97]);
		//Check whether instruction is like a=1
		else if(sscanf(instruction, "%c=%d", &result, &iarg1)==2){ value[result-97]=iarg1; printf("Value stored: %c=%d\n", result, value[result-97]); }
		//Check whether instruction is like a=b
		else if(sscanf(instruction, "%c=%c", &result, &arg1)==2){ value[result-97]=value[arg1-97]; printf("Constant Propagated: %c=%d\n", result, value[result-97]); }
		//All possible isntruction format has been checked, if not one of te above, then invalid
		else printf("Invalid instruction.\n");
	} 
}
