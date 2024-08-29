#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STATES 10
#define MAX_INPUTS 10
#define MAX_QUEUE 20
#define MAX_STRING_LENGTH 20

int states;
int inputs;
int transitions[MAX_INPUTS][MAX_STATES][MAX_STATES];
char nfa_table[MAX_STATES][MAX_INPUTS][MAX_STATES];
char final_dfa[MAX_STATES * MAX_INPUTS][MAX_INPUTS + 1][MAX_STRING_LENGTH];

void get_input();
void fill_nfa_table();
void initialize_dfa(char queue[MAX_QUEUE][MAX_STRING_LENGTH], int *rear);
void process_dfa(char queue[MAX_QUEUE][MAX_STRING_LENGTH], int *rear, int *rows);
void print_dfa(int rows);

int main() {
    char queue[MAX_QUEUE][MAX_STRING_LENGTH];
    int rear = 0;
    int rows = 0;

    get_input();
    fill_nfa_table();
    initialize_dfa(queue, &rear);
    process_dfa(queue, &rear, &rows);
    print_dfa(rows);
    return 0;
}

void get_input() {
    printf("Enter number of states: ");
    scanf("%d", &states);

    printf("Enter number of input symbols: ");
    scanf("%d", &inputs);

    for (int i = 0; i < inputs; i++) {
        printf("Enter NFA matrix for input %d:\n", i + 1);
        for (int j = 0; j < states; j++) {
            for (int k = 0; k < states; k++) {
                scanf("%d", &transitions[i][j][k]);
            }
        }
    }
}

void fill_nfa_table() {
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < states; j++) {
            for (int k = 0; k < inputs; k++) {
                if (transitions[k][i][j] == 1) {
                    char str[MAX_STRING_LENGTH];
                    sprintf(str, "q%d", j);
                    if (strcmp(nfa_table[i][k], str) != 0) {
                        strcat(nfa_table[i][k], str);
                    }
                }
            }
        }
    }
}

void initialize_dfa(char queue[MAX_QUEUE][MAX_STRING_LENGTH], int *rear) {
    for (int i = 0; i < MAX_QUEUE; i++)
        strcpy(queue[i], "");

    strcpy(queue[*rear], "q0");
    (*rear)++;
    strcpy(final_dfa[0][0], "q0");
}

void process_dfa(char queue[MAX_QUEUE][MAX_STRING_LENGTH], int *rear, int *rows) {
    int front = 0;

    while (strcmp(queue[front], "") != 0) {
        int temp_rows = *rows;
        char new_states[MAX_STRING_LENGTH] = "";

        for (int j = 0; j < inputs; j++) {
            char temp_states[MAX_STATES];
            int temp_index = 0;
            for (int i = 1; i < strlen(queue[front]); i += 2) {
                if (isdigit(queue[front][i])) {
                    int n = queue[front][i] - '0';

                    for (int l = 1; l < strlen(nfa_table[n][j]); l += 2) {
                        int num1 = nfa_table[n][j][l] - '0';

                        int flag2 = 0;
                        for (int m = 1; m < strlen(new_states); m += 2) {
                            if (isdigit(new_states[m])) {
                                int num2 = new_states[m] - '0';
                                if (num1 == num2)
                                    flag2 = 1;
                            }
                        }

                        if (flag2 == 0) {
                            char temp[MAX_STRING_LENGTH];
                            sprintf(temp, "q%d", num1);
                            strcat(new_states, temp);
                        }
                    }
                }
            }

            int temp_states_sorted[MAX_STATES];
            int temp_index_sorted = 0;
            for (int d = 0; d < strlen(new_states); d++) {
                if (isdigit(new_states[d])) {
                    temp_states_sorted[temp_index_sorted++] = new_states[d] - '0';
                }
            }

            // Sort states
            for (int q = 0; q < temp_index_sorted; q++) {
                for (int r = 0; r < temp_index_sorted - q - 1; r++) {
                    if (temp_states_sorted[r] > temp_states_sorted[r + 1]) {
                        int swap = temp_states_sorted[r];
                        temp_states_sorted[r] = temp_states_sorted[r + 1];
                        temp_states_sorted[r + 1] = swap;
                    }
                }
            }

            strcpy(new_states, "");
            for (int q = 0; q < temp_index_sorted; q++) {
                char tempstr[MAX_STRING_LENGTH];
                sprintf(tempstr, "q%d", temp_states_sorted[q]);
                strcat(new_states, tempstr);
            }

            int flag = 0;
            for (int a = 0; a < *rear; a++) {
                if (strcmp(queue[a], new_states) == 0) {
                    flag = 1;
                    break;
                }
            }

            if (flag == 0) {
                strcpy(queue[*rear], new_states);
                (*rear)++;
                strcpy(final_dfa[++temp_rows][0], new_states);
            }

            strcpy(final_dfa[*rows][j + 1], new_states);
        }

        (*rows)++;
        front++;
    }
}

void print_dfa(int rows) {
    printf("\nDFA:\n");
    printf("%-10s|", " ");
    for (int i = 0; i < inputs; i++)
        printf("Input %-4d|", i + 1);
    printf("\n");
    for (int i = 0; i < 11 * (inputs + 1); i++)
        printf("=");
    printf("\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < inputs + 1; j++) {
            printf("%-10s|", final_dfa[i][j]);
        }
        printf("\n");
    }
}

