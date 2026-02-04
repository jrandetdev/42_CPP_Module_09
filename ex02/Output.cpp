#include "Output.hpp"

//====================== OUTPUT OPERATORS ========================

std::ostream &operator<<(std::ostream& outstream, std::vector<int> vectorContainer)
{
	outstream << "Before:	";
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		outstream << vectorContainer[i];
		if (i < vectorContainer.size() - 1)
			outstream << " ";
	}

	return outstream;
}

//==================== DEBUG OUTPUT TO SEE PAIR ====================

std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs)
{

	std::vector<Pair *>::iterator it;

	outstream << '\n';
	for (it = pairs.begin(); it < pairs.end(); ++it)
	{
		std::string valStr = std::to_string((*it)->value);
		int totalWidth = 11;
		int padding = (totalWidth - valStr.length()) / 2;

		if ((*it)->left)
			outstream << '[' << std::setw(2) << (*it)->left; // Left part
		else
			outstream << '[' << std::setw(2) << "nullptr";
		outstream << std::string(padding, ' ') << valStr 
				<< std::string(totalWidth - padding - valStr.length(), ' ');

		if ((*it)->right)
			outstream << std::setw(2) << (*it)->right << '|';
		else
			outstream << std::setw(2) << "nullptr" << '|';

		if (it < pairs.end() - 1)
			outstream << "	";
	}
	return outstream;
}
