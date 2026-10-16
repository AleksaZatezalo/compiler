# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Targets
COMPILER = compiler
PARSER = parser

# Source files
COMPILER_SOURCES = compiler.c lexer.c parser.c codegen.c assembler.c
PARSER_SOURCES = parser.c lexer.c
HEADERS = lexer.h parser.h codegen.h assembler.h

# Object files
COMPILER_OBJECTS = $(COMPILER_SOURCES:.c=.o)
PARSER_OBJECTS = $(PARSER_SOURCES:.c=.o)

# Default target - build compiler
all: $(COMPILER)

# Build the compiler
$(COMPILER): $(COMPILER_OBJECTS)
	$(CC) $(CFLAGS) -o $(COMPILER) $(COMPILER_OBJECTS)

# Build parser standalone (for testing)
$(PARSER): $(PARSER_OBJECTS)
	$(CC) $(CFLAGS) -DPARSER_STANDALONE -o $(PARSER) $(PARSER_SOURCES)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run compiler on single test
test: $(COMPILER)
	@echo "Compiling return_2.c..."
	@./$(COMPILER) return_2.c
	@echo ""
	@echo "Running executable..."
	@./a.out; echo "Exit code: $$?"

# Run all tests in test suite
test-all: $(COMPILER)
	@./run_tests.sh

# Run tests with verbose output
test-verbose: $(COMPILER)
	@for test in tests/*.c; do \
		echo "=== Compiling $$test ==="; \
		./$(COMPILER) $$test; \
		echo ""; \
	done

# Clean build artifacts
clean:
	rm -f $(COMPILER) $(PARSER) $(COMPILER_OBJECTS) *.s tests/*.s a.out

# Clean everything including test outputs
distclean: clean
	rm -f *.asm *.out tests/*.asm tests/*.out

# Rebuild from scratch
rebuild: clean all

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build the compiler (default)"
	@echo "  parser       - Build standalone parser for testing"
	@echo "  test         - Compile and run return_2.c"
	@echo "  test-all     - Run all tests in tests/"
	@echo "  test-verbose - Run all tests with full output"
	@echo "  clean        - Remove compiled files and executables"
	@echo "  distclean    - Remove all generated files"
	@echo "  rebuild      - Clean and rebuild from scratch"
	@echo "  help         - Show this help message"

.PHONY: all test test-all test-verbose clean distclean rebuild help
