#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exit codes, so the tool can be used in shell scripts. */
#define EXIT_BALANCED 0
#define EXIT_NOT_BALANCED 1
#define EXIT_ERROR 2

/* Reads one line of any length from fp. Returns a malloc'd string without the
 * trailing newline, or NULL on EOF before any input or on allocation failure.
 * *oom is set to 1 if the failure was an allocation failure. */
static char* read_line(FILE* fp, int* oom) {
    size_t cap = 128;
    size_t len = 0;
    char* buf = malloc(cap);
    int ch;

    *oom = 0;
    if (buf == NULL) {
        *oom = 1;
        return NULL;
    }
    while ((ch = fgetc(fp)) != EOF && ch != '\n') {
        if (len + 1 == cap) {
            char* bigger = realloc(buf, cap * 2);
            if (bigger == NULL) {
                free(buf);
                *oom = 1;
                return NULL;
            }
            buf = bigger;
            cap *= 2;
        }
        buf[len++] = (char)ch;
    }
    if (ch == EOF && len == 0) {
        free(buf);
        return NULL;
    }
    if (len > 0 && buf[len - 1] == '\r') {
        len--;
    }
    buf[len] = '\0';
    return buf;
}

static int report(Stack s, const char* expr) {
    size_t pos = 0;
    int result = CheckBrackets(s, expr, &pos);

    if (result == CHECK_ERROR) {
        fprintf(stderr, "Error: out of memory\n");
        return EXIT_ERROR;
    }
    if (result == BALANCED) {
        printf("%s: balanced\n", expr);
        return EXIT_BALANCED;
    }
    if (expr[pos] == '\0') {
        printf("%s: NOT balanced (unclosed bracket at end of input)\n", expr);
    } else {
        printf("%s: NOT balanced (unexpected '%c' at position %zu)\n", expr,
               expr[pos], pos);
    }
    return EXIT_NOT_BALANCED;
}

int main(int argc, char** argv) {
    Stack stack = CreateStack();
    int status = EXIT_BALANCED;

    if (stack == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return EXIT_ERROR;
    }

    if (argc > 1) {
        /* Check each command-line argument; the worst result wins. */
        for (int i = 1; i < argc; i++) {
            int rc = report(stack, argv[i]);
            if (rc > status) {
                status = rc;
            }
        }
    } else {
        /* Read one full line so expressions may contain spaces. */
        int oom;
        char* line;

        printf("Enter expression: ");
        fflush(stdout);
        line = read_line(stdin, &oom);
        if (line == NULL) {
            fprintf(stderr, oom ? "Error: out of memory\n" : "Error: no input\n");
            DestroyStack(stack);
            return EXIT_ERROR;
        }
        status = report(stack, line);
        free(line);
    }

    DestroyStack(stack);
    return status;
}
