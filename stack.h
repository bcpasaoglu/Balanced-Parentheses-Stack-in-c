#ifndef STACK_H
#define STACK_H

/* Status codes returned by stack operations. */
#define STACK_OK 0
#define STACK_EMPTY (-1)
#define STACK_NO_MEMORY (-2)

/* Return values of AreParenthesesBalanced. */
#define BALANCED 1
#define NOT_BALANCED 0
#define CHECK_ERROR (-1)

typedef struct Node {
    char val;
    struct Node* next;
} StackRecord;

/* A stack is a header node; elements are linked after it. */
typedef StackRecord* Stack;

/* Returns NULL if memory allocation fails. */
Stack CreateStack(void);
/* Frees every element and the header node. Safe to call with NULL. */
void DestroyStack(Stack s);
void MakeEmptyStack(Stack s);
int IsEmptyStack(Stack s);
/* Returns STACK_OK or STACK_NO_MEMORY. */
int PushStack(char x, Stack s);
/* Store the popped / top value in *out. Return STACK_OK or STACK_EMPTY. */
int PopStack(Stack s, char* out);
int TopOfStack(Stack s, char* out);

int IsMatchingPair(char open, char close);
/* Checks (), [] and {}; other characters are ignored. The stack is cleared
 * before and after the check. Returns BALANCED, NOT_BALANCED or CHECK_ERROR. */
int AreParenthesesBalanced(Stack s, const char* expr);

#endif /* STACK_H */
