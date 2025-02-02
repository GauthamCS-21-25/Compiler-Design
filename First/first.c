#include <stdio.h>
#include <string.h>

#define MAX_SIZE 20
#define MAX_LENGTH 20

int no_prod;
char production[MAX_SIZE][MAX_LENGTH];
char first[26][MAX_LENGTH];

char* getFirst(char nonterminal) {
    return first[nonterminal - 65];
}

void addToFirst(char nonterminal, char symbol) {
    char *firstSet = first[nonterminal - 65];

    // Avoid duplicate symbols
    if (strchr(firstSet, symbol) == NULL) {
        int len = strlen(firstSet);
        firstSet[len] = symbol;
        firstSet[len + 1] = '\0';
    }
}

void mergeFirstSets(char nonterminal, char *source) {
    for (int i = 0; source[i] != '\0'; i++) {
        addToFirst(nonterminal, source[i]);
    }
}

void findFirst(int pidx, int cidx) {
    // Check if the current symbol is a terminal or epsilon
    if (!(production[pidx][cidx] >= 'A' && production[pidx][cidx] <= 'Z')) {
        addToFirst(production[pidx][0], production[pidx][cidx]);
    } else {
        // Merge the FIRST set of the non-terminal at the current index
        mergeFirstSets(production[pidx][0], getFirst(production[pidx][cidx]));

        // If epsilon `#` is in the FIRST set, process the next symbol
        while (strchr(getFirst(production[pidx][cidx]), '#') != NULL) {
            cidx++; // Move to next symbol

            // If the next symbol is a terminal, add it to FIRST and stop
            if (!(production[pidx][cidx] >= 'A' && production[pidx][cidx] <= 'Z')) {
                addToFirst(production[pidx][0], production[pidx][cidx]);
                break;
            } else {
                mergeFirstSets(production[pidx][0], getFirst(production[pidx][cidx]));
            }
        }
    }
}

void printFirstSets() {
    for (int i = 0; i < 26; i++) {
        if (strlen(first[i]) != 0) {
            printf("FIRST(%c) = {", i + 65);
            int len = strlen(first[i]);
            for (int j = 0; j < len; j++) {
                printf("%c", first[i][j]);
                if (j < len - 1) printf(", ");
            }
            printf("}\n");
        }
    }
}

int main() {
    printf("Enter the total number of productions: ");
    scanf("%d", &no_prod);

    printf("Enter the productions: \n");
    for (int i = 0; i < no_prod; i++)
        scanf("%s", production[i]);

    // Compute FIRST sets from last production to first
    for (int i = no_prod - 1; i >= 0; i--)
        findFirst(i, 2);

    printf("\n");
    printFirstSets();

    return 0;
}
