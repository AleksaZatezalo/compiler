# Testing Guide

## Quick Start

```bash
# Build and run all tests
make test-all

# Build and run with full output
make test-verbose

# Build and run single test
make test
```

## Test Commands

| Command | Description |
|---------|-------------|
| `make test` | Run single test with `return_2.c` |
| `make test-all` | Run all tests in `tests/` directory |
| `make test-verbose` | Run all tests showing full token/AST output |
| `./run_tests.sh` | Run test script directly |

## Manual Testing

```bash
# Build the parser
make

# Test a specific file
./parser tests/return_0.c
./parser tests/return_42.c
./parser return_2.c
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
Shows the full lexer and parser output:
```
=== Tokens ===
Token: type=0, value='int'
Token: type=1, value='main'
...

=== AST ===
Program
  Function: main
    Return Statement
      Int Literal: 42
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
   ./parser tests/failing_test.c
   ```

3. Check the parser output and error messages

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
