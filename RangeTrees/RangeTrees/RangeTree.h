#pragma once
#include <string>
#include <iostream>
#include "Stack.h"





struct Node
{

	int32_t x, y;
	Node * left;
	Node * right;
	Node * parent;
	size_t size;
	Node * nextDimRoot;

	

	bool isRightSon(Node * possibleSon)
	{
		return (right == possibleSon);
	}

	bool isLeftSon(Node * possibleSon)
	{
		return (left == possibleSon);
	}
	
	bool ConditionHolds(double alpha)
	{
		return (right == nullptr || alpha*size >= right->size) && (left == nullptr || alpha*size >= left->size);
	}

	//check if condition will hold after inserting node in subtree
	bool ConditionHoldsAfterAdd(size_t leftAdd, size_t rightAdd, double alpha)
	{
		return (right == nullptr || alpha*size >= (right->size+rightAdd)) && (left == nullptr || alpha*size >= (left->size+leftAdd));
	}


	void DeleteYTree()
	{
		
		if (nextDimRoot != nullptr)
		{
			delete nextDimRoot;
		}
		nextDimRoot = nullptr;
		
	}

	void RemoveSons()
	{
		size = 1;
		right = nullptr;
		left = nullptr;
	}

	~Node()
	{
		
		if (parent != nullptr)
		{
			if (parent->right == this) parent->right = nullptr;
			else if (parent->left == this)parent->left = nullptr;
		}
		if (right!=nullptr) delete right;
		right = nullptr;
		if (left!=nullptr) delete left;
		left = nullptr;
		if (nextDimRoot != nullptr) delete nextDimRoot;
		nextDimRoot = nullptr;
	}

	Node(int32_t x, int32_t y)
	{
		
		this->x = x;
		this->y = y;
		parent = nullptr;
		nextDimRoot = nullptr;
		size = 1;
		left = nullptr;
		right = nullptr;
	}

	//Print recursively subtrees and Ytree
	std::string PrintAll(bool ytrees) 
	{
		
		std::string rightText = "";
		std::string leftText = "";
		std::string ytreeText = "";
		if (nextDimRoot != nullptr && ytrees)
		{			
			
				ytreeText = "\n\t{Ytree:\t" + nextDimRoot->PrintAll(ytrees) + "}\n";
		}
		if (right != nullptr)  rightText = right->PrintAll(ytrees);
		if (left!=nullptr) leftText = left->PrintAll(ytrees);
		return "L:(" + leftText + ") [" + std::to_string(x)+", "+std::to_string(y)+ "]-" + std::to_string(size) + " "+ytreeText+" P:(" + rightText + ")";
	}

	std::string Print()
	{
		return " [" + std::to_string(x) + ", " + std::to_string(y) + "]";
	
	}

};

struct XNode :Node
{
	inline int32_t key()
	{
		return x;
	}
};

struct YNode :Node
{
	inline int32_t key()
	{
		return y;
	}
};

