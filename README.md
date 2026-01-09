# Sweep Programming Language

Sweep is a dynamic programming language with an interactive REPL. It features an object system with dynamic typing, supporting integers, floats, strings, 3D vectors, and arrays. It includes arithmetic operations with automatic type coercion, variables, expression evaluation, and a **mark-and-sweep garbage collector**.

## Features

- **Dynamic Type System**: Supports INT, FLOAT, STRING, VEC3, and ARRAY types
- **Tree-Walking Interpreter**: Direct AST evaluation without bytecode compilation
- **Mark-and-Sweep Garbage Collection**: Automatic memory management with object graph tracing
- **Interactive REPL**: Real-time expression evaluation with persistent variables

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

## Architecture

### Memory Management
Sweep uses a **mark-and-sweep garbage collector** inspired by the algorithm first described by John McCarthy in 1960. The GC operates in two phases:

1. **Mark Phase**: Traverses the object graph from roots (symbol table), marking all reachable objects
2. **Sweep Phase**: Iterates over all tracked objects, freeing unmarked objects (garbage)

The GC tracks all allocated objects in a VM structure and uses a gray objects stack for depth-first traversal of container objects (VEC3, ARRAY).

### Type System
All objects are represented by `sw_obj_t` with a discriminated union:
- Non-container types: INT, FLOAT, STRING
- Container types: VEC3 (contains 3 objects), ARRAY (contains N objects)

Each object has a `marked` boolean flag used during garbage collection cycles.

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

- **Functions**: Function definition and calling
- **Control Structures**: Conditionals (if/else) and loops (for/while)
- **More Operators**: Logical, comparison, etc.
- **Error Handling**: Better error messages and recovery
- **Optimizations**: Generational GC, incremental collection

## Installation

### Prerequisites
- GCC with C99 support
- Criterion testing framework (optional, for tests)

### Build
```bash
make
```

### Run Tests
```bash
make test
```

## Usage

```bash
./build/main
```

Launches the interactive REPL where you can enter expressions and see results immediately.

