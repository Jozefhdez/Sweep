CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.. -I../include -I/opt/homebrew/include -g
LDFLAGS = -L/opt/homebrew/lib -lcriterion -fsanitize=address

SRC_DIR = ..

# Source files (without main.c)
SOURCES = $(SRC_DIR)/sw_obj.c $(SRC_DIR)/sw_ops.c $(SRC_DIR)/sw_array.c \
          $(SRC_DIR)/sw_lexer.c $(SRC_DIR)/sw_parser.c $(SRC_DIR)/sw_interpreter.c \
          $(SRC_DIR)/sw_gc.c $(SRC_DIR)/sw_stack.c

# Test files
TEST_SOURCES = test_objects.c test_operations.c test_lexer.c test_parser.c test_gc.c

# Test executables
TESTS = $(TEST_SOURCES:.c=)

.PHONY: all clean run

all: $(TESTS)

%: %.c $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

run: all
	@for test in $(TESTS); do \
		echo ""; \
		./$$test --verbose; \
	done

clean:
	rm -f $(TESTS) *.o
