#include "FibonacciHeap.h"
#include <iostream>
#include <string>
#include <stdio.h>
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif

/**/
void test(bool naive)
{
	// make IO operations faster
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	std::string label;
	if (naive) label = "NAIVE"; 
	else label = "NON-NAIVE";

	FibonacciHeap heap(0, naive);
	size_t MaxOpDecrease = 0;
	size_t numOfDecreases = 0;
	double averageOpDecrease = 0.0;
	size_t MaxOpExtractMin = 0;
	size_t numOfExtractMins = 0;
	double averageOpExtractMin = 0.0;
	size_t size = 0;
	size_t ops;
	size_t id, value;
	int x;
	while (!std::cin.eof())
	{

		char command;
		std::cin >> command;
		if (std::cin.eof()) break;
		switch (command)
		{
		case '#': // create new heap
			// write statistics from previous heap
			averageOpDecrease = averageOpDecrease / numOfDecreases;
			averageOpExtractMin = averageOpExtractMin / numOfExtractMins;
			std::cout << size << ";" << averageOpDecrease << ";" << averageOpExtractMin << ";" << MaxOpDecrease << ";" << MaxOpExtractMin<< std::endl;
			heap.clear();
			std::cin >> size;

			// Initialize new heap
			heap = FibonacciHeap(size, naive);
			// reset stats
			MaxOpDecrease = 0;
			numOfDecreases = 0;
			averageOpDecrease = 0.0;
			MaxOpExtractMin = 0;
			numOfExtractMins = 0;
			averageOpExtractMin = 0.0;
			
			break;
		case 'M': // Delete min
			ops = heap.ExtractMin();
			++numOfExtractMins;
			if (MaxOpExtractMin < ops) MaxOpExtractMin = ops;
			averageOpExtractMin += ops;
			break;
		case 'D': // Decrease

			std::cin >> id >> value;
			x = heap.Decrease(id, value);
			if (x < 0) break;
			else ops = (size_t)x;
			++numOfDecreases;
			if (MaxOpDecrease < ops) MaxOpDecrease = ops;
			averageOpDecrease += ops;
			break;
		case 'I': // Insert element

			std::cin >> id >> value;
			heap.Insert(id, value);
			break;
		}
	}
	averageOpDecrease = averageOpDecrease / numOfDecreases;
	averageOpExtractMin = averageOpExtractMin / numOfExtractMins;
	std::cout << size << ";" << averageOpDecrease << ";" << averageOpExtractMin << ";" << MaxOpDecrease << ";" << MaxOpExtractMin << std::endl;

}
/**/


int main(int argc, char ** argv)
{
	bool naive = false;

	if (argc >= 2 && argv[1][0] == '-')
	{
		naive = true;
	}
	test(naive);
	
}


