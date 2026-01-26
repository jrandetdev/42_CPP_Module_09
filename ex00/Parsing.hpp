#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>

bool	isValidExtension(const std::string& filename, const std::string& extension);
bool	isFileOpen(std::ifstream& file);
bool	isValidFirstLine(std::string& line, const std::string& firstLine);
bool	isValidValue(float value);
bool	isValidDate(const std::string& date);
bool	isLeap(int year);
float	getFloat(std::string& value);

#endif
