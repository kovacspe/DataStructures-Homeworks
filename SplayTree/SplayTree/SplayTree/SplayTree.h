#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iostream>


// represents one tree node
class SplayTreeNode
{
private:

public:
	SplayTreeNode * Left;
	SplayTreeNode * Right;
	SplayTreeNode * Parent;
	uint64_t Value;


	SplayTreeNode(uint64_t x, SplayTreeNode * parent)
	{
		Value = x;
		Parent = parent;
	}

	~SplayTreeNode()
	{
		
	}

	// return subtree where value x should be
	SplayTreeNode * GetSubtree(uint64_t x)
	{
	
		if (x < Value)
		{
			return Left;
		}
		else
		{
			return Right;
		}
	}

	// insert value as son to this node
	SplayTreeNode * Insert(uint64_t x)
	{
		if (x < Value)
		{

			Left = new SplayTreeNode(x, this);
			return Left;
		}
		else
		{
			Right = new SplayTreeNode(x, this);
			return Right;
		}
	}

	// check if node is left son of its parent
	bool IsLeftSon()
	{
		if (Parent == nullptr) return false;
		return (Parent->Left == this);
	}

	// check if node is right son of its parent
	bool IsRightSon()
	{
		if (Parent == nullptr) return false;
		return (Parent->Right == this);
	}


	// return scheme of tree in string. Warning! Recursive function, use only for small trees
	std::string Print()
	{
		std::string left = "-";
		std::string right = "-";
		if (Left != nullptr) left = "(" + Left->Print() + ") ";
		if (Right != nullptr) right = " (" + Right->Print() + ")";
		return left + std::to_string(Value) + right;
	}

	
	

};

// represents tree as data structure
class SplayTree
{
private:
	SplayTreeNode * root;
	bool isNaive = false;
public:
	~SplayTree()
	{
		ClearTree();
		root = nullptr;
	}

	// desruct all nodes in tree
	void ClearTree()
	{
		SplayTreeNode * currentNode = root;
		while (root != nullptr)
		{
			if (currentNode->Left != nullptr)
			{
				currentNode = currentNode->Left;
			}
			else if (currentNode->Right != nullptr)
			{
				currentNode = currentNode->Right;
			}
			else
			{
				if (currentNode->Parent == nullptr)
				{
					root = nullptr;
				}
				else if (currentNode->IsLeftSon())
				{
					currentNode->Parent->Left = nullptr;
				}
				else
				{
					currentNode->Parent->Right = nullptr;
				}

				SplayTreeNode * parent = currentNode->Parent;
				delete currentNode;				
				currentNode = parent;
			}
		}
		
	}

	// return scheme of tree in string. Warning! Recursive function, use only for small trees
	std::string Print()
	{
		if (root != nullptr)
			return root->Print();
		else return "NO ROOT";
	}

	// set if tree use naive rotations
	void ChangeNaivity(bool newVal)
	{
		isNaive = newVal;
	}

	// insert element in tree
	void Insert(uint64_t x)
	{
		if (root == nullptr)
		{
			//create root
			root = new SplayTreeNode(x, nullptr);
		}
		else
		{
			// find place to insert
			SplayTreeNode * currentNode = root;
			SplayTreeNode * nextNode;
			nextNode = currentNode->GetSubtree(x);
			while (nextNode != nullptr)
			{
				currentNode = nextNode;
				nextNode = currentNode->GetSubtree(x);

			}

			// Insert
			currentNode = currentNode->Insert(x);

			// Splay
			Splay(currentNode);

		}
	}


	void Splay(SplayTreeNode * node)
	{
		while (node != root)
		{
			if (isNaive)
			{
				// Naive rotation
				if (node->IsLeftSon())
					RightRotation(node->Parent);
				else LeftRotation(node->Parent);
			}
			else if (node->Parent == root)
			{
				if (node->IsLeftSon())
					RightRotation(root);
				else LeftRotation(root);

			}
			else if (node->IsLeftSon() && node->Parent->IsLeftSon())
			{

				SplayTreeNode * firstParent = node->Parent;
				SplayTreeNode * secondParent = node->Parent->Parent;
				
					RightRotation(secondParent);
					RightRotation(firstParent);
				
			}
			else if (node->IsRightSon() && node->Parent->IsRightSon())
			{

				SplayTreeNode * firstParent = node->Parent;
				SplayTreeNode * secondParent = node->Parent->Parent;
			
					LeftRotation(secondParent);
					LeftRotation(firstParent);
				

			}
			else if (node->IsRightSon() && node->Parent->IsLeftSon())
			{

				SplayTreeNode * firstParent = node->Parent;
				SplayTreeNode * secondParent = node->Parent->Parent;

				LeftRotation(firstParent);
				RightRotation(secondParent);

			}
			else if (node->IsLeftSon() && node->Parent->IsRightSon())
			{

				SplayTreeNode * firstParent = node->Parent;
				SplayTreeNode * secondParent = node->Parent->Parent;

				RightRotation(firstParent);
				LeftRotation(secondParent);
			}
			else {
				std::cout << "Unexpected situation" << std::endl;
				return;
			}
		}
	}

	// find element and return length of path from root to element
	size_t Find(uint64_t x)
	{
		size_t counter=0;
		if (root == nullptr)
		{
			
			return 0;
		}
		else
		{
			SplayTreeNode * currentNode = root;
			while (currentNode->Value != x)
			{
				currentNode = currentNode->GetSubtree(x);
				++counter;
				if (currentNode == nullptr) return SIZE_MAX;
			}
			
			Splay(currentNode);
			
			return counter;
		}
	}

	void RightRotation(SplayTreeNode * node)
	{
		SplayTreeNode * Parent;
		SplayTreeNode * LeftSon = node->Left;
		SplayTreeNode * LeftRightSon = node->Left->Right;

		if (node == root)
		{
			root = node->Left;
			Parent = nullptr;
		}
		else
		{
			Parent = node->Parent;
			if (node->IsLeftSon())
				Parent->Left = LeftSon;
			else
				Parent->Right = LeftSon;
		}



		node->Parent = LeftSon;	
		LeftSon->Parent = Parent;
		LeftSon->Right = node;
		node->Left = LeftRightSon;
		if (LeftRightSon != nullptr) LeftRightSon->Parent = node;

	}

	void LeftRotation(SplayTreeNode * node)
	{
		SplayTreeNode * Parent;
		SplayTreeNode * RightSon = node->Right;
		SplayTreeNode * RightLeftSon = node->Right->Left;
		if (node == root)
		{
			root = node->Right;
			Parent = nullptr;
		}
		else
		{
			Parent = node->Parent;
			if (node->IsLeftSon())
				Parent->Left = RightSon;
			else
				Parent->Right = RightSon;
		}

		node->Parent = RightSon;
		RightSon->Parent = Parent;
		RightSon->Left = node; 
		node->Right = RightLeftSon;
		if (RightLeftSon != nullptr) RightLeftSon->Parent = node;


	}

};

