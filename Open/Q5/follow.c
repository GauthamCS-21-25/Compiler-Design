#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PRODUCTIONS 20
#define MAX_LEN 50

// Structure to hold a production.
typedef struct {
    char left;          // Left-hand side nonterminal.
    char right[MAX_LEN]; // Right-hand side string.
} Production;

Production productions[MAX_PRODUCTIONS];
int prodCount;

// We assume nonterminals are uppercase letters A-Z.
// For each nonterminal, we will store FIRST and FOLLOW sets as a boolean array indexed by ASCII code.
bool firstSet[26][128];   // firstSet[i][ch] is true if character ch is in FIRST(nonterminal 'A'+i)
bool followSet[26][128];  // followSet[i][ch] is true if character ch is in FOLLOW(nonterminal 'A'+i)

// A global flag used to detect changes during iterative set computations.
bool changed;

// Helper: returns true if character c is a nonterminal (uppercase letter)
bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Helper: add symbol 'sym' to a set (represented as a bool array of size 128)
// If the symbol was not already in the set, mark it and set 'changed' to true.
void addToSet(bool set[128], char sym) {
    if (!set[(int)sym]) {
        set[(int)sym] = true;
        changed = true;
    }
}

// --------------------------
// Compute FIRST sets
// --------------------------
//
// The algorithm processes every production A -> α. For each production:
//   - For each symbol X in α (from left to right):
//       • If X is a terminal, add it to FIRST(A) and stop.
//       • If X is a nonterminal, add all symbols in FIRST(X) except epsilon (represented by '#')
//         to FIRST(A). If FIRST(X) contains epsilon then continue with the next symbol;
//         otherwise, stop.
//   - If all symbols of α can derive epsilon then add epsilon to FIRST(A).
//
void computeFirst() {
    changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < prodCount; i++) {
            char A = productions[i].left;
            int indexA = A - 'A';
            int len = strlen(productions[i].right);
            bool epsilonInAll = true;  // assume production derives epsilon until proven otherwise
            for (int pos = 0; pos < len; pos++) {
                char symbol = productions[i].right[pos];
                if (symbol == ' ') continue; // skip spaces if any
                if (!isNonTerminal(symbol)) {
                    // symbol is a terminal; add it and break.
                    addToSet(firstSet[indexA], symbol);
                    epsilonInAll = false;
                    break;
                } else {
                    int indexSym = symbol - 'A';
                    // Add everything from FIRST(symbol) except epsilon.
                    for (int ch = 0; ch < 128; ch++) {
                        if (ch == '#' ) continue;
                        if (firstSet[indexSym][ch])
                            addToSet(firstSet[indexA], (char)ch);
                    }
                    // If epsilon is in FIRST(symbol), then continue with the next symbol.
                    if (firstSet[indexSym]['#'])
                        epsilonInAll = true;
                    else {
                        epsilonInAll = false;
                        break;
                    }
                }
            }
            // If every symbol in the right-hand side can derive epsilon, add epsilon to FIRST(A)
            if (epsilonInAll)
                addToSet(firstSet[indexA], '#');
        }
    }
}

