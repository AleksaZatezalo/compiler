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
# Build instructions will be added as the project develops
make
```

## Usage

```bash
# Compile a C source file
./compiler <input.c> -o <output>
```

## Project Structure

```
compiler/
├── src/           # Source code
├── tests/         # Test cases
└── examples/      # Example C programs
```

## Progress

This compiler is being developed chapter-by-chapter following the book. Feature support is added incrementally as each chapter is completed.

## Requirements

- A C compiler (GCC or Clang) for bootstrapping
- GNU Make
- x86-64 Linux or macOS system for assembly output

## Testing

```bash
# Test suite will be added as development progresses
make test
```

## References

- **Book**: [Writing a C Compiler](https://nostarch.com/writing-c-compiler) by Nora Sandler
- **Author's GitHub**: [nlsandler/writing-a-c-compiler-tests](https://github.com/nlsandler/writing-a-c-compiler-tests)

## License

This project is for educational purposes, following the examples and exercises from "Writing a C Compiler" by No Starch Press.
