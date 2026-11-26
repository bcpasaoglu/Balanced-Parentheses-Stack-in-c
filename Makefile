CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -g

BIN      = bracket_checker
TEST_BIN = tests/test_stack

.PHONY: all test valgrind clean

all: $(BIN)

$(BIN): main.c stack.c stack.h
	$(CC) $(CFLAGS) main.c stack.c -o $@

$(TEST_BIN): tests/test_stack.c stack.c stack.h
	$(CC) $(CFLAGS) tests/test_stack.c stack.c -o $@

test: $(TEST_BIN) $(BIN)
	./$(TEST_BIN)
	./$(BIN) "{[a + b] * (c - d)}"
	! ./$(BIN) "{[a + b)]"

valgrind: $(TEST_BIN)
	valgrind --leak-check=full --error-exitcode=1 ./$(TEST_BIN)

clean:
	rm -f $(BIN) $(TEST_BIN)
