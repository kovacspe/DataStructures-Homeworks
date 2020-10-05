#include <stdint.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

//statick˝ logger krokov transpozÌcie
class Logger
{
	//kam sa log zapisuje
	static std::ofstream currentOutput;
public:

	
	// nastavÌ output logu do novÈho filu
	static void openFile(std::string file)
	{
		Logger::currentOutput = std::ofstream(file);
		//currentOutput.open(file);
		std::ios_base::sync_with_stdio(false);
		currentOutput.tie(nullptr);
	}
	//zatvorÌ file
	static void closeFile()
	{
		currentOutput.close();
	}

	//zaloguje zaËiatok transpozÌcie
	static void logStart(size_t matrixSize)
	{
		currentOutput << "N " << matrixSize << std::endl;
		std::cout << "N " << matrixSize << std::endl;
	}

	//zaloguje koniec traspozÌcie
	static void logEnd()
	{
		currentOutput << "E" << std::endl;
		//std::cout << "E" << std::endl;
	}

	// zaloguje jeden krok transpozÌcie
	static void log(size_t x1, size_t y1, size_t x2, size_t y2)
	{
		currentOutput << "X " << x1 << " " << y1 << " " << x2 << " " << y2<<std::endl;
		//std::cout << "X " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	}
};

//pr·zdny Logger - ak netreba logovaù
class NoLog
{
public:
	static void log(size_t x1, size_t y1, size_t x2, size_t y2){}
};


template <class logger>
class Matrix
{
	// reprezent·cia matice
	int32_t * matrixArray;
	static const size_t DIVIDE_CONSTANT = 4; // veækosù submatice, ktor· sa uû transponuje naivne

public:
	const size_t size;
	Matrix(size_t sizeOfMatrix): size{sizeOfMatrix}
	{		
		matrixArray = new int32_t[sizeOfMatrix*sizeOfMatrix];
	}

	~Matrix()
	{			
		delete [] matrixArray;
	}
	
	// indexuje prvok v matici
	int32_t & at(size_t x, size_t y)
	{
		return matrixArray[y*size + x];
	}

	//vypÌöe maticu
	void print(std::ostream & os)
	{
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				os<<at(i, j)<<" ";
			}
			os << std::endl;
		}
	}

	// inicializuje maticu na takÈ hodnoty, aby bolo vidno transponovanie
	void init()
	{
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				at(i, j) = (i+1)*10+j;
			}
		}
	}

	// Prevedie naivn˙ transpozÌciu matice
	void transposeNaive()
	{
		transposeSubmatrixNaive(0, 0, size);
	}

	// transponuje naivne submaticu
	void transposeSubmatrixNaive(size_t x, size_t y, size_t lengthOfSubmatrix)
	{
		for (size_t i = x; i < lengthOfSubmatrix; i++)
		{
			for (size_t j = i + 1; j < lengthOfSubmatrix; j++)
			{
				std::swap(at(i, j), at(j, i));
				logger::log(i, j, j, i);
			}
		}
	}

	//transpose and swap - naivn˝
	void transposeAndSwapNaive(size_t startX1, size_t startY1,  size_t width, size_t height)
	{
		
		for (size_t i = 0; i < width; i++)
		{
			for (size_t j =0; j < height; j++)//i+1
			{
				std::swap(at(startX1+i,startY1+j),at(startY1+j,startX1+i));
				logger::log(startX1 + i, startY1 + j, startY1 + j, startX1 + i);

			}
		}
	}


	//transponuje submaticu na diagon·le
	void transposeOnDiagonal(size_t startX, size_t startY,size_t length)
	{
		if (length <= DIVIDE_CONSTANT )
			transposeSubmatrixNaive(startX, startY, length);
		else
		{
			size_t midlength = length / 2;
			size_t middleX = startX + midlength; //X-s˙radnica v polovici matice
			size_t middleY = startY + midlength; //Y-s˙radnica v polovici matice
			size_t secondLength = length - midlength;
			transposeOnDiagonal(startX, startY, midlength);
			transposeOnDiagonal(middleX, middleY, secondLength);
			transposeAndSwap( startX, middleY, midlength,secondLength);
		}
	}

	void transposeAndSwap(size_t startX1, size_t startY1,  size_t width, size_t height)
	{
		if (width <= DIVIDE_CONSTANT || height <= DIVIDE_CONSTANT)
			transposeAndSwapNaive(startX1, startY1,width,height);
		else
		{
			size_t midwidth = width / 2;
			size_t midheight = height / 2;
			size_t middleX1 = startX1 + midwidth; //X-s˙radnica v polovici matice
			size_t middleY1 = startY1 + midheight; //X-s˙radnica v polovici matice
	

			transposeAndSwap(startX1, startY1, midwidth,midheight);
			transposeAndSwap(middleX1, startY1,  (width-midwidth),midheight);

			transposeAndSwap(startX1, middleY1,  midwidth,(height-midheight));
			transposeAndSwap(middleX1, middleY1,  (width - midwidth),(height - midheight));
		}
	}

	//transponuje maticu CacheOblivious
	void transposeCacheOblivious()
	{
		transposeOnDiagonal(0, 0, size);
	}
		

};