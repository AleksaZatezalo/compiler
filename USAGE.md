# Usage Guide

## Understanding the Compilation Process

Our compiler is **not** a complete C-to-executable compiler. It's a **C-to-assembly** compiler that implements the first stages of compilation:

```
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│  Source.c    │ ──> │  Assembly.s  │ ──> │  Executable  │
│              │     │              │     │              │
│ int main()   │     │ .section     │     │ Binary code  │
│ return 2;    │     │ movl $2, %eax│     │ (machine)    │
└──────────────┘     └──────────────┘     └──────────────┘
   Our compiler          GCC/as               GCC ld
   does this →            does this →
```

## Step-by-Step Usage

### 1. Compile C to Assembly

```bash
./compiler <input.c> <output.s>
```

**Example:**
```bash
./compiler return_2.c return_2.s
```

**Output:**
- Displays generated assembly to stdout
- Writes assembly to `output.s` file
- The `.s` file is **assembly source code**, not executable

### 2. Assemble to Executable

Use GCC to assemble and link:

```bash
gcc <output.s> -o <executable>
```

**Example:**
```bash
gcc return_2.s -o return_2
```

This creates an actual executable binary.

### 3. Run the Executable

```bash
./<executable>
echo $?  # Check exit code
```

**Example:**
```bash
./return_2
echo $?  # Should print: 2
```

---

## Complete Example

```bash
# Write a simple C program
cat > test.c << 'EOF'
int main(void) {
    return 42;
}
EOF

# Step 1: Compile to assembly
./compiler test.c test.s

# Step 2: Assemble to executable
gcc test.s -o test

# Step 3: Run it
./test
echo $?
# Output: 42
```

---

## Common Mistakes

### ❌ Trying to run assembly files directly

```bash
./compiler return_2.c hello    # Creates file named "hello" (assembly)
chmod +x hello
./hello                         # ERROR! This is assembly, not executable
```

**Why it fails:** The file contains assembly source code (text), not machine code (binary).

### ✅ Correct way

```bash
./compiler return_2.c hello.s   # Create assembly file
gcc hello.s -o hello            # Assemble to executable
./hello                         # Now it works!
```

---

## File Extensions Guide

| Extension | Type | Description | Can Execute? |
|-----------|------|-------------|--------------|
| `.c` | Source | C source code | ❌ No |
| `.s` | Assembly | x86-64 assembly text | ❌ No |
| (no ext) | Binary | Machine code executable | ✅ Yes |

**Best Practice:** Always use `.s` extension for assembly output:
```bash
./compiler input.c output.s    # Good
./compiler input.c output      # Confusing - looks like executable
```

---

## Quick Reference

### Build and Test in One Line

```bash
# Compile, assemble, and run
./compiler test.c test.s && gcc test.s -o test && ./test && echo "Exit: $?"
```

### Clean Up Assembly Files

```bash
make clean        # Removes *.s files
rm -f *.s         # Manual cleanup
```

### Test Suite

```bash
make test-all     # Tests all files, auto-cleans assembly
```

---

## What Our Compiler Does

✅ **Does:**
- Lexical analysis (tokenization)
- Syntax analysis (parsing)
- Code generation (assembly)
- Validates C syntax
- Generates valid x86-64 assembly

❌ **Does Not:**
- Assemble to machine code (use `gcc` or `as`)
- Link with libraries (use `gcc` or `ld`)
- Create executables directly
- Handle preprocessor directives (use `cpp`)
- Optimize code

---

## Integration with GCC

Our compiler can be used as a **front-end** to GCC:

```bash
# Our compiler: C → Assembly
./compiler source.c output.s

# GCC: Assembly → Executable
gcc output.s -o program

# Equivalent GCC one-liner (for comparison):
gcc source.c -o program
```

---

## Debugging Tips

### View Generated Assembly

```bash
./compiler test.c test.s
cat test.s              # View the assembly
```

### Compare with GCC's Assembly

```bash
# Our compiler's output
./compiler test.c our.s
cat our.s

# GCC's output (for comparison)
gcc -S test.c -o gcc.s
cat gcc.s
```

### Test if Assembly is Valid

```bash
# Try to assemble it
gcc -c test.s -o test.o    # Create object file
echo $?                     # 0 = success, non-zero = error

# Or assemble and link in one step
gcc test.s -o test
```

---

## Advanced: Manual Assembly

You can also use `as` (assembler) and `ld` (linker) separately:

```bash
# Assemble
as test.s -o test.o

# Link
ld test.o -o test -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2 -e main

# Run
./test
```

But using `gcc` is much simpler!
