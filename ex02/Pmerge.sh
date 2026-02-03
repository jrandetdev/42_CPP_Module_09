#!/bin/bash

make re

echo "Test 1: Valid test"
./PmergeMe 1 2 3
echo "Test 1: Valid test"
echo ""

echo "Test 2: sequence with a letter"
./PmergeMe a 2 3
echo ""

echo "Test 3: sequence with a negative number"
./PmergeMe -3 2 3
echo ""

echo "Test 4: sequence with a very big number"
./PmergeMe 1 54455456454545645645646 3
echo ""

