# Bracket Validator in C

[![CI](https://github.com/bcpasaoglu/Balanced-Parentheses-Stack-in-c/actions/workflows/ci.yml/badge.svg)](https://github.com/bcpasaoglu/Balanced-Parentheses-Stack-in-c/actions/workflows/ci.yml)

A small command-line tool in C11 that checks whether the brackets in an expression are balanced and, if not, points to the first offending character.

It is built on a linked-list stack written from scratch, with status-code based error handling, unit tests, and CI that runs every build under `-Werror`, valgrind, and AddressSanitizer/UBSan on both gcc and clang.

## What It Does

The program checks each expression given as an argument, or one line read from standard input, and reports whether its brackets are correctly opened and closed. Other characters are ignored.

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
make sanitize   # unit tests built with AddressSanitizer + UBSan
```

```txt
$ ./bracket_checker "{[a + b] * (c - d)}" "(a + b"
{[a + b] * (c - d)}: balanced
(a + b: NOT balanced (unclosed bracket at end of input)

$ echo "{[a+b)*(c-d)}" | ./bracket_checker
Enter expression: {[a+b)*(c-d)}: NOT balanced (unexpected ')' at position 5)
```

Exit codes: `0` balanced, `1` not balanced, `2` error (out of memory or no input), so the tool can be used in scripts.

## How It Works

Scan the expression left to right. Push every opening bracket. On a closing bracket, pop and check that it matches; an empty stack or a mismatch means the expression is not balanced, and the current index is the error position. At the end the stack must be empty, otherwise an opening bracket was never closed.

- Time: O(n) for an expression of length n.
- Space: O(d), where d is the maximum nesting depth (one heap node per open bracket).

## Design Notes

- **No `exit()` inside the library.** Stack functions return status codes (`STACK_OK`, `STACK_EMPTY`, `STACK_NO_MEMORY`) and pass values through out-parameters, so the caller decides how to handle errors.
- **No leaks on early exit.** The checker clears the stack before and after each run, so a mismatched expression never leaves nodes behind. CI runs the tests under valgrind with gcc and clang.
- **Whole-line input of any length.** Standard input is read into a growing buffer instead of `scanf("%s")`, which stopped at the first space and could overflow a fixed buffer.
- **Tested edge cases.** Empty input, stray closing brackets, wrong bracket type, unclosed brackets, error positions, reuse of the stack after an early exit, and 100,000 levels of nesting.

## Concepts Practiced

- Stack data structure on a singly linked list (header-node design)
- Dynamic memory allocation and leak-free error paths
- API design with status codes and out-parameters
- Header and source file separation, Makefile builds
- Unit testing and continuous integration for C
