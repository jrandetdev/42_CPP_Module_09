#!/bin/bash

make re

echo "Given test in subject: 8 9 * 9 - 9 - 9 - 4 - 1 +, expected result 42"
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
echo ""

echo "7 7 * 7 -, expected result 42"
./RPN "7 7 * 7 -"
echo ""

echo "1 2 * 2 / 2 * 2 4 - +, expected result 0"
./RPN "1 2 * 2 / 2 * 2 4 - +"
echo ""

echo "(1 + 1), expected result: Error, invalid char that does not belong to the following: 0123456789+*-/ "
./RPN "(1 + 1)"
echo ""

echo "1 2 -, expected -1"
./RPN "1 2 -"
echo ""

echo "1 2 /, expected 0"
./RPN "1 2 /"
echo ""

echo "5 0 /, expected Error: division by zero is not possible."
./RPN "5 0 /"
echo ""

echo "8    9 * 9 - 9 - 9 - 4 - 1 +, Expected: 42"
./RPN "8    9 * 9 - 9 - 9 - 4 - 1 +"
echo ""

echo "3 4 + a, expected result: Error, invalid char that does not belong to the following: 0123456789+*-/ "
./RPN "3 4 + a"
echo ""

echo " 7    8  +   , expected result: 15"
./RPN " 7    8  +   "
echo ""

echo "2 2 2 +, expected result: Error: invalid expression remains that cannot be calculated."
./RPN "2 2 2 +"
echo ""


echo "2 + 2, expected result: Error: invalid expression remains that cannot be calculated."
./RPN "2 + 2"
echo ""