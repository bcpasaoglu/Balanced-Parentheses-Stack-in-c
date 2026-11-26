# Balanced Parentheses Stack in C

[![CI](https://github.com/bcpasaoglu/Balanced-Parentheses-Stack-in-c/actions/workflows/ci.yml/badge.svg)](https://github.com/bcpasaoglu/Balanced-Parentheses-Stack-in-c/actions/workflows/ci.yml)

This is a small C project that checks whether the brackets in a given expression are balanced.

The project uses a stack data structure implemented with a linked list. It was created as a learning project to practice basic data structures, dynamic memory allocation, and separating code into header and source files.

## What It Does

The program reads an expression from the user and checks whether the brackets are correctly opened and closed.

Supported bracket types:

```txt
()
[]
{}
```

Balanced examples:

```txt
(a + b)
{[a + b] * (c - d)}
```

Unbalanced examples:

```txt
(a + b
{[a + b)]
```

## Project Structure

```txt
.
├── main.c              CLI: checks arguments or one line from stdin
├── stack.c / stack.h   Linked-list stack and the bracket checker
├── tests/test_stack.c  Unit tests
└── Makefile
```

## Build, Test and Run

```bash
make            # builds ./bracket_checker with -Wall -Wextra -Wpedantic -Werror
make test       # unit tests + CLI smoke tests
make valgrind   # unit tests under valgrind, fails on any leak
```

```txt
$ ./bracket_checker "{[a + b] * (c - d)}" "(a + b"
{[a + b] * (c - d)}: balanced
(a + b: NOT balanced

$ echo "{[a+b)*(c-d)}" | ./bracket_checker
Enter expression: {[a+b)*(c-d)}: NOT balanced
```

Exit codes: `0` balanced, `1` not balanced, `2` error (out of memory or no input), so the tool can be used in scripts.

## Design Notes

- **No `exit()` inside the library.** Stack functions return status codes (`STACK_OK`, `STACK_EMPTY`, `STACK_NO_MEMORY`) and pass values through out-parameters, so the caller decides how to handle errors.
- **No leaks on early exit.** The checker clears the stack before and after each run, so a mismatched expression never leaves nodes behind. CI runs the tests under valgrind with gcc and clang.
- **Whole-line input.** Input is read with `fgets` instead of `scanf("%s")`, which stopped at the first space and could overflow the buffer.

## Concepts Practiced

This project was made to practice:

- Stack data structure
- Linked list implementation
- Dynamic memory allocation
- Header and source file separation
- Basic algorithm design in C

## Notes

This is a simple learning project. The main goal is to understand how a stack can be used to solve the balanced parentheses problem.
