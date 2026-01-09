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