// --------------------------
// Compute FOLLOW sets
// --------------------------
//
// The FOLLOW set for a nonterminal B is computed using these rules:
//   1. Place '$' in FOLLOW(S), where S is the start symbol.
//   2. For every production A -> αBβ, add FIRST(β) (except epsilon) to FOLLOW(B).
//   3. For every production A -> αBβ where FIRST(β) contains epsilon, or for every production A -> αB,
//      add FOLLOW(A) to FOLLOW(B).
//
void computeFollow() {
    // Assume the start symbol is the left-hand side of the first production.
    int startIndex = productions[0].left - 'A';
    followSet[startIndex]['$'] = true; // '$' denotes end-of-input.

    changed = true;
    while (changed) {
        changed = false;
        // For each production A -> α, process every symbol in α.
        for (int i = 0; i < prodCount; i++) {
            char A = productions[i].left;
            int indexA = A - 'A';
            int len = strlen(productions[i].right);
            for (int pos = 0; pos < len; pos++) {
                char B = productions[i].right[pos];
                if (!isNonTerminal(B))
                    continue;  // only nonterminals get FOLLOW sets.
                int indexB = B - 'A';
                // Compute FIRST(beta) where beta is the substring to the right of B.
                bool epsilonBeta = true;
                for (int j = pos + 1; j < len; j++) {
                    char betaSym = productions[i].right[j];
                    if (betaSym == ' ') continue;
                    if (!isNonTerminal(betaSym)) {
                        // betaSym is terminal: add it and stop.
                        addToSet(followSet[indexB], betaSym);
                        epsilonBeta = false;
                        break;
                    } else {
                        int indexBeta = betaSym - 'A';
                        // Add FIRST(betaSym) except epsilon.
                        for (int ch = 0; ch < 128; ch++) {
                            if (ch == '#') continue;
                            if (firstSet[indexBeta][ch])
                                addToSet(followSet[indexB], (char)ch);
                        }
                        // If FIRST(betaSym) contains epsilon, then continue to next symbol.
                        if (firstSet[indexBeta]['#'])
                            epsilonBeta = true;
                        else {
                            epsilonBeta = false;
                            break;
                        }
                    }
                }
                // If beta is empty or every symbol in beta can derive epsilon,
                // add FOLLOW(A) to FOLLOW(B).
                if (epsilonBeta) {
                    for (int ch = 0; ch < 128; ch++) {
                        if (followSet[indexA][ch])
                            addToSet(followSet[indexB], (char)ch);
                    }
                }
            }
        }
    }
}

int main(void) {
    printf("Enter number of productions: ");
    if (scanf("%d", &prodCount) != 1 || prodCount > MAX_PRODUCTIONS) {
        fprintf(stderr, "Invalid number of productions.\n");
        return 1;
    }
    getchar(); // consume the newline after the number

    printf("Enter productions (one per line) in the form A->α (use '#' for epsilon):\n");
    for (int i = 0; i < prodCount; i++) {
        char prodStr[MAX_LEN];
        if (fgets(prodStr, MAX_LEN, stdin) == NULL) {
            fprintf(stderr, "Error reading production %d.\n", i + 1);
            return 1;
        }
        prodStr[strcspn(prodStr, "\n")] = '\0'; // remove newline
        // The left-hand side is assumed to be the first character.
        productions[i].left = prodStr[0];
        // Look for "->" in the string.
        char *arrow = strstr(prodStr, "->");
        if (arrow == NULL) {
            fprintf(stderr, "Invalid production format in line %d.\n", i + 1);
            return 1;
        }
        // Copy the right-hand side (after "->") into the production's right string.
        strcpy(productions[i].right, arrow + 2);
    }
    
    // Initialize FIRST and FOLLOW sets to false.
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 128; j++) {
            firstSet[i][j] = false;
            followSet[i][j] = false;
        }
    }
    
    // Compute FIRST sets (needed to compute FOLLOW sets).
    computeFirst();
    // Compute FOLLOW sets.
    computeFollow();
    
    // Print the FOLLOW sets for every nonterminal that appears on the LHS of a production.
    bool printed[26] = { false };
    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < prodCount; i++) {
        char A = productions[i].left;
        int indexA = A - 'A';
        if (!printed[indexA]) {
            printed[indexA] = true;
            printf("FOLLOW(%c) = { ", A);
            bool firstPrinted = false;
            for (int ch = 0; ch < 128; ch++) {
                if (followSet[indexA][ch]) {
                    if (firstPrinted)
                        printf(", ");
                    // Print '$' as is; for any other character print it.
                    if (ch == '$')
                        printf("$");
                    else
                        printf("%c", ch);
                    firstPrinted = true;
                }
            }
            printf(" }\n");
        }
    }
    
    return 0;
}

