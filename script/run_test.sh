#!/bin/bash
# run_test.sh

# Run the program and compare its output with the expected output
"$1" < "$2" | diff - "$3"