#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Configuration
VERBOSE=0
NO_VALGRIND=0
KEEP_OUTPUTS=0
TEST_NUM=1
TOTAL_PASSED=0
TOTAL_FAILED=0

# Temporary directories
TEST_DIR=$(mktemp -d)
OUT_DIR=$(mktemp -d)
mkdir -p "$OUT_DIR/failed"

# Test counter and timers
TOTAL_TESTS=0
START_TIME=$(date +%s)

# Cleanup handler
cleanup() {
    rm -rf "$TEST_DIR" "$OUT_DIR"
    [ -d "failed_tests" ] && rm -rf "failed_tests"
    if [ $KEEP_OUTPUTS -eq 0 ]; then
        rm -rf "$OUT_DIR"
    else
        echo -e "${YELLOW}Test outputs kept in: $OUT_DIR${NC}"
    fi
}
trap cleanup EXIT

# Memory check function
check_leaks() {
    local valgrind_out="$1"
    ! grep -q "definitely lost" "$valgrind_out" && \
    ! grep -q "indirectly lost" "$valgrind_out" && \
    grep -q "ERROR SUMMARY: 0 errors" "$valgrind_out"
}

# Test initialization
setup_test_files() {
    # Create test files structure
    mkdir -p "$TEST_DIR/inputs" "$TEST_DIR/perms"

    # Basic test files
    echo "Hello World" > "$TEST_DIR/inputs/basic"
    echo -e "Multiple\nLines\nOf\nText\nHere" > "$TEST_DIR/inputs/multiline"
    echo "" > "$TEST_DIR/inputs/empty"
    echo -e "Line1\nLine2\nLine2\nLine3\nLine3\nLine3" > "$TEST_DIR/inputs/duplicates"

    # Special content files
    echo -e "Special chars: $HOME $PATH \n * ? [ ] { }" > "$TEST_DIR/inputs/special_chars"
    echo -e "#include <stdio.h>\nint main() { return 0; }" > "$TEST_DIR/inputs/code"
    echo -e "tab\there\nspace there\n\nblank\rreturn" > "$TEST_DIR/inputs/whitespace"
    perl -E 'say "A" x 1000' > "$TEST_DIR/inputs/large"

    # Permission tests
    cp "$TEST_DIR/inputs/basic" "$TEST_DIR/perms/no_read"
    chmod 000 "$TEST_DIR/perms/no_read"
    cp "$TEST_DIR/inputs/basic" "$TEST_DIR/perms/no_write"
    chmod 333 "$TEST_DIR/perms/no_write"
    mkdir -p "$TEST_DIR/inputs/dir1/dir2"
    touch "$TEST_DIR/inputs/dir1/dir2/file"
    ln -sf "$TEST_DIR/inputs/basic" "$TEST_DIR/inputs/symlink"
}

print_header() {
    echo -e "\n${BLUE}»»»»»»»»»»»»»»» $1 «««««««««««««««${NC}"
}

