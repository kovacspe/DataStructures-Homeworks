#include <iostream>
#include "RangeTree.h"

int main(int argc, char ** argv)
{

	double alpha = 0.7;
	if (argc == 2) alpha = std::stod(argv[1]);
	size_t size;
	RangeTree * rangeTree=nullptr;
	while (!std::cin.eof())
	{

		char command;
		std::cin >> command;
		if (std::cin.eof()) break;
		switch (command)
		{
		case '#': // create new heap
				  // write statistics from previous heap
			if (rangeTree != nullptr)
			{
				std::cout << size << ";" << rangeTree->GetAverageInsertOperations() << ";" << rangeTree->GetAverageRangCountOperations() << ";" <<
					rangeTree->GetMaxInsertOperations() << ";" << rangeTree->GetMaxRangeCountOperations() << std::endl;
				
				delete rangeTree;
			}

			std::cin >> size;

			// Initialize new heap
			rangeTree = new RangeTree(alpha);
	
			

			break;

		case 'I': // Insert
			size_t x, y;
			std::cin >> x >> y;
			rangeTree->Insert(x, y);
			break;
		case 'C': // Insert element
			size_t x1, x2, y1, y2;
			std::cin >> x1 >> y1 >> x2 >> y2;
			rangeTree->RangeCount(x1, y1, x2, y2);
			break;
		case 'P':
			rangeTree->Print(false);
			break;
		case 'A':
			rangeTree->Print(true);
			break;
		
		}
	}

	std::cout << size << ";" << rangeTree->GetAverageInsertOperations() << ";" << rangeTree->GetAverageRangCountOperations() << ";" <<
		rangeTree->GetMaxInsertOperations() << ";" << rangeTree->GetMaxRangeCountOperations() << std::endl;
	delete rangeTree;




	return 0;
}