#pragma once

#include <stdio.h> 
#include <limits.h>
#include <algorithm>


/*
Sum from i-th to j-th element of array
*/
size_t sum(size_t * arr, int i, int j)
{
	size_t s = 0;
	for (size_t k = i; k <= j; k++)
		s += arr[k];
	return s;
}

/*
Sum from i-th to j*th element in prefix sums array.
*/
size_t fastSum(size_t * prefixSums, size_t i, size_t j)
{
	if (i<=j)
	return prefixSums[j+1] - prefixSums[i];
	else return 0;
}

/*
Compute cost of static optimal binary search tree
*/
size_t GetOptimalSearchTreeCost(std::size_t * prefixSums,size_t numberOfElements)
{

	
	
	std::size_t ** costs = new std::size_t*[numberOfElements]; // costs of subtrees
	std::size_t ** roots = new std::size_t*[numberOfElements]; // roots of subtrees
	for (int i = 0; i < numberOfElements; i++)
	{
		costs[i] = new size_t[numberOfElements];
		roots[i] = new size_t[numberOfElements];
		//costs[i][i] = freq[i];
		roots[i][i] = i;
	}

	for (size_t i = 0; i < numberOfElements; i++)
	{
		for (size_t j = 0; j < numberOfElements; j++)
		{
			costs[i][j] = 0;
		}

		roots[i][i] = i;
		
	}


	for (size_t length = 2; length <= numberOfElements; ++length) // foreach length
	{
		for (size_t i = 0; i < numberOfElements - length + 1; ++i) // foreach node
		{
			size_t j = i + length - 1;

			costs[i][j] = INT_MAX;
			for (size_t r = roots[i][j-1]; r <= roots[i+1][j]; ++r) //foreach possible root
			{				

				size_t c = 
					((i<r) ? costs[i][r - 1] : 0) 
					+ ((r<j) ? costs[r + 1][j] : 0)
					+ fastSum(prefixSums, i, r-1)
					+ fastSum(prefixSums,r+1,j);

				if (c < costs[i][j])
				{
					costs[i][j] = c;
					roots[i][j] = r;

				}

			}

		}
	}

	// dealocate memory
	size_t cost = costs[0][numberOfElements - 1];
	for (size_t i = 0; i < numberOfElements; i++)
	{
		delete[] costs[i];
		delete[] roots[i];

	}
	delete[] costs;
	delete[] roots;
	return cost;
}
/**/



