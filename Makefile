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

# Run tests
test: $(TARGET)
	./$(TARGET) return_2.c

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
	@echo "  all       - Build the parser (default)"
	@echo "  test      - Build and run test with return_2.c"
	@echo "  clean     - Remove compiled files"
	@echo "  distclean - Remove all generated files"
	@echo "  rebuild   - Clean and rebuild from scratch"
	@echo "  help      - Show this help message"

.PHONY: all test clean distclean rebuild help
