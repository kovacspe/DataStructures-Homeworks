#include <cstdint>
#include <iostream>
class BitArray
{
private:
	const size_t CELL_SIZE=8;
	uint8_t * byteArray;
	size_t size;
	size_t numOfBytes;

	bool getBitFromByte(size_t bitNumber, uint8_t byte)
	{
		uint8_t mask = (1 << bitNumber);
		return (byte&mask) != 0;
	}

	inline void SetBitOnByte(size_t bitNumber, size_t byteIndex)
	{
		uint8_t mask = (1 << bitNumber);
		byteArray[byteIndex] |= mask;
	}

public:
	BitArray(size_t sizeOfBitArray)
	{
		numOfBytes = std::ceil(sizeOfBitArray / (double)CELL_SIZE);
		this->size = sizeOfBitArray;
		byteArray = new uint8_t[numOfBytes];
		for (size_t i = 0; i < numOfBytes; i++)
		{
			byteArray[i] = 0;
		}
		
	}

	~BitArray()
	{
		delete[] byteArray;
	}

	size_t GetNumberOfOnes()
	{
		size_t sum=0;
		for (size_t i = 0; i < numOfBytes; i++)
		{
			uint8_t byte = byteArray[i];
			uint8_t mask = 1;
			for (size_t j = 0; j < CELL_SIZE; j++)
			{
				if ((byte&mask) != 0)
				{
					++sum;
				} 
				mask = mask << 1;
			}
		}
		return sum;
	}

	void Insert(size_t hash)
	{

		SetBitOnByte(hash%CELL_SIZE, hash / CELL_SIZE);
	}

	bool GetValue(size_t hash)
	{
		return getBitFromByte(hash%CELL_SIZE, byteArray[hash / CELL_SIZE]);
	}

};