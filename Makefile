# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = parser

# Source files
SOURCES = parser.c lexer.c
HEADERS = lexer.h

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the parser
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run single test
test: $(TARGET)
	./$(TARGET) return_2.c

# Run all tests in test suite
test-all: $(TARGET)
	@./run_tests.sh

# Run tests with verbose output
test-verbose: $(TARGET)
	@for test in tests/*.c; do \
		echo "=== Testing $$test ==="; \
		./$(TARGET) $$test; \
		echo ""; \
	done

# Clean build artifacts
clean:
	rm -f $(TARGET) $(OBJECTS)

# Clean everything including test outputs
distclean: clean
	rm -f *.s *.asm *.out

# Rebuild from scratch
rebuild: clean all

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build the parser (default)"
	@echo "  test         - Build and run single test (return_2.c)"
	@echo "  test-all     - Build and run all tests in tests/"
	@echo "  test-verbose - Build and run all tests with full output"
	@echo "  clean        - Remove compiled files"
	@echo "  distclean    - Remove all generated files"
	@echo "  rebuild      - Clean and rebuild from scratch"
	@echo "  help         - Show this help message"

.PHONY: all test test-all test-verbose clean distclean rebuild help
