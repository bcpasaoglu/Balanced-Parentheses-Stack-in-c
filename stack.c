#include "stack.h"

#include <stdlib.h>

Stack CreateStack(void) {
    Stack s = malloc(sizeof(StackRecord));
    if (s == NULL) {
        return NULL;
    }
    s->next = NULL;
    return s;
}

void DestroyStack(Stack s) {
    if (s == NULL) {
        return;
    }
    MakeEmptyStack(s);
    free(s);
}

void MakeEmptyStack(Stack s) {
    while (!IsEmptyStack(s)) {
        Stack temp = s->next;
        s->next = temp->next;
        free(temp);
    }
}

int IsEmptyStack(Stack s) {
    return s->next == NULL;
}

int PushStack(char x, Stack s) {
    Stack temp = malloc(sizeof(StackRecord));
    if (temp == NULL) {
        return STACK_NO_MEMORY;
    }
    temp->val = x;
    temp->next = s->next;
    s->next = temp;
    return STACK_OK;
}

int PopStack(Stack s, char* out) {
    if (IsEmptyStack(s)) {
        return STACK_EMPTY;
    }
    Stack temp = s->next;
    if (out != NULL) {
        *out = temp->val;
    }
    s->next = temp->next;
    free(temp);
    return STACK_OK;
}

int TopOfStack(Stack s, char* out) {
    if (IsEmptyStack(s)) {
        return STACK_EMPTY;
    }
    if (out != NULL) {
        *out = s->next->val;
    }
    return STACK_OK;
}

int IsMatchingPair(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '{' && close == '}') ||
           (open == '[' && close == ']');
}

int AreParenthesesBalanced(Stack s, const char* expr) {
    int result = BALANCED;
    MakeEmptyStack(s);

    for (const char* p = expr; *p != '\0'; p++) {
        char ch = *p;
        if (ch == '(' || ch == '{' || ch == '[') {
            if (PushStack(ch, s) != STACK_OK) {
                result = CHECK_ERROR;
                break;
            }
        } else if (ch == ')' || ch == '}' || ch == ']') {
            char top;
            if (PopStack(s, &top) != STACK_OK || !IsMatchingPair(top, ch)) {
                result = NOT_BALANCED;
                break;
            }
        }
    }

    if (result == BALANCED && !IsEmptyStack(s)) {
        result = NOT_BALANCED;
    }
    MakeEmptyStack(s);
    return result;
}
