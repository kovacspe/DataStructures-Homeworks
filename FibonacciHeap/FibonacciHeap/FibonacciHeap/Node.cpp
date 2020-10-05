#include "Node.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <cassert>
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif



	Node::Node(std::size_t identificator, std::size_t value) :id(identificator), val(value), lostChild(false), parent(nullptr), rightSibling(nullptr),
		leftSibling(nullptr), degree(0)
	{
		children = new LinkedListChilds(this); // make new List of children with itself as a parent
	}
	Node::~Node()
	{
		delete children;
	}

	std::string Node::toString()
	{
		return "("+ std::to_string(id) + ", " + std::to_string(val) +","+std::to_string(degree)+ ")";
	}



	LinkedListChilds::LinkedListChilds(Node * par) : first(nullptr), last(nullptr),parent(par)
	{
		
	}
	
	//add child
	void LinkedListChilds::AddChild(Node * node)
	{
		node->rightSibling = nullptr;
		if (first == nullptr)
		{
			// list is empty
			node->leftSibling = nullptr;
			first = node;
		}
		else
		{
			//not empty
			last->rightSibling = node;
		}

		
		node->leftSibling = last; 
		last = node; 

		// assign parent
		node->parent = parent;

		// increment degree
		if (node->parent!=nullptr)
		++(node->parent->degree);
		
	}

	//count number of elements in linear time
	size_t LinkedListChilds::size()
	{
		size_t count=0;
		Node * currentNode = first;
		while (currentNode != nullptr)
		{
			++count;
			currentNode = currentNode->rightSibling;
		}
		return count;
	}



	void LinkedListChilds::removeChild(Node * node)
	{
		if (node->leftSibling == nullptr)
		{
			//first node in list
			first = node->rightSibling;

			if (node->rightSibling == nullptr)
			{
				// only one element in list
				last = nullptr;
			}
			else
			{
				// more elements in list
				node->rightSibling->leftSibling = nullptr;
			}
		}
		else
		{
			// not first node
			node->leftSibling->rightSibling = node->rightSibling;


			if (node->rightSibling == nullptr)
			{
				//last node in list
				last = node->leftSibling;


			}

			else
			{
				// not last
				node->rightSibling->leftSibling = node->leftSibling;
			}
		}

		//remove siblings from node to preserve integrity
		node->rightSibling = nullptr;
		node->leftSibling = nullptr;
		
		//decrease degree
		if (node->parent != nullptr)
			--(node->parent->degree);
		//erase parent
		node->parent = nullptr;
		assert(first != nullptr || last == nullptr);
	}

	// consolidation
	size_t LinkedListChilds::consolidateHeap(size_t numOfElements)
	{
		assert(parent == nullptr); // call only in root list
		size_t countOfConsolidations = 0;
		DynamicArray<Node> degrees(numOfElements); // allocate array to index trees with degrees
		Node* currentNode = first;
		while (currentNode != nullptr)					//foreach node in  childlist
		{
			assert(currentNode->children->size() == currentNode->degree);
			Node * next = currentNode->rightSibling;
			size_t deg = currentNode->degree;

			while (degrees[deg] != nullptr) // while there isnt tree with same degree
			{
				++countOfConsolidations;
				// merge trees
				if (degrees[deg]->val < currentNode->val)
				{
					//add tree as child
					removeChild(currentNode);
					degrees[deg]->children->AddChild(currentNode);

					currentNode = degrees[deg];

				}
				else 
				{
					// add as child to this tree
					removeChild(degrees[deg]);
					currentNode->children->AddChild(degrees[deg]);				

				}
				degrees.Add(deg, nullptr);
				++deg;
			}
			degrees.Add(deg,currentNode);
			currentNode = next;
		}
		assert(first != nullptr || last == nullptr);
		return countOfConsolidations;
	}


	Node * LinkedListChilds::findMinInList()
	{
		Node * minimalNode = first;
		Node * currentNode = first;
		while (currentNode != nullptr)		// itereate all nods in  list
		{
			assert(currentNode->parent == nullptr);
			assert(!currentNode->lostChild);
			if (minimalNode->val > currentNode->val || (minimalNode->val == currentNode->val && minimalNode->degree>currentNode->degree)) // find minimal node with minimal degree (as a second condition)
			{
				minimalNode = currentNode;
			}
			currentNode = currentNode->rightSibling;
		}
		assert(first != nullptr || last == nullptr);
		return minimalNode;
	}

	
	void LinkedListChilds::addChildrensToHeapList(Node * parent) // use only on root list 
	{
		assert(parent == nullptr); // call only in root list
		if (parent->children != nullptr)
		{
			Node * currentNode = parent->children->first;
			while (currentNode!=nullptr)			// foreach child of node "parent"
			{
				currentNode->lostChild = false;		// unmark because it will be in the root list
				currentNode->parent = nullptr;		// delete parent

				currentNode = currentNode->rightSibling;
			}

		}
		AppendList(parent->children); // append children to this list
		assert(first != nullptr || last == nullptr);
	}

	void LinkedListChilds::AppendList(LinkedListChilds * list)
	{
		assert(parent == nullptr); // call only in root list, becasue it doesnt increment degree of a tree
		if (list->first == nullptr) return; // if appended list is empty do nothing
		if (first == nullptr)
		{
			// list is empty
			first = list->first;
		}
		else 
		{
			last->rightSibling = list->first;
			list->first->leftSibling = last;

		}
		last = list->last; // repair end

		list->first = nullptr; // make old list empty
		list->last = nullptr; // make old list empty
		assert(first != nullptr || last == nullptr);
	}

	void LinkedListChilds::print(std::string s)
	{
		std::cout << s << std::endl;
		Node * currentNode = first;
		while (currentNode != nullptr)
		{
			std::cout << currentNode->toString()<<",";
			currentNode = currentNode->rightSibling;
		}
		std::cout << std::endl;
	}


