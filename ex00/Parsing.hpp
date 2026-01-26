#ifndef PARSING_HPP
# define PARSING_HPP

#define RESET   "\033[0m"
#define RED     "\033[31m"

#include <iostream>

bool	isValidExtension(const std::string& filename, const std::string& extension);
bool	isFileOpen(std::ifstream& file);
bool	isValidFirstLine(std::string& line, const std::string& firstLine);
bool	isValidValue(float value);
bool	isValidDate(int year, int month, int day);
bool	isLeap(int year);
float	getFloat(std::string& value);

#endif
