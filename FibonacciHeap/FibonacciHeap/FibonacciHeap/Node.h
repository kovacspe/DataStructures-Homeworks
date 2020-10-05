#pragma once
#include <cstdio>
#include <cassert>
#include <string>
#include <math.h>
#include <algorithm>
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif
class LinkedListChilds;

template <class T>
class DynamicArray
{
private:
	T** arr;
	size_t currentLength;
public:
	DynamicArray(size_t size)
	{
		currentLength = size;
		arr = new  T*[size];
		for (size_t i = 0; i < size; ++i)
		{
			arr[i] = nullptr;
		}
	}


	~DynamicArray()
	{
		delete[] arr;
	}

	// copy assignment, leave copied dynamic array blank
	DynamicArray& operator=(DynamicArray&& dyn)
	{
		arr = dyn.arr;
		dyn.arr = nullptr;
		currentLength = dyn.currentLength;
		dyn.currentLength = 0;
		return *this;
	}

	/* 
	* Delete all objects, that can be referenced from the array and remove them from array
	* 
	*/
	void DeleteAll()
	{
		for (size_t i = 0; i < currentLength; ++i)
		{
			if (arr[i] != nullptr)
			{
				delete arr[i];
				arr[i] = nullptr;
			}
		}
	}



	void Resize(size_t newLength)
	{
		// allocate new array
		T** newArr = new T*[newLength];

		// copy old array
		for (size_t i = 0; i < currentLength; ++i)
		{
			newArr[i] = arr[i];
		}

		// fill the rest with null pointers
		for (size_t i = currentLength; i < newLength; ++i)
		{
			newArr[i] = nullptr;
		}

		delete[] arr; // delete old array
		currentLength = newLength;
		arr = newArr;
	}

	// returns current length of array
	size_t size()
	{
		return currentLength;
	}

	// write node pointer on specified index
	void Add(size_t index, T * node)
	{
		if (index >= currentLength)
		{
			// need to resize array
			size_t newLength =(size_t) pow(2,ceil(log2(index+1))); // compute nearest power of 2
			Resize(newLength);
		}
		assert(index < currentLength);
		arr[index] = node;
	}

	void print(std::string label)
	{
		std::cout << label << std::endl;
			for (size_t i = 0; i < currentLength; i++)
			{
				if (i > 10)break;
				if (arr[i]!=nullptr)
					std::cout << arr[i]->toString()<<", ";
				else std::cout << "-,";
			}
		std::cout << std::endl;
	}

	// readonly indexer
	T * operator[](size_t index)
	{
		if (index >= currentLength)
		{
			// need to resize
			size_t newLength = (size_t)ceil(pow(2, log2(index + 1))); // compute nearest power of 2
			Resize(newLength);
		}
		assert(index < currentLength);
		return arr[index];
	}
};



struct Node
{
	LinkedListChilds * children; // Linked list of children nodes
	bool lostChild;              // Flag whether node lost child
	Node * parent;				 // Pointer to node's parent	
	std::size_t val;			 // Key (value)
	std::size_t id;				 // ID
	std::size_t degree;			 // Degree - number of children
	Node * rightSibling;		 // Right sibling - next child in parent's children linked list
	Node * leftSibling;			 // Right sibling - previous child in parent's children linked list


	Node(std::size_t identificator, std::size_t value);
	~Node();
	std::string toString();      // Text representation of node


};



class LinkedListChilds
{
private:
	Node * first; // first element in linked list
	Node * last;  // last element in linked list
	Node * parent; // Parent who owns child linked list. This parent will be set as parent to every child which will be added

public:
	LinkedListChilds(Node * par);


	void AppendList(LinkedListChilds* list);		//append LinkedList to the end of this Linked List
	void AddChild(Node * node);						// add element to linked list
	void removeChild(Node * node);					// Remove element from linkedList
	void print(std::string s);						// print list
	size_t size();									// Get size of list ... Allert ... in linear time
	std::size_t consolidateHeap(std::size_t numOfElements);  // Consolidate elements in the list. Use only in root
	Node * findMinInList();							// find node with minimal value in the list
	void addChildrensToHeapList(Node * parent);		// Add all children of node parent to this Linked List

};




