CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -g

BIN      = bracket_checker
TEST_BIN = tests/test_stack

.PHONY: all test valgrind sanitize clean

all: $(BIN)

$(BIN): main.c stack.c stack.h
	$(CC) $(CFLAGS) main.c stack.c -o $@

$(TEST_BIN): tests/test_stack.c stack.c stack.h
	$(CC) $(CFLAGS) tests/test_stack.c stack.c -o $@

test: $(TEST_BIN) $(BIN)
	./$(TEST_BIN)
	./$(BIN) "{[a + b] * (c - d)}"
	! ./$(BIN) "{[a + b)]"
	echo "(a + b) * [c]" | ./$(BIN)
	! printf "" | ./$(BIN)

valgrind: $(TEST_BIN)
	valgrind --leak-check=full --error-exitcode=1 ./$(TEST_BIN)

# AddressSanitizer + UndefinedBehaviorSanitizer build of the unit tests.
sanitize:
	$(CC) $(CFLAGS) -O1 -fsanitize=address,undefined -fno-omit-frame-pointer \
		tests/test_stack.c stack.c -o tests/test_stack_san
	./tests/test_stack_san

clean:
	rm -f $(BIN) $(TEST_BIN) tests/test_stack_san
