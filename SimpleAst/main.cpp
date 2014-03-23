#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <stack>
using namespace std;

#define NUMBER 'N'
#define ADD '+' 
#define MUL '*' 

#define TERM 11
#define FACTOR 12 
#define EXP 13 

struct ast {
    int nodetype;
    struct ast* left;
    struct ast* right;
};

stack<int> shift_reduce;
stack<ast*> ast_stack;


/*************************************
    exp:     factor | exp ADD factor;
    factor:  term | factor MUL term;
    term:    number;
 *************************************/
void parser(int* token, int token_num) {
    int i = 0;
    shift_reduce.push(token[i]);

    while (i <= token_num - 1) {
        if (shift_reduce.top() == NUMBER) {
            shift_reduce.pop();
            shift_reduce.push(TERM);

            // ast operation.
            ast* current = (ast*) malloc(sizeof(ast));
            current->nodetype = NUMBER;
            current->left = 0;
            current->right = 0;
            ast_stack.push(current);

            continue;
        }
        
        if (shift_reduce.top() == TERM) {            
            if (shift_reduce.size() == 1) {
                shift_reduce.pop();
                shift_reduce.push(FACTOR);
            }
            else {
                shift_reduce.pop();
                if (shift_reduce.top() == MUL) {
                    shift_reduce.pop();
                    if (shift_reduce.top() == FACTOR) {
                        shift_reduce.pop();
                        shift_reduce.push(FACTOR);

                        ast* current = (ast*)malloc(sizeof(ast));
                        current->nodetype = MUL;
                        current->right = ast_stack.top();
                        ast_stack.pop();
                        current->left = ast_stack.top();
                        ast_stack.pop();
                        ast_stack.push(current);
                    }
                }
                else {
                    shift_reduce.push(FACTOR);
                }
            }
            continue;
        }
        
        if (shift_reduce.top() == FACTOR) {
            // look ahead
            if (token[i+1] == MUL) {
                shift_reduce.push(token[i+1]);
                i++;
                continue;
            }

            if (shift_reduce.size() == 1) {
                shift_reduce.pop();
                shift_reduce.push(EXP);
            }
            else {
                shift_reduce.pop();
                if (shift_reduce.top() == ADD) {
                    shift_reduce.pop();
                    if (shift_reduce.top() == EXP) {
                        shift_reduce.pop();
                        shift_reduce.push(EXP);

                        ast* current = (ast*)malloc(sizeof(ast));
                        current->nodetype = ADD;
                        current->right = ast_stack.top();
                        ast_stack.pop();
                        current->left = ast_stack.top();
                        ast_stack.pop();
                        ast_stack.push(current);
                    }
                }
            }
            continue;
        }

        // can not reduce. so let's check if it is done.
        if (i == token_num - 1) {
            if (shift_reduce.top() == EXP) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
            }
            break;
        }
        else {
            i++;
            shift_reduce.push(token[i]);
        }
    }
}

void ast_out(ast* current) {
    if (current->left) {
        ast_out(current->left);
    }
    if (current->right) {
        ast_out(current->right);
    }
    
    printf("%c ", current->nodetype);
}

int main() {
    int token[5] = {NUMBER, ADD, NUMBER, MUL, NUMBER};
    parser(token, 5);

    ast* current = ast_stack.top();
    ast_stack.pop();
    ast_out(current);

    return 0;
}