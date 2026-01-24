#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>

struct InputData
{
	std::string date;
	float		value;
};

struct SeparatedTokens
{
	int		year;
	int		month;
	int		day;
	char	firstHyphen;
	char	secondHyphen;
	char	verticalBar;
};

bool	isValidFile(const std::string& filename, std::ifstream& file, const std::string& extension);
bool	isValidFirstLine(const std::string& filename, std::ifstream& file, const std::string& firstLine);
bool	isValidValue(float value);
bool	isValidDate(SeparatedTokens *elements);
void	extractDateKey(SeparatedTokens *elements, InputData *input);
bool	isLeap(int year);

#endif
