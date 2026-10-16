#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "======================================"
echo "Running Parser Tests"
echo "======================================"
echo ""

# Check if compiler exists
if [ ! -f "./compiler" ]; then
    echo -e "${RED}Error: compiler executable not found${NC}"
    echo "Run 'make' first to build the compiler"
    exit 1
fi

# Counter for test results
PASSED=0
FAILED=0
TOTAL=0

# Function to run a single test
run_test() {
    local test_file=$1
    local test_name=$(basename "$test_file" .c)
    local output_file="${test_file%.c}.s"
    
    TOTAL=$((TOTAL + 1))
    
    echo -n "Testing $test_name ... "
    
    # Run the compiler on the test file
    if ./compiler "$test_file" "$output_file" > /dev/null 2>&1; then
        # Check if assembly file was created
        if [ -f "$output_file" ]; then
            echo -e "${GREEN}PASS${NC}"
            PASSED=$((PASSED + 1))
            rm -f "$output_file"  # Clean up
        else
            echo -e "${RED}FAIL (no output)${NC}"
            FAILED=$((FAILED + 1))
        fi
    else
        echo -e "${RED}FAIL${NC}"
        FAILED=$((FAILED + 1))
    fi
}

# Run all tests in the tests directory
if [ -d "tests" ]; then
    for test_file in tests/*.c; do
        if [ -f "$test_file" ]; then
            run_test "$test_file"
        fi
    done
else
    echo -e "${YELLOW}Warning: tests/ directory not found${NC}"
    echo "Testing with return_2.c only"
    run_test "return_2.c"
fi

echo ""
echo "======================================"
echo "Test Results"
echo "======================================"
echo -e "Total:  $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed${NC}"
    exit 1
fi
