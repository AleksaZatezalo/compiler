# Compiler Architecture

## Overview

This is a four-stage compiler that translates C source code to executable binaries:

```
Source Code (.c) → Lexer → Tokens → Parser → AST → Code Generator → Assembly → Assembler → Executable
```

The compiler outputs executables directly (default: `a.out`).

## Components

### 1. Lexer (`lexer.h`, `lexer.c`)

**Purpose**: Converts source code into tokens (lexical analysis)

**Key Functions**:
- `Token* lex(char **input)` - Tokenizes one token from input
- `Token** tokenize(char *source, int *count)` - Tokenizes entire source
- `print_tokens()` - Debug output
- `free_tokens()` - Memory cleanup

**Token Types**:
- Keywords: `int`, `main`, `void`, `return`
- Symbols: `(`, `)`, `{`, `}`, `;`
- Literals: Integer numbers
- Special: `TOKEN_EOF`

**Example**:
```c
Input:  "int main(void) { return 2; }"
Output: [TOKEN_INT] [TOKEN_MAIN] [TOKEN_LPAREN] [TOKEN_VOID] ...
```

---

### 2. Parser (`parser.h`, `parser.c`)

**Purpose**: Converts tokens into Abstract Syntax Tree (syntax analysis)

**Key Functions**:
- `ASTNode* parse_program(Token **tokens)` - Entry point, parses entire program
- `ASTNode* parse_function()` - Parses function definition
- `ASTNode* parse_statement()` - Parses statements (return)
- `ASTNode* parse_expression()` - Parses expressions (integers)
- `void free_ast(ASTNode *)` - Memory cleanup
- `void print_ast(ASTNode *, int)` - Debug output

**AST Node Types**:
- `NODE_PROGRAM` - Root node, contains function
- `NODE_FUNCTION` - Function definition with name and body
- `NODE_STATEMENT_RETURN` - Return statement with expression
- `NODE_EXPRESSION_INT` - Integer literal value

**Example**:
```
Input Tokens: [int][main][(][void][)]...
Output AST:
  Program
    Function: main
      Return Statement
        Int Literal: 2
```

---

### 3. Code Generator (`codegen.h`, `codegen.c`)

**Purpose**: Converts AST into x86-64 assembly code

**Key Functions**:
- `char* generate_code(ASTNode *ast)` - Generates assembly from AST
- `void write_assembly(const char *code, const char *filename)` - Writes to file

**Generated Assembly Structure**:
```asm
    .section .text          # Code section directive
    .globl main             # Make main visible to linker
main:                       # Function label
    pushq   %rbp            # Save base pointer
    movq    %rsp, %rbp      # Set up stack frame
    movl    $<value>, %eax  # Put return value in %eax
    popq    %rbp            # Restore base pointer
    ret                     # Return from function
```

**Register Usage**:
- `%eax` - Return value (32-bit)
- `%rbp` - Base pointer (stack frame)
- `%rsp` - Stack pointer

---

### 4. Assembler (`assembler.h`, `assembler.c`)

**Purpose**: Converts assembly code to executable binary

**Key Functions**:
- `int assemble(const char *assembly_code, const char *output_file)` - Assembles to executable

**How it works (WITHOUT using GCC)**:

**Step 1: Assemble** with `as` (GNU assembler)
1. Writes assembly to `/tmp/compiler_temp.s`
2. Forks and executes: `as /tmp/compiler_temp.s -o /tmp/compiler_temp.o`
3. Creates object file (`.o`)

**Step 2: Link** with `ld` (GNU linker)
1. Forks and executes: `ld` with arguments:
   - `-dynamic-linker /lib64/ld-linux-x86-64.so.2` - Runtime linker
   - `/usr/lib/x86_64-linux-gnu/crt1.o` - C runtime startup
   - `/usr/lib/x86_64-linux-gnu/crti.o` - Constructor prologue
   - `/tmp/compiler_temp.o` - Our compiled code
   - `/usr/lib/x86_64-linux-gnu/crtn.o` - Constructor epilogue
   - `-lc` - Link C standard library
   - `-o a.out` - Output executable
