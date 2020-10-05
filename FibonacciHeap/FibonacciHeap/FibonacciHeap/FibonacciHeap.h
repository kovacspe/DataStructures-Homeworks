#pragma once
#include <cassert>
#include "Node.h"
#include <iostream>
class LinkedListChilds;
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif




class FibonacciHeap
{
private:
	LinkedListChilds treeRoots; // list of trees
	DynamicArray<Node> elements; // list of elements. I is used for indexing nodes in constant time, e. g. in Decrease to find to node in constant time 
	size_t maxDegree;
	bool isNaive;
	

	bool cutNode(Node * currentNode)
	{
		bool parentMarked = false;
		assert(currentNode->parent != nullptr);
		parentMarked = currentNode->parent->lostChild; // save if parent was marked before cut
		if (currentNode->parent->parent!=nullptr) currentNode->parent->lostChild = true; // mark parent
		currentNode->parent->children->removeChild(currentNode); // cut node form parent


		treeRoots.AddChild(currentNode); // add cutted node to the root list
		currentNode->lostChild = false; // unmark cutted node, because it is a root node now
		return parentMarked; // return if the parent of cutted node was marked before cut
	}

	

public:
	FibonacciHeap(size_t expectedElements, bool naive) : elements(expectedElements), isNaive(naive), treeRoots(nullptr)
	{	
		
		maxDegree = 1;
	}



	~FibonacciHeap()
	{
		elements.DeleteAll();
	}

	// Clear heap, dealocate all nodes
	void clear()
	{
		elements.DeleteAll();
	}

	// copy assignment
	FibonacciHeap& operator=(FibonacciHeap&& heap)
	{
		elements = std::move(heap.elements);
		treeRoots = std::move(heap.treeRoots);
		isNaive = heap.isNaive;
		maxDegree = heap.maxDegree;
		return *this;
	}

	// Delete Min
	size_t ExtractMin()
	{
		Node * MinimalNode = treeRoots.findMinInList();															 // Find minimal node in root list
		if (MinimalNode == nullptr) return 0;																	 // if there isnt any minimal node return 0 operations
		assert(MinimalNode->children->size() == MinimalNode->degree);
		size_t numberOfOperations = MinimalNode->degree;														 // For each child count one operation 
		treeRoots.addChildrensToHeapList(MinimalNode);															 // Move all children of minimal node to root list
		treeRoots.removeChild(MinimalNode);																		 //Remove minimal node from root list
		numberOfOperations += treeRoots.consolidateHeap((size_t) ceil(std::log2(elements.size())));				 //Consolidate heap
		elements.Add(MinimalNode->id,nullptr);																	 // Erase minimal node form indexer 
		delete MinimalNode;
		return numberOfOperations;
	}

	int Decrease(size_t element, size_t newVal)
	{
		size_t numberOfOperations=0;
		Node * currentNode = elements[element];
		if (currentNode == nullptr || currentNode->val<newVal) return -1;
		
		currentNode->val = newVal;
		
		if (currentNode->parent != nullptr && currentNode->parent->val>currentNode->val)  // Check heap condition
		{
			
			Node * Parent = currentNode->parent;
			bool lostChild = cutNode(currentNode);    // cut Node from parent
			++numberOfOperations;
			if (!isNaive) // If is naive continue with cascade cutting
			{
				while (Parent->parent != nullptr && lostChild) // while current node isnt in root list or lost child for the first time
				{
					Node * NextParent = Parent->parent;
					lostChild = cutNode(Parent);				// cut node
					++numberOfOperations;
					Parent = NextParent;					
				}
			}
		}
		return numberOfOperations;
	}

	// add new element
	void Insert(size_t id,size_t value)
	{
		Node * newNode = new Node(id,value);
		elements.Add(id, newNode);		// add new node to indexer
		treeRoots.AddChild(newNode);	// Add new node to root list
	}

};