#include <stdio.h>
#include <string.h>

#define SIZE 1024

int isOperator(char t){
	if(t=='+' || t=='-' || t=='*' || t=='/' || t=='='){
		printf(" %c - Operator\n", t);
		return 1;
	}
	return 0;
}

int isDelimiter(char t){
	if(t=='(' || t==')' || t=='{' || t=='}' || t==',' || t==';'){
		printf(" %c - Delimiter\n", t);
		return 1;
	}
	return 0;
}

int isLiteral(char t[]){
	int flag=0;
	for(int i=0; i<strlen(t); i++){
		if(!(t[i]>='0' && t[i]<='9'))
			flag=1;
	}
	if(!flag){
		printf("%s - Literal\n", t);
		return 1;
	}
	return 0;
}

int isKeyword(char t[]){
	if(strcmp(t, "int")==0 || strcmp(t, "float")==0 || strcmp(t, "char")==0){
		printf("%s - Keyword\n", t);
		return 1;
	}
	return 0;
}

void main() {
	
	char line[SIZE];
	char token[SIZE];
	strcpy(token, "");
	
	FILE *input = fopen("input.txt", "r");
	
	while(fgets(line, sizeof(line), input)){
		
		int flag = 0;
		
		for(int i=0; i<strlen(line); i++){
			if(line[i]=='/' && line[i+1]=='/'){
				flag = 1;
				break;
			}
		}
		
		if(!flag){
			for(int i=0; i<strlen(line); i++){
				if(line[i]=='/' && line[i+1]=='*'){
					while(fgets(line, sizeof(line), input)){
						for(int j=0; j<strlen(line); j++){
							if(line[j]=='*' && line[j+1]=='/'){
								flag = 1;
								break;
							}
							
						}
						if(flag)
							break;
					}
				}
			}
		}
		
		if(!flag){
			line[strlen(line)-1]='\0';
			printf("%s\n", line);
			for(int i=0; i<strlen(line); i++){
				if(isOperator(line[i]) || isDelimiter(line[i]) || line[i]==' '){
					if(strcmp(token, "")!=0){
						if(!isKeyword(token))
							if(!isLiteral(token))
							printf("%s - Identifier\n", token);
						strcpy(token, "");
					}
				}
				else if(line[i]!=' '){
					strncat(token, &line[i], 1);
				}
			}
			printf("\n");
		}
	}
}
