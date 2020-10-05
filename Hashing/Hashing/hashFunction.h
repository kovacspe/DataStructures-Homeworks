#include <string>
#include <iostream>

class HashFunction
{
private:
	size_t prime;
	size_t maxHash;
	size_t a;

public:

	HashFunction()
	{

	}
	HashFunction(size_t prime, size_t maxHash, size_t a)
	{
		this->prime = prime;
		this->maxHash = maxHash;
		this->a = a;
	}

	size_t GetHash(std::string word)
	{
		size_t sum=0;
		for (size_t i = 0; i < word.length(); ++i)
		{
			size_t currentHash = word[i];
			for (size_t j = 0; j < i; j++)
			{
				currentHash = (currentHash*a);
				if (currentHash > prime)
					{
						currentHash = currentHash%prime;
					}
			}
			sum += currentHash;
			if (sum > prime)
			{
				sum = sum % prime;
			}
		}
		return sum % maxHash;
	}
};
