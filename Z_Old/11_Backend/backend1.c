#include <stdio.h>
#include <string.h>

void generateAssembly(char op, char result, char arg1, char arg2) {
    switch (op) {
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
            printf("MOV BX, [%c]\n", arg2);      
            printf("IMUL BX\n");                 
            printf("MOV [%c], AX\n", result);    
            break;
        case '=':
            printf("MOV AX, [%c]\n", arg1);      
            printf("MOV [%c], AX\n", result);    
            break;
        default:
            printf("Invalid operation\n");
            break;
    }
}

void parseAndGenerate(char* tac) {
    char result, arg1, arg2, op;

    if (sscanf(tac, "%c=%c%c%c", &result, &arg1, &op, &arg2) == 4) {
        generateAssembly(op, result, arg1, arg2);
    } else if (sscanf(tac, "%c=%c", &result, &arg1) == 2) {
        generateAssembly('=', result, arg1, '\0');
    } else {
        printf("Invalid TAC format\n");
    }
}

int main() {
    char tac[100];

    printf("Enter three-address code (type 'exit' to stop):\n");

    while (1) {
        printf("> ");
        fgets(tac, 100, stdin);
        tac[strcspn(tac, "\n")] = 0;

        if (strcmp(tac, "exit") == 0)
            break;

        parseAndGenerate(tac);
    }

    return 0;
}

