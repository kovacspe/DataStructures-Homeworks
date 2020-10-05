#pragma once
#include "SplayTree.cpp";
template <type T>
class SplayTree
{
private:
	SplayTree * Left;
	SplayTree * Right;
	T value;

public:

	SplayTree(T x);

	void Insert(T x);

	void Splay(SplayTree * X);

	T Find(T x);

};