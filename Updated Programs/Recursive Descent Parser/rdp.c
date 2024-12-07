/* Grammar used:

	S -> aABe
	A -> bA'
	A' -> bcA' | # (epsilon)
	B -> d	
	
*/

#include <stdio.h>
#include <stdlib.h>

char input[10];
int i=0;

void B(){
	if(input[i++]=='d'){}
	else { printf("%s rejected.\n", input); exit(1); }
}

void A_dash(){
	if(input[i++]=='b'){
		if(input[i++]=='c') A_dash();
		else { printf("%s rejected.\n", input); exit(1); }
	} else i--; //i should be decremented back incase of epsilon transition
}

void A(){
	if(input[i++]=='b') A_dash();
	else { printf("%s rejected.\n", input); exit(1); }
}

void S(){
	if(input[i++]=='a'){
		A();
		B();
		if(input[i++]='e') printf("%s accepted.\n", input);
		else printf("%s rejected.\n", input);
	} else printf("%s rejected.\n", input);
}

void main(){
	
	printf("Enter the string: ");
	scanf("%s", input);
	
	//Call the start symbol
	S();
}
