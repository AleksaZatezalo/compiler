# Quick Start Guide

## TL;DR

```bash
make                                    # Build compiler
./compiler test.c test                  # Compile C → executable
./test && echo "Exit code: $?"          # Run
```

## 5-Minute Tutorial

### 1. Build the Compiler

```bash
make
```

You should see:
```
gcc -c compiler.c -o compiler.o
gcc -c lexer.c -o lexer.o
gcc -c parser.c -o parser.o
gcc -c codegen.c -o codegen.o
gcc -o compiler compiler.o lexer.o parser.o codegen.o
```

### 2. Create a Test Program

```bash
cat > hello.c << 'EOF'
int main(void) {
    return 42;
}
EOF
```

### 3. Compile It

```bash
./compiler hello.c hello
```

You'll see:
```
=== Generated Assembly ===
    .section .text
    .globl main
main:
    pushq   %rbp
    movq    %rsp, %rbp
    movl    $42, %eax
    popq    %rbp
    ret
==========================

Assembling to executable...

✓ Success! Executable created: hello
Run it with: hello
```

### 4. Run It

```bash
./hello
echo $?
```

Output: `42` ✅

---

## What Just Happened?

```
hello.c          →  hello.s         →  hello (executable)
(C source)          (assembly)         (machine code)

int main() {        .globl main        01001000 01010011...
  return 42;        movl $42, %eax     (binary)
}                   ret
```

Our compiler did: `hello.c → hello.s`  
GCC did: `hello.s → hello`

---

## Run the Test Suite

```bash
make test-all
```

Expected output:
```
======================================
Running Parser Tests
======================================

Testing return_0 ... PASS
Testing return_100 ... PASS
Testing return_42 ... PASS

======================================
Test Results
======================================
Total:  3
Passed: 3
Failed: 0

All tests passed!
```

---

## Try Different Values

```bash
# Return 0
echo 'int main(void) { return 0; }' > zero.c
./compiler zero.c zero && ./zero; echo $?
# Output: 0

# Return 255
echo 'int main(void) { return 255; }' > max.c
./compiler max.c max && ./max; echo $?
# Output: 255
```

---

## Makefile Commands

| Command | What It Does |
|---------|--------------|
| `make` | Build the compiler |
| `make test` | Compile return_2.c and show assembly |
| `make test-all` | Run all tests |
| `make clean` | Remove build files |
| `make help` | Show all commands |

---

## Project Structure

```
compiler/           # Your current directory
├── compiler        # Executable (built by make)
├── compiler.c      # Main program
├── lexer.c/h       # Tokenizer
├── parser.c/h      # AST builder
├── codegen.c/h     # Assembly generator
├── Makefile        # Build system
└── tests/          # Test files
```

---

## Common Issues

### "command not found: ./compiler"

**Problem:** Compiler not built  
**Solution:** Run `make` first

### "Permission denied: ./hello"

**Problem:** Trying to run assembly file  
**Solution:** Assemble it first with `gcc hello.s -o hello`

### "No such file or directory"

**Problem:** Wrong file path  
**Solution:** Use `ls` to check files exist

---

## Next Steps

1. **Read the docs:**
   - `README.md` - Project overview
   - `USAGE.md` - Detailed usage guide
   - `ARCHITECTURE.md` - How it works
   - `TESTING.md` - Testing guide

2. **Explore the code:**
   ```bash
   cat lexer.c      # See how tokenization works
   cat parser.c     # See how parsing works
   cat codegen.c    # See how code generation works
   ```

3. **Follow the book:**
   This implements Chapter 1 of "Writing a C Compiler"  
   Continue to Chapter 2 for unary operators!

---

## Getting Help

```bash
# See all make commands
make help

# View a source file
cat compiler.c

# Check what files exist
ls -lh

# Test if compiler works
make test-all
```

---

## Clean Up

```bash
# Remove build files and assembly
make clean

# Remove everything generated
make distclean

# Rebuild from scratch
make rebuild
```

---

**You're ready to go! Try compiling something!** 🚀
