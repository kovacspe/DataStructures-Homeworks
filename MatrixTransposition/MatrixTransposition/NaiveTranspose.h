#include "Matrix.h"
#include <algorithm>
#include <fstream>

class Transpose
{
	static void TransposeMatrixNaive(Matrix & m)
	{
		for (size_t i = 0; i < m.size; i++)
		{
			for (size_t j = i+1; j < m.size; j++)
			{
				std::swap(m.at(i, j), m.at(j, i));
			}
		}
	}

	static void TransposeMatrixNaive_Verbose(Matrix & m,std::ostream & os)
	{
		for (size_t i = 0; i < m.size; i++)
		{
			for (size_t j = i+1; j < m.size; j++)
			{
				os << "X" << i << j << j << i<<std::endl;
			}
		}
	}

	static void CacheObliviousTranspose()
	{

	}

	static void TransposeOnDiagonal(Matrix & m,size_t startX,size_t startY, size_t endX, size_t endY)
	{

	}

	static void TransposeAndSwap()
	{


	}
	Procedure transpose on diagonal(A)
		2 if Matice A je mala´ then
		3 Transponujeme matici A trivialn´ ´ım postupem
		4 else
		5 A11, A12, A21, A22 ← souˇradnice submatic
		6 transpose on diagonal(A11)
		7 transpose on diagonal(A22)
		8 transpose and swap(A12, A21)
		9 Procedure transpose and swap(A, B)
		10 if Matice A a B jsou male´ then
		11 Prohod´ıme a transponujeme matice A a B trivialn´ ´ım postupem
		12 else
		13 A11, A12, A21, A22, B11, B12, B21, B22 ← souˇradnice submatic
		14 transpose and swap(A11, B11)
		15 transpose and swap(A12, B21)
		16 transpose and swap(A21, B12)
		17 transpose and swap(A22, B22)

};
