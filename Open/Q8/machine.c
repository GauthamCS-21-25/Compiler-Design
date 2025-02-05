#include <stdio.h>
#include <stdlib.h>

// Define AST Node Structure
typedef struct ASTNode {
    char op;  // Operator: '+', '-', '*', '/', '='
    char var; // Variable (if applicable)
    struct ASTNode *left, *right;
} ASTNode;

// Create a new AST node
ASTNode* createNode(char op, char var, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->op = op;
    node->var = var;
    node->left = left;
    node->right = right;
    return node;
}

// Generate Assembly Code from AST (Post-order Traversal)
void generateCode(ASTNode* root) {
    if (root == NULL) return;

    // Traverse Left and Right first (Post-order)
    generateCode(root->left);
    generateCode(root->right);

    // Process the current node
    if (root->op == '=') {
        printf("MOV [%c], AX\n", root->var);
    } else if (root->op == '+') {
        printf("POP BX\n");
        printf("ADD AX, BX\n");
    } else if (root->op == '-') {
        printf("POP BX\n");
        printf("SUB AX, BX\n");
    } else if (root->op == '*') {
        printf("POP BX\n");
        printf("MUL BX\n"); // Result stored in AX
    } else if (root->op == '/') {
        printf("POP BX\n");
        printf("DIV BX\n"); // AX / BX, quotient in AX
    } else { // Operand (variable)
        printf("MOV AX, [%c]\n", root->var);
        printf("PUSH AX\n"); // Store for future operations
    }
}

// Main function
int main() {
    /*
      Example AST for: a = b + c * d
             =
           /   \
          a     +
              /   \
             b     *
                 /   \
                c     d
    */

    ASTNode* mul = createNode('*', '\0', createNode('c', 'c', NULL, NULL), createNode('d', 'd', NULL, NULL));
    ASTNode* add = createNode('+', '\0', createNode('b', 'b', NULL, NULL), mul);
    ASTNode* assign = createNode('=', 'a', add, NULL);

    printf("Generated 8086 Assembly Code:\n\n");
    generateCode(assign);

    return 0;
}

