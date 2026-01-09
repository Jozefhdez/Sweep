# Sweep Programming Language

## Description

Sweep is a dynamic programming language under development, designed to be simple and expressive. It currently features an object system that supports basic types such as integers, floats, strings, 3D vectors, and arrays, with basic operations like addition.

## Installation

```bash
make
```

## Usage

```bash
./build/main
```

## Syntax

Sweep features a creative, expressive syntax inspired by mathematics and data manipulation. It's designed to feel like "sweeping" through data with ease. Below is the proposed syntax (subject to change):

### Variables and Assignment
```
x := 5
name := "Sweep"
```

### Operations
```
sum := x + 3
message := name + " Language"
vector_sum := vec3(1,2,3) + vec3(4,5,6)  // Results in vec3(5,7,9)
array_concat := [1,2] ++ [3,4]  // Results in [1,2,3,4]
```

### Functions
```
add := fn(a, b) => a + b;
result := add(2, 3);

complex_fn := fn(x) {
    if x > 10 {
        return x * 2;
    } else {
        return x + 1;
    }
}
```

### Control Structures
```
if x > 10 {
    print("Large!");
} elif x > 5 {
    print("Medium!");
} else {
    print("Small!");
}

for i in 0..10 {
    print(i);
}

while x < 100 {
    x := x * 2;
}
```

### Data Types
```
int := 42;
float := 3.14;
str := "Hello";
vec := vec3(1.0, 2.0, 3.0);
arr := [1, 2, 3, 4];
```

### Comments
```
// Line comment
/* Block comment */
```

## Future Implementations

- **Lexer**: Source code tokenization.
- **Parser**: Abstract Syntax Tree (AST) construction.
- **Interpreter**: Code execution from the AST.
- **Variables and Assignment**: Support for declaring and using variables.
- **Functions**: Function definition and calling.
- **Control Structures**: Conditionals (if/else) and loops (for/while).
- **Memory Management**: Garbage Collector implementation.
- **REPL**: Interactive interface for real-time code testing.
- **More Operators**: Addition, subtraction, multiplication, division, etc., for all types.
- **Modules and Libraries**: Module import system.

