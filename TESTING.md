# Testing Guide

## Quick Start

```bash
# Build the compiler
make

# Run all tests
make test-all

# Run single test with output
make test

# Run all tests with full output
make test-verbose
```

## Test Commands

| Command | Description |
|---------|-------------|
| `make test` | Compile `return_2.c` and show generated assembly |
| `make test-all` | Compile all tests in `tests/` directory |
| `make test-verbose` | Compile all tests showing full output |
| `./run_tests.sh` | Run test script directly |

## Manual Testing

```bash
# Build the compiler
make

# Compile a specific file
./compiler tests/return_0.c tests/return_0.s
./compiler tests/return_42.c tests/return_42.s
./compiler return_2.c return_2.s

# Assemble and run to verify
gcc tests/return_42.s -o test_42
./test_42
echo $?  # Should print 42
```

## Understanding Test Output

### Normal Output (test-all)
```
======================================
Running Parser Tests
======================================

Testing return_0 ... PASS
Testing return_42 ... PASS
Testing return_100 ... PASS

======================================
Test Results
======================================
Total:  3
Passed: 3
Failed: 0

All tests passed!
```

### Verbose Output (test-verbose)
Shows the generated assembly code:
```
=== Compiling tests/return_42.c ===
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

Writing to file: tests/return_42.s
Success! Assembly written to tests/return_42.s
```

## Adding New Tests

1. Create a new `.c` file in `tests/` directory:
   ```bash
   echo 'int main(void) { return 255; }' > tests/return_255.c
   ```

2. Run tests - new file is automatically detected:
   ```bash
   make test-all
   ```

## Test File Requirements

Currently, test files must follow this format:

```c
int main(void) {
    return <number>;
}
```

Where `<number>` is any integer literal.

## Debugging Failed Tests

If a test fails:

1. Run it in verbose mode to see output:
   ```bash
   make test-verbose
   ```

2. Run it directly to see errors:
   ```bash
   ./compiler tests/failing_test.c tests/failing_test.s
   ```

3. Check the compiler output and error messages

4. Try assembling the output to see if it's valid:
   ```bash
   gcc tests/failing_test.s -o test_program
   ```

## CI/CD Integration

The test suite can be integrated into CI/CD pipelines:

```bash
# Exit code 0 if all tests pass, 1 if any fail
./run_tests.sh
echo $?  # Check exit code
```

## Test Coverage

Current tests cover:
- ✅ Simple return statements
- ✅ Different integer values (0, 2, 42, 100)
- ✅ Whitespace variations

Future tests will cover:
- ⏳ Arithmetic expressions
- ⏳ Variables
- ⏳ Multiple statements
- ⏳ Function calls
- ⏳ Control flow
