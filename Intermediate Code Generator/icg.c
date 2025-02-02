#include <stdio.h>
#include <string.h>

void main(){
	char expression[100];
	char var = 'Z'; //temporary variable to store intermediate codes.
	
	printf("Enter the expression: ");
	scanf("%s", expression);
	
	printf("\nProccessing: \n\n");
	
	//Repeat until length of the expression is greater than 3. Acceptable format: variable1=variable2. Example a=E (length = 3).
	while(strlen(expression)>3){
		int flag=0; //Set when an intermediate code with / or * is generated
		
		for(int i=0; i<strlen(expression); i++){
			if(expression[i]=='*' || expression[i]=='/'){
				// Consider the expression a=b*c+d. Operator(/) is at index 3, Operand1(b) is at i-1 and Operand2(c) is at i+1
				printf("%c=%c%c%c\n", var, expression[i-1], expression[i], expression[i+1]);
					
				//Update the expression a=b/c+d as Z=b/c, a=Z+d. i-1 gets updated with temporary variable and the rest of the expression is shifted left by 2 indices.
				expression[i-1]=var--; //Update variable to next variable so as to store new intermediate codes
				for(int j=i; expression[j]!='\0'; j++)
					expression[j]=expression[j+2];
				flag=1; break;
			}
		}
		//flag=1 implies that a intermediate code was generated previously, hence the updated expression needs to checked from start.
		if(flag != 1){
			//Repeat the same logic but update the operrator being checked to + and -
			for(int i=0; i<strlen(expression); i++){
				if(expression[i]=='+' || expression[i]=='-'){
					// Consider the expression a=Z+d. Operator(+) is at index 3, Operand1(Z) is at i-1 and Operand2(d) is at i+1
					printf("%c=%c%c%c\n", var, expression[i-1], expression[i], expression[i+1]);
					
					//Update the expression a=Z+d as Y=Z+d, a=Y. i-1 gets updated with temporary variable and the rest of the expression is shifted left by 2 indices.
					expression[i-1]=var--; //Update variable to next variable so as to store new intermediate codes
					for(int j=i; expression[j]!='\0'; j++)
						expression[j]=expression[j+2];
					break;
				}
			}
		}
	}
	printf("\nIntermediate code: %s\n", expression);
}
