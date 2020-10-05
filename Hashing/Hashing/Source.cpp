#include "HashTable.h"
#include <iostream>


int main(int argc, char ** argv)
{
	size_t prime = 27644437;
	size_t bits;
	size_t func;
	if (argc != 3) return 0;
	else {
		func = std::stoi(argv[1]);
		bits = std::stoi(argv[2]);
	}
	HashTable hashTable(prime, func,bits);
	std::string s;

	while (std::cin.good())
	{
		std::cin >> s;
		hashTable.Insert(s);

	}
	std::cout << func << ";" << bits << ";" << hashTable.GetNumberOfWords() << ";" << hashTable.GetNumbefOfOnesInBitArray() / (double) bits<<std::endl;



	
	
}