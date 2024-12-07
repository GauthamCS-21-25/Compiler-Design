#include <stdio.h>
#include <string.h>

#define SIZE 1024

//strchr return NULL if search character is not present, otherwise returns the index of first appearance

int isOperator(char ch){
	if(strchr("+-*/=", ch)!=NULL){ printf("%c - Operator\n", ch); return 1; }
	else return 0;
}

int isDelimiter(char ch){
	if(strchr("(){},;", ch)!=NULL){ printf("%c - Delimiter\n", ch); return 1; }
	else return 0;
}

int isLiteral(char *token){
	for(int i=0; i<strlen(token); i++)
		if(token[i]>='0' && token[i]<='9'){}
		else return 0;
	printf("%s - Literal\n", token);
	return 1;
}

int isKeyword(char *token){
	if(strcmp(token, "int")==0 || strcmp(token, "float")==0 || strcmp(token, "char")==0){ printf("%s - Keyword\n", token); return 1;}
	else return 0;
}

void main(){
	char line[SIZE], token[SIZE];
	strcpy(token, "");
	FILE *file=fopen("input.txt", "r");
	
	while(fgets(line, sizeof(line), file)){
		line[strlen(line)-1]='\0'; //Remove the newline character 
	
		//Check whether the current line is a single line comment, if so do nothing, continue to next line
		if(line[0]=='/' && line[1]=='/') continue;
		
		//Check whether the current line is a start of multi-line comment
		if(line[0]=='/' && line[1]=='*'){
			//Take the following lines and check whether multi-line comment has ended
			while(fgets(line, sizeof(line), file)){
				if(line[strlen(line)-3]=='*' && line[strlen(line)-2]=='/') break;
			} continue;
		}
		
		printf("%s\n", line);
		for(int i=0; i<strlen(line); i++){
			//Check whether the current character is a operator, or a delimeter, or a white space
			if(isOperator(line[i]) || isDelimiter(line[i]) || line[i]==' '){
				//Check whether the there are any token to be classified
				if(strcmp(token, "")!=0){
					//Classify the token into keyword or literal or identifier
					if(!isKeyword(token))
						if(!isLiteral(token))
							printf("%s - Identifier\n", token);
					strcpy(token, "");
				}
			}
			//Add the character to token if not classified
			else if(line[i]!=' ')
				strncat(token, &line[i], 1);
		}
		printf("\n");
	}
}
