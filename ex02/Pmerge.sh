#!/bin/bash

make re

echo "====== TESTING PARSING ======="
echo ""

echo "Test 1: valid input"
./PmergeMe 11 2 17 0 16 8 6 15 10 3 21 1 18 9 14 19
echo ""

echo "Test 1: valid input"
./PmergeMe 11 2 17 0 16 8 6 15 10 3 21 1 18 9 14 19
echo ""

echo "Test 2: invalid sequence with a letter"
./PmergeMe a 2 3
echo ""

echo "Test 3: invalid sequence with a negative number"
./PmergeMe -3 2 3
echo ""

echo "Test 4: invalid sequence with a very big number"
./PmergeMe 1 54455456454545645645646 3
echo ""

echo "Test 4: invalid with no arguments"
./PmergeMe
echo ""

echo "Test 4: invalid with only one argument"
./PmergeMe 1
echo ""

