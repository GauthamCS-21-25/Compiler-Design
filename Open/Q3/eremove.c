#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STATES 100      // Maximum number of states
#define MAX_SYMBOLS 10      // Maximum number of input symbols (excluding ε)

// Node structure for linked lists (used for both ε–transitions and non-ε transitions)
typedef struct Node {
    int state;
    struct Node *next;
} Node;

// Global arrays for adjacency lists
Node* epsilonAdj[MAX_STATES];  // For ε–transitions
// For non-ε transitions: nonEpsilonAdj[state][symbolIndex] is a pointer to a list of destination states.
Node* nonEpsilonAdj[MAX_STATES][MAX_SYMBOLS];

// To store the input alphabet (non-ε symbols)
char alphabet[MAX_SYMBOLS];
int numSymbols;

// To store final states of the original NFA
int isFinalOriginal[MAX_STATES] = {0};

// To store the computed ε–closure for each state:
// closure[i][j] == 1 if state j is in ε–closure of state i.
int closure[MAX_STATES][MAX_STATES];

// Number of states in the NFA
int n;

// Function prototypes
void addEdge(Node **head, int to);
void computeEpsilonClosure(int state, int visited[]);
void computeAllEpsilonClosures();
int isInSet(int set[], int size, int element);

int main() {
    int m;  // number of transitions
    int numFinal;
    
    // Initialize adjacency lists for non-ε transitions
    for (int i = 0; i < MAX_STATES; i++) {
        epsilonAdj[i] = NULL;
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            nonEpsilonAdj[i][j] = NULL;
        }
    }
    
    // Input number of states
    printf("Enter the number of states in the NFA: ");
    if (scanf("%d", &n) != 1 || n > MAX_STATES) {
        fprintf(stderr, "Invalid number of states.\n");
        return EXIT_FAILURE;
    }
    
    // Input number of transitions
    printf("Enter the number of transitions: ");
    if (scanf("%d", &m) != 1) {
        fprintf(stderr, "Invalid number of transitions.\n");
        return EXIT_FAILURE;
    }
    
    // Input the number of input symbols (excluding epsilon) and then the symbols.
    printf("Enter the number of input symbols (excluding 'e' for ε): ");
    if (scanf("%d", &numSymbols) != 1 || numSymbols > MAX_SYMBOLS) {
        fprintf(stderr, "Invalid number of symbols.\n");
        return EXIT_FAILURE;
    }
    printf("Enter the input symbols separated by space: ");
    for (int i = 0; i < numSymbols; i++) {
        // Read a non-space character.
        scanf(" %c", &alphabet[i]);
    }
    
    // Input transitions.
    // Format: from_state input_symbol to_state
    // The symbol 'e' (or 'E') represents an ε–transition.
    printf("Enter transitions in the format: from_state input_symbol to_state\n");
    for (int i = 0; i < m; i++) {
        int from, to;
        char symbol;
        if (scanf("%d %c %d", &from, &symbol, &to) != 3) {
            fprintf(stderr, "Error reading transition %d.\n", i + 1);
            return EXIT_FAILURE;
        }
        if (symbol == 'e' || symbol == 'E') {
            // Add edge in the ε–transitions list
            addEdge(&epsilonAdj[from], to);
        } else {
            // Find the index for the symbol in the input alphabet.
            int index = -1;
            for (int j = 0; j < numSymbols; j++) {
                if (alphabet[j] == symbol) {
                    index = j;
                    break;
                }
            }
            if (index == -1) {
                fprintf(stderr, "Symbol %c not in the declared alphabet.\n", symbol);
                return EXIT_FAILURE;
            }
            addEdge(&nonEpsilonAdj[from][index], to);
        }
    }
    
    // Input the final states of the original NFA.
    printf("Enter the number of final states: ");
    if (scanf("%d", &numFinal) != 1) {
        fprintf(stderr, "Error reading number of final states.\n");
        return EXIT_FAILURE;
    }
    printf("Enter the final states separated by space: ");
    for (int i = 0; i < numFinal; i++) {
        int fs;
        scanf("%d", &fs);
        if (fs < 0 || fs >= n) {
            fprintf(stderr, "Invalid final state: %d\n", fs);
            return EXIT_FAILURE;
        }
        isFinalOriginal[fs] = 1;
    }
    
    // Compute the ε–closure for each state.
    computeAllEpsilonClosures();
    
    // Now, convert the NFA with ε–transitions to one without ε–transitions.
    // For each state and for each input symbol, we compute:
    // new_delta(q, a) = U { ε–closure(p) | p in δ(q, a) for some p in ε–closure(q) }
    
    printf("\nTransitions for the NFA without ε–transitions:\n");
    for (int i = 0; i < n; i++) {
        for (int sym = 0; sym < numSymbols; sym++) {
            // Use a temporary array to mark the target states.
            int targetStates[MAX_STATES] = {0};  // 0: not present, 1: present
            // For each state p in ε–closure(i)
            for (int p = 0; p < n; p++) {
                if (closure[i][p]) {
                    // For each non-ε transition from state p on symbol alphabet[sym]
                    Node* temp = nonEpsilonAdj[p][sym];
                    while (temp != NULL) {
                        int q = temp->state;
                        // For every state r in ε–closure(q), mark it.
                        for (int r = 0; r < n; r++) {
                            if (closure[q][r])
                                targetStates[r] = 1;
                        }
                        temp = temp->next;
                    }
                }
            }
            // Print the transition if there is any destination.
            int found = 0;
            printf("δ'( %d , %c ) = { ", i, alphabet[sym]);
            for (int j = 0; j < n; j++) {
                if (targetStates[j]) {
                    if (found)
                        printf(", ");
                    printf("%d", j);
                    found = 1;
                }
            }
            printf(" }\n");
        }
    }
    
    // Determine the new set of final states.
    // A state i is final in the new NFA if any state in its ε–closure was final in the original NFA.
    printf("\nFinal states in the NFA without ε–transitions:\n{ ");
    int first = 1;
    for (int i = 0; i < n; i++) {
        int isFinal = 0;
        for (int j = 0; j < n; j++) {
            if (closure[i][j] && isFinalOriginal[j]) {
                isFinal = 1;
                break;
            }
        }
        if (isFinal) {
            if (!first)
                printf(", ");
            printf("%d", i);
            first = 0;
        }
    }
    printf(" }\n");
    
    // Free all allocated memory.
    for (int i = 0; i < n; i++) {
        // Free ε–transition list.
        Node *temp = epsilonAdj[i];
        while (temp) {
            Node *toFree = temp;
            temp = temp->next;
            free(toFree);
        }
        // Free non-ε transition lists.
        for (int j = 0; j < numSymbols; j++) {
            temp = nonEpsilonAdj[i][j];
            while (temp) {
                Node *toFree = temp;
                temp = temp->next;
                free(toFree);
            }
        }
    }
    
    return 0;
}

// Adds an edge by prepending a new node with destination 'to' into the linked list pointed by *head.
void addEdge(Node **head, int to) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->state = to;
    newNode->next = *head;
    *head = newNode;
}

// Recursive DFS to compute ε–closure for a given state.
// 'visited' array is used to mark visited states.
void computeEpsilonClosure(int state, int visited[]) {
    visited[state] = 1;
    Node* temp = epsilonAdj[state];
    while (temp != NULL) {
        if (!visited[temp->state]) {
            computeEpsilonClosure(temp->state, visited);
        }
        temp = temp->next;
    }
}

// Compute and store the ε–closure for every state in the global array 'closure'
void computeAllEpsilonClosures() {
    for (int i = 0; i < n; i++) {
        int visited[MAX_STATES] = {0};
        computeEpsilonClosure(i, visited);
        for (int j = 0; j < n; j++) {
            closure[i][j] = visited[j];
        }
    }
}

