#include "stack.h"

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 4096

/* Exit codes: 0 = balanced, 1 = not balanced, 2 = error. */
static int report(Stack s, const char* expr) {
    int result = AreParenthesesBalanced(s, expr);
    if (result == CHECK_ERROR) {
        fprintf(stderr, "Error: out of memory\n");
        return 2;
    }
    printf("%s: %s\n", expr, result == BALANCED ? "balanced" : "NOT balanced");
    return result == BALANCED ? 0 : 1;
}

int main(int argc, char** argv) {
    Stack stack = CreateStack();
    if (stack == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 2;
    }

    int status = 0;
    if (argc > 1) {
        /* Check each command-line argument. */
        for (int i = 1; i < argc; i++) {
            int rc = report(stack, argv[i]);
            if (rc > status) {
                status = rc;
            }
        }
    } else {
        /* Read one full line so expressions may contain spaces. */
        char line[MAX_LINE_LEN];
        printf("Enter expression: ");
        fflush(stdout);
        if (fgets(line, sizeof line, stdin) == NULL) {
            fprintf(stderr, "Error: no input\n");
            DestroyStack(stack);
            return 2;
        }
        line[strcspn(line, "\r\n")] = '\0';
        status = report(stack, line);
    }

    DestroyStack(stack);
    return status;
}
