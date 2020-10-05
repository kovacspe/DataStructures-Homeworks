
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>



	class SplayTreeNode
	{
	private:

	public:
		SplayTreeNode * Left;
		SplayTreeNode * Right;
		SplayTreeNode * Parent;
		uint64_t Value;

		 
		SplayTreeNode(uint64_t x,SplayTreeNode * parent)
		{
			Value = x;
			Parent = parent;
		}

		~SplayTreeNode()
		{
			delete Left;
			delete Right;
			delete Parent;			
			//delete this;
		}

		void Insert(uint64_t x)
		{
			if (x < Value)
			{
				if (Left == nullptr) 
					Left = new SplayTreeNode(x,this);

				else (*Left).Insert(x);
			}
			else
			{
				if (Right == nullptr) 
					Right = new SplayTreeNode(x,this);
				else 
					(*Right).Insert(x);
			}
		}

		bool IsLeftSon()
		{
			if (Parent == nullptr) return false;
			return (Parent->Left == this);
		}

		bool IsRightSon()
		{
			if (Parent == nullptr) return false;
			return (Parent->Right == this);
		}
		
		std::string Print()
		{
			std::string left = "";
			std::string right = "";
			if (Left != nullptr) left = "(" + Left->Print() + ") ";
			if (Right != nullptr) right = " ("+Right->Print() + ")";
			return left  + std::to_string(Value) + right;
		}

		SplayTreeNode * Find(uint64_t x,size_t & counter)
		{
			if (x == Value)
			{ 
				return this;
			}
			if (x < Value)
			{
				if (Left == nullptr) 
					return nullptr;
				else 
					return (*Left).Find(x,++counter);

			}
			else
			{
				if (Right == nullptr) 
					return nullptr;
				else 
					return (*Right).Find(x,++counter);
			}

		}

	};


	class SplayTree
	{
	private:
		SplayTreeNode * root;

	public:
		~SplayTree()
		{
			delete root;
		}

		void ClearTree()
		{
			delete root;
		}

		std::string Print()
		{
			if (root != nullptr) 
				return root->Print();
			else return "NO ROOT";
		}

		void Insert(uint64_t x)
		{
			if (root == nullptr)
			{
				//create root
				root = new SplayTreeNode(x,nullptr);
			}
			else
			{
				root->Insert(x);
			}
		}


		void Splay(SplayTreeNode * node)
		{
			while (node != root)
			{
				if (node->Parent == root)
				{
					if (node->IsLeftSon())
						RightRotation(root);
					else LeftRotation(root);

				}
				else if (node->IsLeftSon() && node->Parent->IsLeftSon())
				{
					// zig-zig to the right
					SplayTreeNode * firstParent = node->Parent;
					SplayTreeNode * secondParent = node->Parent->Parent;
					RightRotation(secondParent);
					RightRotation(firstParent);
				}
				else if (node->IsRightSon() && node->Parent->IsRightSon())
				{
					//zig-zig to the left
					SplayTreeNode * firstParent = node->Parent;
					SplayTreeNode * secondParent = node->Parent->Parent;
					LeftRotation(secondParent);
					LeftRotation(firstParent);
				}
				else if (node->IsRightSon() && node->Parent->IsLeftSon())
				{
					//zig-zag
					SplayTreeNode * firstParent = node->Parent;
					SplayTreeNode * secondParent = node->Parent->Parent;

					LeftRotation(firstParent);
					RightRotation(secondParent);

				}
				else if (node->IsLeftSon() && node->Parent->IsRightSon())
				{
					//zig-zag
					SplayTreeNode * firstParent = node->Parent;
					SplayTreeNode * secondParent = node->Parent->Parent;

					RightRotation(firstParent);
					LeftRotation(secondParent);
				}
			}
		}

		size_t Find(uint64_t x)
		{
			size_t counter;
			if (root == nullptr)
			{
				//create root
				return 0;
			}
			else
			{
				SplayTreeNode * foundNode = root->Find(x, counter);
				Splay(foundNode);
				return counter;
			}
		}

		void RightRotation(SplayTreeNode * node)
		{
			SplayTreeNode * Parent;
			if (node == root)
			{
				root = node->Left;
				Parent = nullptr;
			}
			else
				Parent = node->Parent;
			SplayTreeNode * LeftSon = node->Left;
			SplayTreeNode * LeftRightSon = node->Left->Right;
			node->Parent = LeftSon;
			node->Left = LeftRightSon;
			LeftSon->Parent = Parent;
		}

		void LeftRotation(SplayTreeNode * node)
		{
			SplayTreeNode * Parent;
			if (node == root)
			{
				root = node->Right;
				Parent = nullptr;
			}
			else
			{
				Parent = node->Parent;
				if (node->IsLeftSon())
					Parent->Left = node;
				else
					Parent->Right = node;
			}
			
			SplayTreeNode * RightSon = node->Right;
			SplayTreeNode * RightLeftSon = node->Right->Left;

			node->Parent = RightSon;
			RightSon->Parent = Parent;

			node->Right = RightLeftSon;

		}

	};

