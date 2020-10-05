#pragma once

template <typename T>
class Stack
{
	T * stack;
	size_t stackPointer;

public:
	Stack(size_t length)
	{
		stack = new T[length];
		stackPointer = 0;
	}

	~Stack()
	{
		delete [] stack;
	}

	bool IsEmpty()
	{
		return stackPointer == 0;
	}

	T Pop() 
	{
		
		T item = stack[stackPointer-1];
		--stackPointer;
		return item;
	}

	T Peek()
	{
		return stack[stackPointer - 1];
	}

	void Push(T item)
	{
		++stackPointer;
		stack[stackPointer-1] = item;
	}
};