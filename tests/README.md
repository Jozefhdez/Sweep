# Sweep Test Suite

Unit tests for the Sweep programming language using [Criterion](https://github.com/Snaipe/Criterion) testing framework.

## Prerequisites

Install Criterion:
```bash
brew install criterion
```

## Test Files

- `test_objects.c` - Object creation and type system tests
- `test_operations.c` - Arithmetic operations and type coercion tests
- `test_lexer.c` - Tokenization and lexical analysis tests
- `test_parser.c` - AST generation and parsing tests

## Running Tests

Run all tests with verbose output:
```bash
make run
```

Run a specific test suite:
```bash
./test_objects --verbose
./test_operations --verbose
./test_lexer --verbose
./test_parser --verbose
```

Run with pattern matching:
```bash
./test_operations --filter operations/int_*
```

Clean test binaries:
```bash
make clean
```

## Test Output

Criterion provides colored output with:
- ✓ Passing tests in green
- ✗ Failing tests in red
- Detailed assertion failure messages
- Test execution time
- Summary statistics

## Writing New Tests

Use the `Test()` macro to define tests:
```c
#include <criterion/criterion.h>

Test(suite_name, test_name) {
    cr_assert_eq(actual, expected, "message");
    cr_assert_not_null(ptr, "message");
    cr_assert_str_eq(str1, str2, "message");
}
```

See [Criterion documentation](https://criterion.readthedocs.io/) for more assertion macros.
