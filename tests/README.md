# Test Suite

This directory contains test cases for the C compiler.

## Current Tests

- `return_0.c` - Returns 0
- `return_42.c` - Returns 42
- `return_100.c` - Returns 100

## Running Tests

From the project root directory:

```bash
# Run all tests
make test-all

# Run all tests with verbose output
make test-verbose

# Run single test
make test
```

## Adding New Tests

To add a new test case:

1. Create a new `.c` file in this directory
2. The test will automatically be picked up by `make test-all`

## Test File Format

Currently supports simple return statements:

```c
int main(void) {
    return <number>;
}
```

More complex features will be added as the compiler develops.
