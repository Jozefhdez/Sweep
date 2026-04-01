# Sweep

A dynamically typed language with an interactive REPL, written in C.

## Build & Run

```bash
make
./build/main
```

## Language Tour

### Types

```
42              // integer
3.14            // float
"hello"         // string
(1, 2, 3)       // vec3
[1, 2, 3]       // array
```

### Arithmetic

Operator precedence: `*` `/` bind tighter than `+` `-`. Mixed types coerce automatically.

```
5 + 3 * 2           // 11
3.5 + 2             // 5.5
"hello " + "world"  // "hello world"
(1,2,3) + (4,5,6)  // (5, 7, 9)
2 * (1, 2, 3)       // (2, 4, 6)
[1,2] + [3,4]       // [1, 2, 3, 4]
```

### Variables

```
x := 5
y := x + 3          // 8
v := (1,2,3) * 2    // (2, 4, 6)
```

### Comparisons & Logic

```
x := 10
x > 5               // 1 (true)
x == 10             // 1
x != 3              // 1
x > 3 && x < 20     // 1
x < 0 || x == 10    // 1
! (x > 3)           // 0 (false)
```

### Control Flow

```
x := 8

if (x > 5) {
    result := "big"
} else {
    result := "small"
}

i := 0
while (i < 5) {
    i := i + 1
}
```

### Functions

Functions have lexical scope — locals don't leak into the caller.

```
fn add(a, b) {
    return a + b
}

add(3, 4)       // 7
```

Multi-statement bodies and early return work as expected:

```
fn clamp(x) {
    if (x > 100) { return 100 }
    if (x < 0)   { return 0 }
    return x
}

clamp(42)       // 42
clamp(999)      // 100
clamp(-5)       // 0
```

Recursion:

```
fn factorial(n) {
    if (n == 0) { return 1 }
    return n * factorial(n - 1)
}

factorial(10)   // 3628800
```

Scope isolation — `n` in the caller is untouched by the function's parameter:

```
n := 7

fn double(n) {
    return n * 2
}

double(3)       // 6
n               // 7
```

### REPL

Variables persist across inputs. Use `.exit` to quit.

```
Sweep > x := 10
10

Sweep > x * 3
30

Sweep > fn square(n) { return n * n }
Sweep > square(x)
100

Sweep > .exit
```

## How It Works

Sweep is a tree-walking interpreter: source text is lexed into tokens, parsed into an AST, and evaluated directly. A mark-and-sweep garbage collector runs every 256 allocations, tracing live objects from the symbol table and freeing everything unreachable. Functions get their own scope frame pushed onto a linked chain, so locals never bleed across call boundaries.