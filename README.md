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

```bash
# Parse a C source file
./parser <input.c>

# Example
./parser return_2.c
```

## Project Structure

```
compiler/
├── lexer.h         # Lexer declarations
├── lexer.c         # Lexer implementation
├── parser.c        # Parser and main program
├── Makefile        # Build system
├── tests/          # Test cases
│   ├── return_0.c
│   ├── return_42.c
│   └── return_100.c
├── return_2.c      # Example test file
└── README.md       # This file
```

## Current Status

### ✅ Completed
- **Lexical Analysis** - Tokenizes C source code
- **Syntax Analysis** - Builds Abstract Syntax Tree (AST)
- **Test Suite** - Automated testing framework

### ⏳ In Progress
- Code generation (next step)

### 📋 Planned
- Semantic analysis
- Optimization
- Full C language support

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

## References

- **Book**: [Writing a C Compiler](https://nostarch.com/writing-c-compiler) by Nora Sandler
- **Author's GitHub**: [nlsandler/writing-a-c-compiler-tests](https://github.com/nlsandler/writing-a-c-compiler-tests)

## License

This project is for educational purposes, following the examples and exercises from "Writing a C Compiler" by No Starch Press.