2. Creates final executable

**Step 3: Cleanup**
- Removes temporary `.s` and `.o` files

**Why use `as`/`ld` instead of `gcc`**:
- More control over linking process
- Educational - shows what GCC does behind the scenes
- No dependency on GCC for final assembly
- Direct use of GNU binutils (as, ld)

**Why use fork/exec instead of system()**:
- Better error handling (check exact exit codes)
- More secure (no shell interpretation)
- Direct process control
- Proper architecture separation

---

### 5. Main Compiler (`compiler.c`)

**Purpose**: Orchestrates the entire compilation pipeline

**Workflow**:
1. Read source file → string
2. Lexical analysis → tokens
3. Parsing → AST
4. Code generation → assembly string
5. Print assembly to stdout
6. Assemble to executable → a.out
7. Clean up memory

**Usage**:
```bash
./compiler <source> <dest>
# Example: ./compiler input.c output
```

---

## Build System (`Makefile`)

### Targets

| Target | Description |
|--------|-------------|
| `make` or `make all` | Build the compiler |
| `make parser` | Build standalone parser (for debugging) |
| `make test` | Compile and show assembly for return_2.c |
| `make test-all` | Run all tests |
| `make test-verbose` | Run all tests with full output |
| `make clean` | Remove build artifacts |
| `make rebuild` | Clean and rebuild |

### Build Process

```
lexer.c     → (compile) → lexer.o     ┐
parser.c    → (compile) → parser.o    │
codegen.c   → (compile) → codegen.o   ├→ (link) → compiler
assembler.c → (compile) → assembler.o │
compiler.c  → (compile) → compiler.o  ┘
```

---

## Data Flow Example

### Input: `return_2.c`
```c
int main(void) {
    return 2;
}
```

### Stage 1: Lexer Output
```
TOKEN_INT("int")
TOKEN_MAIN("main")
TOKEN_LPAREN("(")
TOKEN_VOID("void")
TOKEN_RPAREN(")")
TOKEN_LBRACE("{")
TOKEN_RETURN("return")
TOKEN_NUMBER("2")
TOKEN_SEMICOLON(";")
TOKEN_RBRACE("}")
TOKEN_EOF
```

### Stage 2: Parser Output (AST)
```
Program
└── Function: "main"
    └── Return Statement
        └── Int Literal: 2
```

### Stage 3: Code Generator Output
```asm
    .section .text
    .globl main
main:
    pushq   %rbp
    movq    %rsp, %rbp
    movl    $2, %eax
    popq    %rbp
    ret
```

### Stage 4: Assembler Output
```bash
# Assembler automatically creates executable
./a.out
echo $?  # Outputs: 2
```

---

## Memory Management

Each component is responsible for cleaning up its allocations:

- **Lexer**: Allocates tokens and their values → `free_tokens()`
- **Parser**: Allocates AST nodes and strings → `free_ast()`
- **Code Generator**: Allocates assembly string → caller must `free()`
- **Main**: Allocates source file buffer → `free(source)`

---

## Testing

### Test Suite (`tests/`)
- `return_0.c` - Returns 0
- `return_42.c` - Returns 42
- `return_100.c` - Returns 100

### Test Script (`run_tests.sh`)
Automatically:
1. Compiles each test file
2. Verifies assembly file is created
3. Reports pass/fail
4. Cleans up artifacts

### Manual Verification
```bash
./compiler tests/return_42.c tests/return_42.s
gcc tests/return_42.s -o test
./test
echo $?  # Should output 42
```

---

## Future Extensions

Following the "Writing a C Compiler" book chapters:

1. **Unary operators** (`-`, `~`, `!`)
2. **Binary operators** (`+`, `-`, `*`, `/`, `%`)
3. **Logical operators** (`&&`, `||`)
4. **Control flow** (`if`, `while`, `for`)
5. **Local variables** and scope
6. **Function calls** and parameters
7. **Pointers** and arrays
8. **Preprocessor** directives

Each addition will require updates to all three stages: lexer, parser, and code generator.
