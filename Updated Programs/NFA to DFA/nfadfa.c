#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_INPUTS 10
#define MAX_STATES 10
#define MAX_SIZE 10 //maximum length of string to be stored
#define DFA_STATES 2^10

int transition[MAX_INPUTS][MAX_STATES][MAX_STATES];
char nfatable[MAX_STATES][MAX_INPUTS][MAX_SIZE]; //A 2D array of strings, that allows each cell to have strings of atmost MAX_SIZE characters
char dfatable[DFA_STATES][MAX_INPUTS+1][MAX_SIZE]; // DFA will contain atmost 2^n states, where n is the no. of states in NFA. +1 if to store state name.

int no_inputs, no_states;
int idx=0, final_idx=0; //idx where the current index/row of dfa table. final_idx last index/row of dfa table.

char newStr[100]; //to store new states without character repeatition and in sorted. Hence 012, 120, 112200 all these states are considered as 012 

char* sortRemove(char *str){
	strcpy(newStr, "");
	char temp;
	
	//remove repeatition from str and store it to newStr
	for(int i=0; i<strlen(str); i++)
		if(strchr(newStr, str[i])==NULL){
			int len = strlen(newStr);
			newStr[len]=str[i];
			newStr[len+1]='\0';
		}
	
	//sort the chracters of newStr
	int len = strlen(newStr);
	for(int i=0; i<=len-2; i++)
		for(int j=i+1; j<=len-1; j++)
			if(newStr[j]<newStr[i]){
				temp=newStr[i];
				newStr[i]=newStr[j];
				newStr[j]=temp;
			}

	return newStr;
}

void addDFAState(char *currentState){
	int exist = 0; //Check whether state already exists in dfatable
	for(int k=0; k<idx; k++){
		if(strcmp(dfatable[k][0], currentState)==0)
			exist = 1;
	}
	//insert currentState if it is not already present in the DFA table
	if(!exist){
		strcpy(dfatable[final_idx][0], currentState);
		//Process the transition for the currentState's inputs.
		for(int x=0; x<no_inputs; x++){
			//consider current state is 012, concat transition state 0 given x(th) input, 1 given x(th) input, and 2 given x(th) input.
			for(int y=0; y<strlen(dfatable[final_idx][0]); y++)
				strcat(dfatable[final_idx][x+1], nfatable[dfatable[final_idx][0][y]-48][x]);
			//Update the state processed by sortRemove
			strcpy(dfatable[idx][x+1], sortRemove(dfatable[idx][x+1]));
		}
		final_idx++;
	}
}

void main(){

	printf("Enter the total number of inputs: ");
	scanf("%d", &no_inputs);
	printf("Enter the total number of states: ");
	scanf("%d", &no_states);
	
	//Read transition matrix for each inputs. Example transition matrix for the input 'a'.
	for(int i=0; i<no_inputs; i++){
		printf("\nEnter the transition matrix for input%d: \n", i);
		//Read transition matrix for i(th) input. 1 if j(th) state leads to k(th) state through i(th) input.
		for(int j=0; j<no_states; j++)
			for(int k=0; k<no_states; k++)
				scanf("%d", &transition[i][j][k]);
	}
	
	printf("\nTransition matrices are: \n");
	
	for(int i=0; i<no_inputs; i++){
		printf("Transition matrix for input%d: \n", i);
		for(int j=0; j<no_states; j++){
			for(int k=0; k<no_states; k++)
				printf("%d\t", transition[i][j][k]);
			printf("\n");
		}
	}
	
	//Compute the NFA table. Matrix of the format no_states x no_inputs
	for(int i=0; i<no_states; i++){
		for(int j=0; j<no_inputs; j++){
			strcpy(nfatable[i][j], ""); //Reset each cell before processing
			//Check whether there is a transition from i(th) state to k(th) state using j(th) input
			for(int k=0; k<no_states; k++)
				//transition matrix is of the format [input][state][state], here j is input, i is fromState and k is toState. Hence index [j][i][k]
				if(transition[j][i][k]==1){
					//concat index to the nfatable cell if there exist a transition. index (integer) is converted to string, then concated 
					char numStr[3];
					sprintf(numStr, "%d", k);
					strcat(nfatable[i][j], numStr);
				}
		}
	}
	
	printf("\n\nNFA table: \n");
	for(int i=0; i<no_states; i++){
		for(int j=0; j<no_inputs; j++){
			if(strcmp(nfatable[i][j], "")==0) printf("-\t");
			else printf("%s\t", nfatable[i][j]);
		}
		printf("\n");
	}
	
	//Add the first state of the NFA to DFA
	addDFAState(nfatable[0][0]);
	
	//Repeat until there are more undiscovered states. final_idx gets updated only when an undiscovered state is met. idx gets updated in each loop.
	while(++idx==final_idx)
		//Add states of previous row to the DFA table
		for(int x=0; x<no_inputs; x++)
			addDFAState(dfatable[idx-1][x+1]);
	
	printf("\nStates in DFA are: \n");
	for(int i=0; i<final_idx; i++){
		for(int j=0; j<no_inputs+1; j++)
			printf("%s\t", dfatable[i][j]);
		printf("\n");
	}
	
}
