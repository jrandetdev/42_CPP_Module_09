// #include <vector>
// #include <iostream>

// int	getMinIndex(int a, int b)
// {
// 	return (a < b ? a : b);
// }

// void printVector(const std::vector<int> &seq) {
// 	for (std::vector<int>::const_iterator it = seq.begin(); it < seq.end(); it++)
// 	{
// 		std::cout << *it;
// 		if (it < seq.end() - 1) {
// 			std::cout << " ";
// 		}
// 	}
// 	std::cout << std::endl;
// }

// std::vector<int> generateJacobIndexes(size_t size) {
// 	std::vector<int> indexes(size);
// 	const int jacobSequence[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203};
// 	int jacobIndex = 1;
// 	int index = 0;
// 	// taille des groupes c'est number - previous number
// 	while (index < size)
// 	{
// 		int number = getMinIndex(jacobSequence[jacobIndex] - 2, static_cast<int>(size - 1));
// 		int previousNumber = jacobSequence[jacobIndex - 1] - 2;
// 		while (number > previousNumber)
// 		{
// 			indexes[number] = index; // 0
// 			index++; // 1
// 			number--;
// 		}
// 		jacobIndex++;
// 	}
// 	std::cout << "The sequence of indexes obtained:" << std::endl;
// 	printVector(indexes);
// 	return indexes;
// }


// int main() {


// 	std::vector<int> seq;
// 	std::vector<int> jacob;

// 	for (size_t i = 0; i < 42; i++)
// 	{
// 		seq.push_back(i);
// 	}
	

// 	std::cout << "INITIAL SEQUENCE:" << std::endl;
// 	printVector(seq);


// 	std::vector<int> jacobIndexes = generateJacobIndexes(seq.size());
// 	for (size_t i = 0; i < seq.size(); i++)
// 	{
// 		jacob.push_back(seq[jacobIndexes[i]]);
// 	}
	
// 	std::cout << "JACOB SEQUENCE ATTENDU:" << std::endl;
// 	std::cout << "1 0 3 2 9 8 7 6 5 4 10" << std::endl;
	
// 	std::cout << "JACOB SEQUENCE OBTENUE:" << std::endl;
// 	printVector(jacob);
// }
