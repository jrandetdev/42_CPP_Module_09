#!/bin/bash

testdir="./tests"

make re

# Check if directory exists
if [ ! -d "$testdir" ]; then
    echo "Error: $testdir not found."
    exit 1
fi

# Iterate through files
for file in "$testdir"/*; do
    # Check if it's a regular file
    if [ -f "$file" ]; then
        echo "Running: $file"
        ./btc "$file"
		echo ""
    fi
done