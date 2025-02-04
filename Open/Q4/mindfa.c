#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 10

// Global arrays to hold DFA information.
int n;                           // number of states
int numSymbols;                  // number of symbols in the alphabet
int dfa[MAX_STATES][MAX_SYMBOLS]; // transition table: dfa[state][symbol] = next state
bool isFinal[MAX_STATES];        // final state marker for each state

// For minimization using the table-filling algorithm,
// we will use a 2D table to mark pairs of states as distinguishable.
// We only use the upper triangular part of the table (i < j).
bool marked[MAX_STATES][MAX_STATES];  // false = equivalent (so far), true = distinguished

// Utility function: get the minimum of two integers.
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Utility function: get the maximum of two integers.
int max(int a, int b) {
    return (a > b) ? a : b;
}

int main(void) {
    int i, j, k;
    
    // Input DFA description.
    printf("Enter the number of states: ");
    if (scanf("%d", &n) != 1 || n > MAX_STATES) {
        fprintf(stderr, "Invalid number of states.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter the number of input symbols: ");
    if (scanf("%d", &numSymbols) != 1 || numSymbols > MAX_SYMBOLS) {
        fprintf(stderr, "Invalid number of input symbols.\n");
        return EXIT_FAILURE;
    }
    
    // Read the transition table.
    // For each state and for each symbol, enter the destination state.
    printf("Enter the transition table:\n");
    printf("For each state (from 0 to %d), enter %d numbers (destination state for each symbol):\n", n-1, numSymbols);
    for (i = 0; i < n; i++) {
        printf("State %d: ", i);
        for (j = 0; j < numSymbols; j++) {
            if (scanf("%d", &dfa[i][j]) != 1) {
                fprintf(stderr, "Error reading transition for state %d, symbol %d.\n", i, j);
                return EXIT_FAILURE;
            }
        }
    }
    
    // Read the set of final states.
    // The user enters the number of final states followed by the state numbers.
    int numFinal;
    printf("Enter the number of final states: ");
    if (scanf("%d", &numFinal) != 1) {
        fprintf(stderr, "Error reading number of final states.\n");
        return EXIT_FAILURE;
    }
    // Initialize final state markers.
    for (i = 0; i < n; i++) {
        isFinal[i] = false;
    }
    printf("Enter the final state numbers separated by space: ");
    for (i = 0; i < numFinal; i++) {
        int fs;
        if (scanf("%d", &fs) != 1 || fs < 0 || fs >= n) {
            fprintf(stderr, "Invalid final state: %d\n", fs);
            return EXIT_FAILURE;
        }
        isFinal[fs] = true;
    }
    
    // -------------------------------
    // Minimization: Table-filling algorithm.
    // -------------------------------
    
    // Step 1. Initialize table: mark all pairs (i, j) where one is final and the other is not.
    for (i = 0; i < n; i++) {
        for (j = i+1; j < n; j++) {
            if (isFinal[i] != isFinal[j]) {
                marked[i][j] = true; // distinguished because one is final and the other is not.
            } else {
                marked[i][j] = false;
            }
        }
    }
    
    // Step 2. Iteratively mark pairs that are distinguishable.
    bool changed = true;
    while (changed) {
        changed = false;
        for (i = 0; i < n; i++) {
            for (j = i+1; j < n; j++) {
                if (!marked[i][j]) { // not yet distinguished
                    // Check for each input symbol if transitions lead to a pair already marked.
                    for (k = 0; k < numSymbols; k++) {
                        int ti = dfa[i][k];
                        int tj = dfa[j][k];
                        // Order the pair (ti, tj) so that ti < tj.
                        int a = min(ti, tj);
                        int b = max(ti, tj);
                        // If the transitions go to distinguishable states then mark (i,j)
                        if (a != b && marked[a][b]) {
                            marked[i][j] = true;
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Step 3. Group equivalent states.
    // Two states i and j (with i < j) are equivalent if they are not marked.
    // We assign each state a representative (the smallest state in its equivalence class).
    int rep[MAX_STATES];  // rep[i] = representative of state i
    for (i = 0; i < n; i++) {
        rep[i] = i; // start by assuming each state is in its own class.
    }
    
    for (i = 0; i < n; i++) {
        for (j = i+1; j < n; j++) {
            if (!marked[i][j]) {
                // States i and j are equivalent; choose the smaller one as representative.
                if (rep[j] > rep[i])
                    rep[j] = rep[i];
                else
                    rep[i] = rep[j];
            }
        }
    }
    
    // For consistency, update every state's representative by following the chain.
    for (i = 0; i < n; i++) {
        while (rep[i] != rep[rep[i]])
            rep[i] = rep[rep[i]];
    }
    
    // Now, count how many unique equivalence classes there are.
    bool used[MAX_STATES] = { false };
    int numClasses = 0;
    for (i = 0; i < n; i++) {
        if (!used[rep[i]]) {
            used[rep[i]] = true;
            numClasses++;
        }
    }
    
    // Create a mapping from the old state's representative to the new state number.
    int newStateMapping[MAX_STATES];
    int newStateCounter = 0;
    for (i = 0; i < n; i++) {
        if (used[i]) { // i is a representative
            newStateMapping[i] = newStateCounter++;
        }
    }
    
    // For each state, the new state number is determined by its representative.
    int stateNew[MAX_STATES];
    for (i = 0; i < n; i++) {
        stateNew[i] = newStateMapping[rep[i]];
    }
    
    // Build the minimized DFA's transition table.
    // There will be numClasses states in the minimized DFA.
    int minDFA[MAX_STATES][MAX_SYMBOLS];
    // Initialize with -1 (error state if any transition is undefined)
    for (i = 0; i < numClasses; i++) {
        for (j = 0; j < numSymbols; j++) {
            minDFA[i][j] = -1;
        }
    }
    
    // For each original state, use its representative to define transitions.
    // (Since all states in one equivalence class behave identically.)
    for (i = 0; i < n; i++) {
        int newState = stateNew[i];
        for (j = 0; j < numSymbols; j++) {
            int target = dfa[i][j];
            int newTarget = stateNew[target];
            minDFA[newState][j] = newTarget;
        }
    }
    
    // Determine the minimized DFA final states.
    bool minFinal[MAX_STATES] = { false };
    for (i = 0; i < n; i++) {
        if (isFinal[i]) {
            minFinal[stateNew[i]] = true;
        }
    }
    
    // The new initial state is the new state of the original initial state (assumed state 0).
    int newInitial = stateNew[0];
    
    // -------------------------------
    // Output the minimized DFA.
    // -------------------------------
    
    printf("\nMinimized DFA:\n");
    printf("Number of states: %d\n", numClasses);
    printf("Initial state: %d\n", newInitial);
    printf("Final states: ");
    for (i = 0; i < numClasses; i++) {
        if (minFinal[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    
    printf("Transition Table:\n");
    for (i = 0; i < numClasses; i++) {
        printf("State %d: ", i);
        for (j = 0; j < numSymbols; j++) {
            printf("%d ", minDFA[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}

