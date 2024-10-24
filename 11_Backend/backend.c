#include <stdio.h>
#include <string.h>

void generateAssembly(char op, char result, char arg1, char arg2, char assembly[][100], int *line) {
    switch (op) {
        case '+':
            sprintf(assembly[*line], "MOV AX, [%c]", arg1);
            (*line)++;
            sprintf(assembly[*line], "ADD AX, [%c]", arg2);  
            (*line)++;
            sprintf(assembly[*line], "MOV [%c], AX", result);
            (*line)++;
            break;
        case '-':
            sprintf(assembly[*line], "MOV AX, [%c]", arg1);  
            (*line)++;
            sprintf(assembly[*line], "SUB AX, [%c]", arg2); 
            (*line)++;
            sprintf(assembly[*line], "MOV [%c], AX", result); 
            (*line)++;
            break;
        case '*':
            sprintf(assembly[*line], "MOV AX, [%c]", arg1); 
            (*line)++;
            sprintf(assembly[*line], "MUL AX, [%c]", arg2);  
            (*line)++;
            sprintf(assembly[*line], "MOV [%c], AX", result);
            (*line)++;
            break;
        case '=':
            sprintf(assembly[*line], "MOV AX, [%c]", arg1);
            (*line)++;
            sprintf(assembly[*line], "MOV [%c], AX", result);
            (*line)++;
            break;
        default:
            printf("Invalid operation\n");
            break;
    }
}

void parseAndGenerate(char* tac, char assembly[][100], int *line) {
    char result, arg1, arg2, op;

    if (sscanf(tac, "%c=%c%c%c", &result, &arg1, &op, &arg2) == 4) {
        generateAssembly(op, result, arg1, arg2, assembly, line);
    } else if (sscanf(tac, "%c=%c", &result, &arg1) == 2) {
        generateAssembly('=', result, arg1, '\0', assembly, line);
    } else {
        printf("Invalid TAC format\n");
    }
}

int main() {
    int numInstructions;
    printf("Enter the total number of instructions: ");
    scanf("%d", &numInstructions);
    
    char tac[100];
    char assembly[100][100];
    int line = 0;

    printf("Enter the three-address code instructions:\n");
    getchar();

    for (int i = 0; i < numInstructions; i++) {
        printf("Instruction %d: ", i + 1);
        fgets(tac, 100, stdin);
        tac[strcspn(tac, "\n")] = 0;
        parseAndGenerate(tac, assembly, &line);
    }

    printf("\nGenerated 8086 Assembly Code:\n");
    for (int i = 0; i < line; i++) {
        printf("%s\n", assembly[i]);
    }

    return 0;
}