run_test() {
    local test_name="$1"
    local cmd1="$2"
    local cmd2="$3"
    local infile="$4"
    local outfile1="$OUT_DIR/out1-$TEST_NUM"
    local outfile2="$OUT_DIR/out2-$TEST_NUM"
    local pipe_err="$OUT_DIR/pipe-err-$TEST_NUM"
    local pipex_err="$OUT_DIR/pipex-err-$TEST_NUM"
    local valgrind_out="$OUT_DIR/valgrind-$TEST_NUM"
    local test_passed=1
    local start=$(date +%s.%N)

    # Print test header
    echo -e "\n${CYAN}➤ Test #$TEST_NUM: ${test_name}${NC}"
    echo -e "   Input: ${YELLOW}$infile${NC}"
    echo -e "   Command 1: ${YELLOW}${cmd1}${NC}"
    echo -e "   Command 2: ${YELLOW}${cmd2}${NC}"

    # Run original pipe
    bash -c "< \"$infile\" $cmd1 | $cmd2 > \"$outfile1\"" 2> "$pipe_err"
    local pipe_status=$?

    # Run pipex
    ./pipex "$infile" "$cmd1" "$cmd2" "$outfile2" 2> "$pipex_err"
    local pipex_status=$?

    # Exit code check
    if [ $pipe_status -ne $pipex_status ]; then
        echo -e "${RED}✗ Exit codes differ (Pipe: $pipe_status | Pipex: $pipex_status)${NC}"
        test_passed=0
    fi

    # Output comparison
    if ! diff -q "$outfile1" "$outfile2" > /dev/null; then
        echo -e "${RED}✗ Outputs differ${NC}"
        [ $VERBOSE -eq 1 ] && diff --color=always "$outfile1" "$outfile2"
        test_passed=0
    fi

    # Error output comparison
    if ! diff -q "$pipe_err" "$pipex_err" > /dev/null; then
        echo -e "${RED}✗ Error outputs differ${NC}"
        [ $VERBOSE -eq 1 ] && diff --color=always "$pipe_err" "$pipex_err"
        test_passed=0
    fi

    # Valgrind check
    if [ $NO_VALGRIND -eq 0 ]; then
        valgrind --leak-check=full --track-origins=yes --error-exitcode=1 \
                 ./pipex "$infile" "$cmd1" "$cmd2" "$valgrind_out" &> "$valgrind_out"

        if ! check_leaks "$valgrind_out"; then
            echo -e "${RED}✗ Memory issues detected${NC}"
            [ $VERBOSE -eq 1 ] && grep -A5 -B2 -i "ERROR SUMMARY" "$valgrind_out"
            test_passed=0
        fi
    fi

    # Update counters
    if [ $test_passed -eq 1 ]; then
        echo -e "${GREEN}✓ Test passed${NC}"
        TOTAL_PASSED=$((TOTAL_PASSED + 1))
        rm -f "$outfile1" "$outfile2" "$pipe_err" "$pipex_err" "$valgrind_out"
    else
        echo -e "${RED}✗ Test failed${NC}"
        TOTAL_FAILED=$((TOTAL_FAILED + 1))
        # Save failed test files
        mkdir -p "failed_tests/test_$TEST_NUM"
        cp "$outfile1" "failed_tests/test_$TEST_NUM/pipe_out"
        cp "$outfile2" "failed_tests/test_$TEST_NUM/pipex_out"
        cp "$pipe_err" "failed_tests/test_$TEST_NUM/pipe_err"
        cp "$pipex_err" "failed_tests/test_$TEST_NUM/pipex_err"
        [ $NO_VALGRIND -eq 0 ] && cp "$valgrind_out" "failed_tests/test_$TEST_NUM/valgrind"
    fi

    # Print test duration
    local duration=$(printf "%.2f" $(echo "$(date +%s.%N) - $start" | bc))
    echo -e "${CYAN}Test duration: ${duration}s${NC}"

    TEST_NUM=$((TEST_NUM + 1))
}

print_summary() {
    local total=$((TOTAL_PASSED + TOTAL_FAILED))
    local elapsed=$(($(date +%s) - START_TIME))
    local pass_percent=$((TOTAL_PASSED * 100 / total))
    local fail_percent=$((TOTAL_FAILED * 100 / total))

    echo -e "\n${BLUE}═══════════════════════════════════════════${NC}"
    echo -e "${CYAN} TEST SUMMARY${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════${NC}"
    echo -e "${GREEN}PASSED: $TOTAL_PASSED/$total ($pass_percent%)${NC}"
    echo -e "${RED}FAILED: $TOTAL_FAILED/$total ($fail_percent%)${NC}"
    echo -e "TOTAL TIME: ${elapsed}s"

    if [ $TOTAL_FAILED -eq 0 ]; then
        echo -e "\n${GREEN}🎉 All tests passed successfully!${NC}"
    else
        echo -e "\n${RED}💥 Some tests failed. Check failed_tests/ directory for details.${NC}"
    fi
}

# Parse command line options
while [[ $# -gt 0 ]]; do
    case "$1" in
        -v|--verbose) VERBOSE=1 ;;
        --no-valgrind) NO_VALGRIND=1 ;;
        --keep) KEEP_OUTPUTS=1 ;;
        *) echo "Unknown option: $1"; exit 1 ;;
    esac
    shift
done

# Setup test environment
setup_test_files

# Run tests
print_header "BASIC FUNCTIONALITY TESTS"
run_test "Basic cat and grep" "cat" "grep Hello" "$TEST_DIR/inputs/basic"
run_test "Line count with wc" "cat" "wc -l" "$TEST_DIR/inputs/multiline"
run_test "Sort and uniq" "sort" "uniq" "$TEST_DIR/inputs/duplicates"

print_header "EDGE CASES"
run_test "Empty input file" "cat" "wc -l" "$TEST_DIR/inputs/empty"
run_test "Non-existent input" "cat" "wc -l" "$TEST_DIR/inputs/nonexistent"

print_header "PERMISSION TESTS"
run_test "No read permission" "cat" "wc -l" "$TEST_DIR/perms/no_read"
run_test "Directory input" "cat" "wc -l" "$TEST_DIR/inputs/dir1"

print_header "SPECIAL CASES"
run_test "Special characters" "cat" "grep '*'" "$TEST_DIR/inputs/special_chars"
run_test "Whitespace handling" "cat -e" "grep tab" "$TEST_DIR/inputs/whitespace"
run_test "Large file handling" "cat" "sort" "$TEST_DIR/inputs/large"

# Show summary
print_summary
