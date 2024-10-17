#include <stdio.h>
#include <string.h>

char exp[20];
char var = 'Z';

void main()
{
    printf("Enter the expression: ");
    scanf("%s", exp);

    printf("Intermediate code:\n");

    while (strlen(exp) > 3)
    {
        int flag = 0;

        if (flag != 1)
        {
            for (int i = 0; i < strlen(exp); i++)
            {
                if (exp[i] == '/' || exp[i] == '*')
                {
                    printf("%c=%c%c%c\n", var, exp[i - 1], exp[i], exp[i + 1]);
                    
                    exp[i - 1] = var;
                    var--;

                    for (int j = i; exp[j] != '\0'; j++)
                        exp[j] = exp[j + 2]; 
                    
                    flag = 1;
                    break;
                }
            }
        }

        if (flag != 1)
        {
            for (int i = 0; i < strlen(exp); i++)
            {
                if (exp[i] == '+' || exp[i] == '-')
                {
                    printf("%c=%c%c%c\n", var, exp[i - 1], exp[i], exp[i + 1]);
                    
                    exp[i - 1] = var;
                    var--;

                    for (int j = i; exp[j] != '\0'; j++)
                        exp[j] = exp[j + 2]; 
                    
                    flag = 1;
                    break;
                }
            }
        }
    }

    printf("%s\n", exp);
}
