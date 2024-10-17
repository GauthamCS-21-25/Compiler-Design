#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int values[150];

void main()
{
    int no_of_lines;
    char lines[20][20];

    printf("Enter no. of lines of the program: ");
    scanf("%d", &no_of_lines);
    printf("Enter the program:\n");
    for (int i = 0; i < no_of_lines; i++)
        scanf("%s", lines[i]);

    for (int i = 0; i < no_of_lines; i++)
    {
        int flag = 0;
        for (int j = 2; j < strlen(lines[i]); j++)
        {
            if (isalpha(lines[i][j]))
                flag = 1;
        }

        if (flag == 0)
        {
            int j;
            char temp[20];
            for (j = 2; j < strlen(lines[i]); j++)
                temp[j - 2] = lines[i][j];
            temp[j - 2] = '\0';

            int value = atoi(temp);
            values[lines[i][0]] = value;
        }
        else
        {
            int operand1, operand2;

            if (isalpha(lines[i][2]))
                operand1 = values[lines[i][2]];
            else
            {
                int j;
                char temp[20];
                for (j = 2; isdigit(lines[i][j]); j++)
                    temp[j - 2] = lines[i][j];
                temp[j - 2] = '\0';
                operand1 = atoi(temp);
            }

            int j;
            char operator;
            for (j = 2; isalnum(lines[i][j]); j++)
                ;
            operator= lines[i][j];

            if (isalpha(lines[i][j + 1]))
                operand2 = values[lines[i][j + 1]];
            else
            {
                int k, index = 0;
                char temp[20];
                for (k = j + 1; isdigit(lines[i][k]); k++)
                    temp[index++] = lines[i][k];
                temp[index++] = '\0';
                operand2 = atoi(temp);
            }

            printf("Line %d after constant propagation: %c=%d%c%d\n", i + 1, lines[i][0], operand1, operator, operand2);

            switch (operator)
            {
                case '+':
                    printf("Result: %d\n", operand1 + operand2);
                    values[lines[i][0]] = operand1 + operand2;
                    break;
                case '-':
                    printf("Result: %d\n", operand1 - operand2);
                    values[lines[i][0]] = operand1 - operand2;
                    break;
                case '*':
                    printf("Result: %d\n", operand1 * operand2);
                    values[lines[i][0]] = operand1 * operand2;
                    break;
                case '/':
                    printf("Result: %d\n", operand1 / operand2);
                    values[lines[i][0]] = operand1 / operand2;
                    break;
            }
        }
    }
}
