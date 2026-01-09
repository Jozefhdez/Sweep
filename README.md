# Sweep Programming Language

## Description

Sweep is a dynamic programming language under development, designed to be simple and expressive. It features an object system with dynamic typing, supporting integers, floats, strings, 3D vectors, and arrays. It includes arithmetic operations with automatic type coercion, variables, and expression evaluation.

## Installation

```bash
make
```

## Usage

```bash
./build/main
```

The program currently evaluates a hardcoded expression. Future versions will include a REPL.

## Syntax

Sweep uses a simple, expression-based syntax. Statements are separated by newlines.

### Literals
- Integers: `42`
- Floats: `3.14`
- Strings: `"Hello"`
- Vectors: `vec3(1, 2, 3)`
- Arrays: `[1, 2, 3]`

### Operations
Arithmetic operators with precedence: `*`, `/` (highest), `+`, `-` (lowest). Automatic type coercion for compatible types.

Examples:
```
5 + 3 * 2        // 11
3.5 + 2          // 5.5
"Hello " + "World"  // "Hello World"
vec3(1,2,3) + vec3(4,5,6)  // vec3(5,7,9)
[1,2] + [3,4]    // [1,2,3,4]
```

### Variables and Assignment
Use `:=` for assignment (right-associative).

```
x := 5
y := x + 3       // 8
z := "Value: " + y  // "Value: 8"
```

### Multiple Statements
Separate statements with newlines.

```
x := 5 + 3
y := x * 2
```

## Examples

Running `./build/main` evaluates `x := 5 + 3` and prints:
```
Testing Sweep expression: x := 5 + 3

Tokens:
  x
  :=
  5
  +
  3

AST: x := (5 + 3)

Result: 8
```

## Future Implementations

- **Functions**: Function definition and calling.
- **Control Structures**: Conditionals (if/else) and loops (for/while).
- **Memory Management**: Garbage Collector implementation.
- **REPL**: Interactive interface for real-time code testing.
- **Modules and Libraries**: Module import system.
- **More Operators**: Logical, comparison, etc.
- **Error Handling**: Better error messages and recovery.

