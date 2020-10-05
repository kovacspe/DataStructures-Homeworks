#include "SplayTree.h"
#include "OptimalBST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bits/stdc++.h>
#include<vector>
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS_GLOBALS

static void ParseInput(std::ifstream & in, std::string name, std::ofstream & out, bool optBST)
{

	char op;
	SplayTree tree;
	SplayTree naiveTree;
	size_t sumOfFinds;
	size_t sumOfFindsNaive;
	naiveTree.ChangeNaivity(true);
	sumOfFinds = 0;
	sumOfFindsNaive = 0;
	int number = 0;
	size_t sample_count = 0;
	in >> op >> number;

	while (!in.eof() && !in.fail())
	{

		sample_count = number;
		std::map<uint64_t, size_t> frequencies;

		tree.ClearTree();
		naiveTree.ClearTree();
		in >> op >> number;

		while (op != '#' && !in.eof() && !in.fail())
		{

			if (op == 'I')
			{
				// insert
				tree.Insert(number - 1);
				naiveTree.Insert(number - 1);
			}
			else if (op == 'F')
			{
				//find
				size_t treeCount = tree.Find(number - 1);
				size_t naiveTreeCount = naiveTree.Find(number - 1);
				sumOfFinds += treeCount;
				sumOfFindsNaive += naiveTreeCount;

				
				frequencies.insert({ number - 1,0 });
				frequencies[number - 1]++;

			}
			in >> op >> number;

		}
		size_t opt = 0;


		// collect frequencies from map
		std::vector<size_t> freq;
		for (auto && x : frequencies)
		{
			freq.push_back(x.second);
			std::cout << x.second << "|";
		}


		// count prefix sums of frequencies to make queries faster
		size_t * prefixSums = new std::size_t[freq.size() + 1];
		prefixSums[0] = 0;
		for (size_t i = 1;i <= freq.size();++i)
		{
			prefixSums[i] = prefixSums[i - 1] + freq[i - 1];

		}

		//count static optimal binary search tree
		if (optBST)
		{
			opt = GetOptimalSearchTreeCost(prefixSums, freq.size());
		}

		// output
		out << name << ";" << sample_count << ";" << prefixSums[freq.size()] << ";" << sumOfFinds << ";" << sumOfFindsNaive << ";" << opt << std::endl;
		std::cout << name << ";" << sample_count << ";" << prefixSums[freq.size()] << ";" << sumOfFinds << ";" << sumOfFindsNaive << ";" << opt << std::endl;
		
		//reset counters
		sumOfFinds = 0;
		sumOfFindsNaive = 0;
		delete[] prefixSums;

	}


}

int main(int argc, char* argv[])
{

	std::ifstream input("inputFiles.txt");
	std::ofstream output("OUTother2.txt");

	std::ios_base::sync_with_stdio(false);
	input.tie(nullptr);
	while (!input.eof())
	{
		std::string name;
		input >> name;
		std::ifstream in(name);
		ParseInput(in, name,output,true);
		
	}
	output.close();
	input.close();
	return 0;
}