# Sweep Programming Language

Sweep is a dynamic programming language with an interactive REPL. It features an object system with dynamic typing, supporting integers, floats, strings, 3D vectors, and arrays. It includes arithmetic operations with automatic type coercion, variables, and expression evaluation.

## Syntax

### Literals
- Integers: `42`
- Floats: `3.14`
- Strings: `"Hello"`
- Vectors: `(1, 2, 3)`
- Arrays: `[1, 2, 3]`

### Operations
Arithmetic operators with precedence: `*`, `/` (highest), `+`, `-` (lowest). Automatic type coercion for compatible types. Supports scalar-vector and scalar-array operations.

Examples:
```
5 + 3 * 2                    // 11
3.5 + 2                      // 5.5
"Hello " + "World"           // "Hello World"
(1,2,3) + (4,5,6)            // (5,7,9)
2 * (10,20,30)               // (20,40,60)
[1,2] + [3,4]                // [1,2,3,4]
[1+1, 2*3, 10/2]             // [2,6,5]
```

### Variables and Assignment
Use `:=` for assignment. Variables persist across statements in the REPL session.

```
x := 5
y := x + 3                   // 8
z := (1,2,3) * 2            // (2,4,6)
arr := [1, 2, 3, 4, 5]
```

### REPL Commands
- `.exit` - Exit the REPL and free memory
- `clear` - Clear the terminal screen

## Examples

Interactive REPL session:
```
Sweep > x := 10
10

Sweep > y := x * 2
20

Sweep > (1,2,3) + (4,5,6)
(5, 7, 9)

Sweep > [1, 2, 3] + [4, 5]
[1, 2, 3, 4, 5]

Sweep > .exit
```

## Future Implementations

- **Functions**: Function definition and calling.
- **Control Structures**: Conditionals (if/else) and loops (for/while).
- **Memory Management**: Garbage Collector implementation.
- **More Operators**: Logical, comparison, etc.
- **Error Handling**: Better error messages and recovery.

## Installation

```bash
make
```

## Usage

```bash
./build/main
```

Launches the interactive REPL where you can enter expressions and see results immediately.

