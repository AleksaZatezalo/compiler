# C Compiler

A C compiler implementation following the book [**Writing a C Compiler**](https://nostarch.com/writing-c-compiler) by Nora Sandler (No Starch Press).

## About

This project is a working implementation of a C compiler written from scratch. The compiler translates C source code into x86-64 assembly, following the step-by-step approach outlined in the book.

## Features

The compiler is being built incrementally with support for:

- Lexical analysis (tokenization)
- Parsing C syntax into an Abstract Syntax Tree (AST)
- Semantic analysis and type checking
- Code generation for x86-64 assembly
- Progressive feature implementation following the book's chapters

## Building

```bash
# Build the parser
make

# Clean and rebuild
make rebuild

# See all available commands
make help
```

## Usage

**Important:** Our compiler generates assembly (`.s` files), not executables. You need GCC to create the final executable.

### Complete Workflow

```bash
# Step 1: Compile C to assembly (our compiler)
./compiler return_2.c return_2.s

# Step 2: Assemble to executable (using GCC)
gcc return_2.s -o return_2

# Step 3: Run the executable
./return_2
echo $?  # Check exit code (should be 2)
```

### One-Liner

```bash
./compiler test.c test.s && gcc test.s -o test && ./test && echo "Exit: $?"
```

### Standalone Parser (for debugging)

```bash
# Build and run parser to see tokens and AST
make parser
./parser return_2.c
```

### ⚠️ Common Mistake

```bash
# WRONG: Don't do this
./compiler return_2.c hello
./hello  # ERROR! "hello" is assembly, not executable

# RIGHT: Use .s extension and assemble first
./compiler return_2.c hello.s
gcc hello.s -o hello
./hello  # Now it works!
```

See [USAGE.md](USAGE.md) for detailed examples.

## Project Structure

```
compiler/
├── lexer.h         # Lexer declarations
├── lexer.c         # Lexer implementation (tokenization)
├── parser.h        # Parser declarations
├── parser.c        # Parser implementation (AST building)
├── codegen.h       # Code generator declarations
├── codegen.c       # Code generator (x86-64 assembly)
├── compiler.c      # Main compiler program
├── Makefile        # Build system
├── tests/          # Test cases
│   ├── return_0.c
│   ├── return_42.c
│   └── return_100.c
├── return_2.c      # Example test file
├── README.md       # This file
└── TESTING.md      # Testing guide
```

## Current Status

### ✅ Completed (Chapter 1)
- **Lexical Analysis** - Tokenizes C source code into tokens
- **Syntax Analysis** - Parses tokens into Abstract Syntax Tree (AST)
- **Code Generation** - Generates x86-64 assembly from AST
- **Test Suite** - Automated testing framework
- **Working Compiler** - Can compile simple return statements to executable programs

### Supported C Features
- `int main(void) { ... }` function definition
- `return <integer>;` statements
- Integer literals

### 📋 Next Steps (Following the Book)
- Chapter 2: Unary operators
- Chapter 3: Binary operators
- Chapter 4: Logical operators and control flow
- Chapter 5: Local variables
- And more...

## Requirements

- GCC or Clang (for building the parser itself)
- GNU Make
- Linux, macOS, or WSL

## Testing

```bash
# Run single test
make test

# Run all tests
make test-all

# Run all tests with verbose output (shows tokens and AST)
make test-verbose
```

The test suite will automatically find and run all `.c` files in the `tests/` directory.

## Documentation

- **[QUICKSTART.md](QUICKSTART.md)** - Get started in 5 minutes
- **[USAGE.md](USAGE.md)** - Detailed usage guide and common mistakes
- **[TESTING.md](TESTING.md)** - Testing guide and debugging tips
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Technical design and internals

## References

- **Book**: [Writing a C Compiler](https://nostarch.com/writing-c-compiler) by Nora Sandler
- **Author's GitHub**: [nlsandler/writing-a-c-compiler-tests](https://github.com/nlsandler/writing-a-c-compiler-tests)

## License

This project is for educational purposes, following the examples and exercises from "Writing a C Compiler" by No Starch Press.
