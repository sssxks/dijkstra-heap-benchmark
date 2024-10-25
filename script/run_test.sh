#!/bin/bash

# Variables
EXECUTABLE="./pathfinder"
INPUT_FILE="input.txt"
# EXPECTED_OUTPUT_FILE="expected_output.txt"
ACTUAL_OUTPUT_FILE="actual_output.txt"

# Run the executable with the input file and save the output
$EXECUTABLE $INPUT_FILE > $ACTUAL_OUTPUT_FILE

# Compare the actual output with the expected output
# if diff $ACTUAL_OUTPUT_FILE $EXPECTED_OUTPUT_FILE > /dev/null; then
#     echo "Test passed!"
# else
#     echo "Test failed!"
#     diff $ACTUAL_OUTPUT_FILE $EXPECTED_OUTPUT_FILE
# fi