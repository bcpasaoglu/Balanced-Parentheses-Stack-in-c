#include "../stack.h"

#include <stdio.h>

static int failures = 0;
static int checks = 0;

#define CHECK(cond)                                                         \
    do {                                                                    \
        checks++;                                                           \
        if (!(cond)) {                                                      \
            failures++;                                                     \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                   \
    } while (0)

static void test_push_pop_order(void) {
    Stack s = CreateStack();
    char out = 0;
    CHECK(s != NULL);
    CHECK(IsEmptyStack(s));
    CHECK(PushStack('a', s) == STACK_OK);
    CHECK(PushStack('b', s) == STACK_OK);
    CHECK(TopOfStack(s, &out) == STACK_OK && out == 'b');
    CHECK(PopStack(s, &out) == STACK_OK && out == 'b');
    CHECK(PopStack(s, &out) == STACK_OK && out == 'a');
    CHECK(IsEmptyStack(s));
    DestroyStack(s);
}

static void test_empty_stack_errors(void) {
    Stack s = CreateStack();
    char out = 'x';
    CHECK(PopStack(s, &out) == STACK_EMPTY);
    CHECK(TopOfStack(s, &out) == STACK_EMPTY);
    CHECK(out == 'x');
    DestroyStack(s);
    DestroyStack(NULL);
}

static void test_make_empty(void) {
    Stack s = CreateStack();
    for (int i = 0; i < 1000; i++) {
        PushStack('(', s);
    }
    MakeEmptyStack(s);
    CHECK(IsEmptyStack(s));
    DestroyStack(s);
}

static void test_balanced_expressions(void) {
    const char* balanced[] = {"", "()", "[]{}()", "{[()]}", "(a + b)", "{[a + b] * (c - d)}", "no brackets"};
    const char* unbalanced[] = {"(", ")", "(]", "{[a + b)]", "(a + b", "())(", "[[[]]"};
    Stack s = CreateStack();

    for (size_t i = 0; i < sizeof balanced / sizeof *balanced; i++) {
        CHECK(AreParenthesesBalanced(s, balanced[i]) == BALANCED);
    }
    for (size_t i = 0; i < sizeof unbalanced / sizeof *unbalanced; i++) {
        CHECK(AreParenthesesBalanced(s, unbalanced[i]) == NOT_BALANCED);
    }
    DestroyStack(s);
}

static void test_stack_is_reusable_after_early_exit(void) {
    Stack s = CreateStack();
    CHECK(AreParenthesesBalanced(s, "((((]") == NOT_BALANCED);
    CHECK(IsEmptyStack(s));
    CHECK(AreParenthesesBalanced(s, "()") == BALANCED);
    DestroyStack(s);
}

int main(void) {
    test_push_pop_order();
    test_empty_stack_errors();
    test_make_empty();
    test_balanced_expressions();
    test_stack_is_reusable_after_early_exit();

    if (failures > 0) {
        fprintf(stderr, "%d of %d checks failed\n", failures, checks);
        return 1;
    }
    printf("All %d checks passed\n", checks);
    return 0;
}
