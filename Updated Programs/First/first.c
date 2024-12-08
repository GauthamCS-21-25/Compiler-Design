#include <stdio.h>
#include <string.h>

#define MAX_SIZE 20
#define MAX_LENGTH 20

int no_prod;
char production[MAX_SIZE][MAX_LENGTH];
char first[26][MAX_LENGTH];

char* getFirst(char nonterminal){
	return first[nonterminal-65];
}

void findFirst(int pidx, int cidx){
	//Check whether the current index is a terminal
	if(!(production[pidx][cidx]>='A' && production[pidx][cidx]<='Z')){
		// update the first of non-terminal by adding the terminal to it's first
		int len=strlen(first[production[pidx][0]-65]);
		first[production[pidx][0]-65][len]=production[pidx][cidx];
		first[production[pidx][0]-65][len+1]='\0';
	} else {
		//get the first of the non-terminal in current index and add it to first of current non-terminal
		strcat(first[production[pidx][0]-65], getFirst(production[pidx][cidx]));
		//check whether the processed first contains epsion production
		while(strchr(getFirst(production[pidx][cidx]), '#')!=NULL){
			//update the current index by 1
			cidx++;
			//check whether the next symbol in current non-terminal is a terminal
			if(!(production[pidx][cidx]>='A' && production[pidx][cidx]<='Z')){
				// update the first of non-terminal by adding the terminal to it's first
				int len=strlen(first[production[pidx][0]-65]);
				first[production[pidx][0]-65][len]=production[pidx][cidx];
				first[production[pidx][0]-65][len+1]='\0';
				//No more recurssion needed. FIRST has been found.
				break;
			} else {
				//get the first of the non-terminal in current index and add it to first of current non-terminal
				strcat(first[production[pidx][0]-65], getFirst(production[pidx][cidx]));
			}
		}	
	}
}

void main(){
	
	printf("Enter the total no. of productions: ");
	scanf("%d", &no_prod);
	
	printf("Enter the productions: \n");
	for(int i=0; i<no_prod; i++)
		scanf("%s", production[i]);
	
	//FIRST is calculated from the last production.
	for(int i=no_prod-1; i>=0; i--)
		findFirst(i, 2); //i(th) production definition starts from 2nd index. Example S=aAB
	
	printf("\n");
	
	for(int i=0; i<26; i++)
		if(strlen(first[i])!=0)
			printf("FIRST(%c) = %s\n", i+65, first[i]);
}
