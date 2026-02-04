#include <iostream>
#include <vector>
#include "PmergeMe.hpp"

#ifndef OUTPUT_HPP
# define OUTPUT_HPP

std::ostream &operator<<(std::ostream& outstream, std::vector<int> vecContainer);
std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs);

#endif
