#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 100  // maximum number of states

// Node structure for linked list to store epsilon transitions
typedef struct Node {
    int state;
    struct Node* next;
} Node;

// Array of pointers to the head of linked lists for each state
Node* epsilonAdj[MAX_STATES];

// Function to add an edge from state 'from' to state 'to' in the epsilon transitions list
void addEdge(int from, int to) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->state = to;
    newNode->next = epsilonAdj[from];
    epsilonAdj[from] = newNode;
}

// Recursive function to compute the ε–closure for a given state.
// 'visited' array marks states already in the closure (to avoid cycles).
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

int main() {
    int n, m;  // n = number of states, m = number of transitions

    // Input the number of states.
    printf("Enter the number of states in the NFA: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error reading number of states.\n");
        return EXIT_FAILURE;
    }
    if(n > MAX_STATES) {
        fprintf(stderr, "The number of states exceeds the maximum allowed (%d).\n", MAX_STATES);
        return EXIT_FAILURE;
    }

    // Initialize the epsilon transitions list heads to NULL.
    for (int i = 0; i < n; i++) {
        epsilonAdj[i] = NULL;
    }

    // Input the number of transitions.
    printf("Enter the number of transitions: ");
    if (scanf("%d", &m) != 1) {
        fprintf(stderr, "Error reading number of transitions.\n");
        return EXIT_FAILURE;
    }

    printf("Enter transitions in the format: from_state input_symbol to_state\n");
    printf("Note: For an epsilon transition, enter the symbol as 'e' (without quotes).\n");

    // Read each transition. We only add the transition if the symbol is 'e'
    for (int i = 0; i < m; i++) {
        int from, to;
        char symbol;
        if (scanf("%d %c %d", &from, &symbol, &to) != 3) {
            fprintf(stderr, "Error reading transition %d.\n", i+1);
            return EXIT_FAILURE;
        }
        if (symbol == 'e' || symbol == 'E') {
            addEdge(from, to);
        }
        // If the symbol is not 'e', you may want to handle other transitions differently.
    }

    // For each state, compute and print its ε–closure.
    for (int i = 0; i < n; i++) {
        int visited[MAX_STATES] = {0}; // visited array for DFS
        computeEpsilonClosure(i, visited);

        printf("ε–closure of state %d: { ", i);
        int first = 1;
        for (int j = 0; j < n; j++) {
            if (visited[j]) {
                if (!first)
                    printf(", ");
                printf("%d", j);
                first = 0;
            }
        }
        printf(" }\n");
    }

    // Free allocated memory for epsilon transitions
    for (int i = 0; i < n; i++) {
        Node* temp = epsilonAdj[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }

    return 0;
}

