#pragma once

#include "bitArray.h"
#include "hashFunction.h"
#include <cstdlib>
#include <string>

class HashTable
{
private:
	size_t numberOfWords;
	size_t numberOfHashFunctions;
	BitArray bitArray;
	HashFunction * hashFunctions;
public:
	HashTable(size_t prime, size_t numberOfHashFunctions, size_t sizeOfBitArray) : bitArray{sizeOfBitArray}
	{

		numberOfWords = 0;
		this->numberOfHashFunctions = numberOfHashFunctions;
		
		hashFunctions = new HashFunction[numberOfHashFunctions];
		// generate hash functions
		for (size_t i = 0; i < numberOfHashFunctions; i++)
		{
			size_t a = rand() % prime;
			hashFunctions[i] = HashFunction(prime, sizeOfBitArray, a);
		}
	
	}

	size_t GetNumberOfWords()
	{
		return numberOfWords;
	}
	size_t GetNumbefOfOnesInBitArray()
	{
		return bitArray.GetNumberOfOnes();
	}

	// returns true if word was already saved in hash table
	bool Insert(std::string word)
	{
		bool wasSaved = true;
		for (size_t i = 0; i < numberOfHashFunctions; i++)
		{
			auto hash = hashFunctions[i].GetHash(word);
			if (!bitArray.GetValue(hash))
			{
				wasSaved = false;
				bitArray.Insert(hash);
			}

		}
		if (!wasSaved)
		{
			// new word inserted
			++numberOfWords;
		}

		return wasSaved;
	}

	bool Contains(std::string word)
	{
		for (size_t i = 0; i < numberOfHashFunctions; i++)
		{
			if (!bitArray.GetValue(hashFunctions[i].GetHash(word)))
				return false;
		}
		return true;
	}
};