void swap(YNode ** xp, YNode ** yp)
{
	YNode * temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void bubbleSort(YNode** arr, size_t size)
{
	int i, j;
	for (i = 0; i < size - 1; ++i)

		for (j = 0; j < size - i - 1; j++)
			if (arr[j]->y > arr[j + 1]->y)
				swap(&arr[j], &arr[j + 1]);
}


class RangeTree
{
private:
	size_t maxNumberOfInsertOperation;
	size_t maxNumberOfRangeCountOperation;
	size_t currentNumberOfInsertOperation;
	size_t currentNumberOfRangeCountOperation;
	size_t numberOfInserts;
	size_t numberOfInsertOperations;
	size_t numberOfRangeCount;
	size_t numberOfRangeCountOperation;
	XNode * root;
	double alpha;



	//insert Node in Y tree
	void _BasicInsertInYtree(Node * currentRoot,YNode * nodeToInsert)
	{
		bool inserted = false;

		YNode * currentNode =(YNode*) currentRoot->nextDimRoot;
		while (!inserted)
		{
			++currentNumberOfInsertOperation;
			++currentNode->size;
			if (currentNode->key() <= nodeToInsert->key())
			{
				if (currentNode->ConditionHoldsAfterAdd(0, 1, alpha))
				{


					if (currentNode->right == nullptr)
					{
						// insert node
						currentNode->right = nodeToInsert;
						nodeToInsert->parent = currentNode;
						return;
					}
					else
					{
						currentNode = (YNode*)currentNode->right;
					}
				}
				else
				{
					// rebuild needed
					_rebuildAndConnect(currentNode, nodeToInsert, false,currentRoot);
					return;
				}

			}
			else
			{
				if (currentNode->ConditionHoldsAfterAdd(0, 1, alpha))
				{

					if (currentNode->left == nullptr)
					{
						//insert node
						currentNode->left = nodeToInsert;
						nodeToInsert->parent = currentNode;
						return;
					}
					else
					{
						currentNode = (YNode*)currentNode->left;
					}
				}
				else
				{
					// rebuild needed
					_rebuildAndConnect(currentNode, nodeToInsert, false,currentRoot);
					return;
				}
			}

		}

	}

	// create Y tree or insert into existing
	void _insertInYTree(XNode * insertInto, YNode * node)
	{
		// insert node
		if (insertInto->nextDimRoot == nullptr)
		{
			insertInto->nextDimRoot = node;
			node->size = 1;
		}
		else
		_BasicInsertInYtree(insertInto, node);

	}

	//insert to X tree ant to all Ytrees on path to inserted X node
	void _insertToAllYtreesOnPathToX(XNode * currentRoot, int32_t x, int32_t y)
	{

		XNode * currentNode = currentRoot;
		while (true)
		{
			_insertInYTree(currentNode,(YNode*) new Node(x,y));
			++currentNumberOfInsertOperation;
			++currentNode->size;
			if (currentNode->key() <= x)
			{
				if (currentNode->ConditionHoldsAfterAdd(0, 1, alpha))
				{
					if (currentNode->right == nullptr)
					{
						// insert node
						XNode * n = (XNode*) new Node(x, y);
						currentNode->right = n;
						n->parent = currentNode;
						YNode * yNode = (YNode*) new Node(x, y);
						n->nextDimRoot = yNode;						
						return;
					}
					else
					{
						currentNode = (XNode*)currentNode->right;
					}
				}
				else
				{
					// rebuild needed
					_rebuildAndConnect(currentNode, new Node(x, y), true,nullptr);
					return;
				}

			}
			else
			{
				if (currentNode->ConditionHoldsAfterAdd(1, 0, alpha))
				{
					if (currentNode->left == nullptr)
					{
						//insert node
						XNode * n = (XNode*) new Node(x, y);
						currentNode->left = n;
						n->parent = currentNode;
						YNode * yNode = (YNode*) new Node(x, y);
						n->nextDimRoot = yNode;
						return;
					}
					else
					{
						currentNode = (XNode*)currentNode->left;
					}
				}
				else
				{
					// rebuild needed
					_rebuildAndConnect(currentNode, new Node(x, y), true,nullptr);
					return;
				}
			}

		}

	}

	// Rebuild whole subtree where current node belongs and insert newNode into it. Tree after rebuild will be connected to nodeToConnect 
	void _rebuildAndConnect(Node * currentNode,Node * newNode, bool Xtree, Node * nodeToConnect)
	{

		Node * n = currentNode->parent;
		Node ** arr; 
		Node * newRoot;
		
		if (Xtree)
		{			
			arr = _toArray<XNode>((XNode*) currentNode,(XNode*) newNode);			
			newRoot = BuildXtree(arr, 0, currentNode->size - 1);			
		}
		else
		{			
			arr = _toArray<YNode>((YNode*)currentNode, (YNode*)newNode);
			newRoot = BuildYtree((YNode**)arr, 0, currentNode->size - 1);		
		}
		delete [] arr;
		
		if (n == nullptr && Xtree) 
			root = (XNode*) newRoot;  // root was rebuild
		else if (n == nullptr) 
			nodeToConnect->nextDimRoot = newRoot; // root of Ytree was rebuild
		else if (n->isRightSon(currentNode))
		{
			n->right = newRoot;
			newRoot->parent = n;
		}
		else if (n->isLeftSon(currentNode))
		{
			n->left = newRoot;
			newRoot->parent = n;
		} 

	}
	
	// create Xtree or insert in existing
	void _insertInXtree(int32_t x,int32_t y)
	{
		if (root == nullptr)
		{
			root =(XNode*) new Node(x,y);			
			root->nextDimRoot = (YNode*) new Node(x,y);
		}
		else
		{
			_insertToAllYtreesOnPathToX(root, x, y);
		}

	}

	// Convert whole subtree where rootNode belongs to Array a a insert addNode into array. Array will be sort by key (based on node type )
	template <typename T>
	Node ** _toArray(T * rootNode,T * addNode)
	{

		
		bool addNodeAdded = false;
		Node ** nodes = new Node *[rootNode->size];
		size_t currentIndex = 0;
		Stack<std::pair<T*,bool>> stack(rootNode->size);
		stack.Push(std::pair<T*,bool>(rootNode,false));
		while (!stack.IsEmpty())
		{
			auto pair = stack.Pop();
			T * currentNode = pair.first;
			if (pair.second)
			{
				if (!addNodeAdded && addNode->key() <= currentNode->key() && (currentIndex==0 || addNode->key()>=((T*)nodes[currentIndex-1])->key()))
				{
					nodes[currentIndex] = addNode;
					++currentIndex;
					addNodeAdded = true;
				}
				nodes[currentIndex] = pair.first;
				pair.first->DeleteYTree();
				++currentIndex;
			}
			else
			{
				if (currentNode->right != nullptr) stack.Push(std::pair<T*, bool>((T*)currentNode->right, false));							
				stack.Push(std::pair<T*, bool>(currentNode, true));
				if (currentNode->left != nullptr)  stack.Push(std::pair<T*, bool>((T*)currentNode->left, false));
			}
		}		
		if (!addNodeAdded)
		{
			nodes[currentIndex] = addNode;
			currentIndex++;
		}
		

		
		return nodes;
	}

	// find node which is last common on path to both keys
	template <typename T>
	T * _findLastCommonPoint(T * treeRoot, int32_t key1, uint32_t key2)
	{
		T * currentNode = treeRoot;
		bool differentDirection = false;
		while (!differentDirection)
		{
			++currentNumberOfRangeCountOperation;
			if (currentNode == nullptr) return nullptr;
			if (key1 > currentNode->key())
			{
				currentNode =(T*) currentNode->right;
			}
			else if (key2 < currentNode->key())
			{
				currentNode =(T*) currentNode->left;
			}
			else differentDirection = true;
		}
		return currentNode;
	}

	// returnn, if point is in rectangle 
	bool _isInRectangle(Node * currentRoot, Node * boundary1, Node * boundary2)
	{
		return (currentRoot->x >= boundary1->x) && (currentRoot->x <= boundary2->x) && (currentRoot->y >= boundary1->y) && (currentRoot->y <= boundary2->y);
	}


	//go to the left and query right sons
	template <typename T>
	size_t _queryRightSons(T * currentRoot, T * boundary1, T* boundary2)
	{
		size_t count = 0;
		T * currentNode = currentRoot;
		while (currentNode != nullptr)
		{
			if (_isInRectangle(currentNode, boundary1, boundary2))
				++count;
			++currentNumberOfRangeCountOperation;
			if (currentNode->key() >= boundary1->key())
			{

					if (currentNode->right != nullptr)
					{
						if (currentNode->right->nextDimRoot != nullptr)
							count += _query<YNode>((YNode*)currentNode->right->nextDimRoot, (YNode*)boundary1, (YNode*)boundary2);
						else
							count += currentNode->right->size;
					}
					currentNode =(T*) currentNode->left;
				
			}
			else
			{
					currentNode =(T*) currentNode->right;			
			}
		}
		return count;
	}

	//go to the right and query left sons 
	template <typename T>
	size_t _queryLeftSons(T * currentRoot, T * boundary1, T* boundary2)
	{
		size_t count = 0;
		T * currentNode = currentRoot;
		while (currentNode != nullptr)
		{
			if (_isInRectangle(currentNode, boundary1, boundary2))
				++count;
			++currentNumberOfRangeCountOperation;
			if (currentNode->key() > boundary2->key())
			{
					currentNode = (T*)currentNode->left;
			}
			else
			{
				if (currentNode->left != nullptr) {
					if (currentNode->left->nextDimRoot != nullptr)
						count += _query<YNode>((YNode*)currentNode->left->nextDimRoot, (YNode*)boundary1, (YNode*)boundary2);
					else
						count += currentNode->left->size;
				}
				currentNode = (T*)currentNode->right;
			}
		}
		return count;
	}

	// query tree
	template <typename T>
	size_t _query(T * currentRoot,T * boundary1, T* boundary2)
	{

		size_t sum = 0;
		if (currentRoot == nullptr) return sum;
		T * lastCommon =(T*) _findLastCommonPoint(currentRoot, boundary1->key(), boundary2->key());
		if (lastCommon == nullptr) return sum;
		if (_isInRectangle(lastCommon, boundary1, boundary2))
			++sum;
		
		sum += _queryRightSons<T>((T*)lastCommon->left,boundary1,boundary2);		
		sum += _queryLeftSons<T>((T*)lastCommon->right,boundary1,boundary2);
		return sum;
	}

public:

	// rebuild Y tree
	YNode * BuildYtree(YNode ** Nodes, size_t startOffset, size_t endOffset)
	{
		++currentNumberOfInsertOperation;
		if (startOffset > endOffset) return nullptr;
		if (startOffset == endOffset) {	
			Nodes[startOffset]->RemoveSons();
			Nodes[startOffset]->parent = nullptr;
			return Nodes[startOffset];
		}
	
		
		
		size_t middle = startOffset + (endOffset - startOffset) / 2;
		YNode * median = Nodes[middle];
		median->RemoveSons();
		median->size = 1;
		if (middle == 0)
			median->left = nullptr;
		else
		{
			median->left = BuildYtree(Nodes, startOffset, middle - 1);
			if (median->left != nullptr)
			{
				median->left->parent = median;
				median->size += median->left->size;
			}
		}
		median->right = BuildYtree(Nodes, middle + 1, endOffset);
		if (median->right != nullptr)
		{
			median->right->parent = median;
			median->size += median->right->size;
		}
		median->parent = nullptr;
		
		return median;

	}

	// rebuild X tree and its Y trees
	XNode * BuildXtree(Node ** Nodes, size_t startOffset, size_t endOffset)
	{
		++currentNumberOfInsertOperation;
		if (startOffset > endOffset) return nullptr;
		if (startOffset == endOffset)
		{			
			Nodes[startOffset]->RemoveSons();			
					
			Nodes[startOffset]->parent = nullptr;			
			Nodes[startOffset]->nextDimRoot = new Node(Nodes[startOffset]->x, Nodes[startOffset]->y);
			return (XNode*)Nodes[startOffset];
		}
	
		size_t middle = startOffset + (endOffset - startOffset) / 2;
		
		XNode * median =(XNode*) Nodes[middle];
		median->RemoveSons();
		median->DeleteYTree();
		size_t length = endOffset - startOffset;
		YNode** Ynodes = new YNode*[length+1];
		for (size_t i = 0; i <= length; i++)
		{
			Ynodes[i] = (YNode*) new Node(Nodes[startOffset + i]->x, Nodes[startOffset + i]->y);
			
		}
		
		bubbleSort(Ynodes, length + 1);
		median->nextDimRoot = BuildYtree(Ynodes, 0, length);
		delete [] Ynodes;
		median->size = 1;
		if (middle == 0)
			median->left = nullptr;
		else
		{
			median->left = BuildXtree(Nodes, startOffset, middle - 1);
			
			if (median->left != nullptr)
			{
				median->left->parent = median;
				median->size += median->left->size;
			}
		}
		median->right = BuildXtree(Nodes, middle + 1, endOffset);
		
		if (median->right != nullptr)
		{
			median->right->parent = median;
			median->size += median->right->size;
		}
		median->parent = nullptr;
		
		return median;
	}

	void Insert(int32_t x, uint32_t y)
	{
		numberOfInserts++;
		currentNumberOfInsertOperation = 0;

		_insertInXtree(x, y);

		if (maxNumberOfInsertOperation < currentNumberOfInsertOperation) maxNumberOfInsertOperation = currentNumberOfInsertOperation;
		numberOfInsertOperations += currentNumberOfInsertOperation;

	}

	void Print(bool ytree)
	{
		std::cout << root->PrintAll(ytree);
	}

	size_t RangeCount(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
	{
		++numberOfRangeCount;
		currentNumberOfRangeCountOperation = 0;

		size_t result = _query<XNode>(root, (XNode*) new Node(x1, y1), (XNode*) new Node(x2, y2));
				
		if (currentNumberOfRangeCountOperation > maxNumberOfRangeCountOperation) maxNumberOfRangeCountOperation = currentNumberOfRangeCountOperation;
		numberOfRangeCountOperation += currentNumberOfRangeCountOperation;
		return result;
	}

	size_t GetMaxRangeCountOperations()
	{
		return maxNumberOfRangeCountOperation;
	}

	size_t GetMaxInsertOperations()
	{
		return maxNumberOfInsertOperation;
	}

	double GetAverageInsertOperations()
	{
		return numberOfInsertOperations / (double)numberOfInserts;
	}

	double GetAverageRangCountOperations()
	{
		return numberOfRangeCountOperation/(double)numberOfRangeCount;
	}

	~RangeTree()
	{
		delete root;
	}

	RangeTree(double alpha)
	{
		root = nullptr;
		this->alpha = alpha;
		numberOfInsertOperations = 0;
		numberOfInserts = 0;
		maxNumberOfInsertOperation = 0;
		maxNumberOfRangeCountOperation = 0;
		numberOfRangeCount = 0;
		numberOfRangeCountOperation = 0;
	}
	
};




