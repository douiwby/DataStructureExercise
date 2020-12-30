#pragma once
#include "BinarySearchTree.h"

template<typename T>
class AVLTree : public BinarySearchTree<T>
{
public:

	virtual BinNode<T>* insert(const T& val);
	virtual bool erase(const T& val);

protected:

	bool Balanced(BinNode<T>* pos) { return stature(pos->lChild) == stature(pos->rChild); }
	int BalanceFactor(BinNode<T>* pos) { return stature(pos->lChild) - stature(pos->rChild); }
	bool AVLBalanced(BinNode<T>* pos) { return BalanceFactor(pos) > -2 && BalanceFactor(pos) < 2; }
	BinNode<T>* tallerChild(BinNode<T>* pos);
};

template<typename T>
inline BinNode<T>* AVLTree<T>::tallerChild(BinNode<T>* pos)
{
	if (stature(pos->lChild) > stature(pos->rChild))
	{
		return pos->lChild;
	}
	else if (stature(pos->lChild) < stature(pos->rChild))
	{
		return pos->rChild;
	}
	else if(pos->isLChild())
	{
		return pos->lChild;
	}
	else
	{
		return pos->rChild;
	}
}

template<typename T>
BinNode<T>* AVLTree<T>::insert(const T & val)
{
	if (this->empty())
	{
		this->insertAsRoot(val);
		return this->root();
	}

	BinNode<T>* pos = this->search(val);
	BinNode<T>* ret = nullptr;
	if (!pos)
	{
		if (val > this->_hot->getData())
		{
			ret = this->_hot->insertAsRChild(new BinNode<T>(val));
		}
		else
		{
			ret = this->_hot->insertAsLChild(new BinNode<T>(val));
		}
		++(this->_size);

		// Rotate node to make tree balance
		for (BinNode<T>* g = this->_hot; g; g = g->parent)
		{
			if (!AVLBalanced(g))
			{
				this->rotateAt(tallerChild(tallerChild(g)));
				break;
			}
			else
			{
				this->updateHeight(g);
			}
		}

		return ret;
	}
	else
	{
		return pos;
	}
}

template<typename T>
bool AVLTree<T>::erase(const T & val)
{
	BinNode<T>* pos = this->search(val);
	if (pos)
	{
		BinNode<T>* swapNode;
		BinNode<T>* parentNode;
		if (!pos->hasRChild())
		{
			swapNode = pos->getLChild();

			this->fromParentTo(pos) = swapNode;
			if (swapNode) swapNode->parent = pos->parent;
			parentNode = pos->parent;
			delete pos;
		}
		else if (!pos->hasLChild())
		{
			swapNode = pos->getRChild();

			this->fromParentTo(pos) = swapNode;
			if (swapNode) swapNode->parent = pos->parent;
			parentNode = pos->parent;
			delete pos;
		}
		else
		{
			swapNode = pos->succ();
			this->swap(pos, swapNode);

			this->fromParentTo(pos) = pos->getRChild();
			if (pos->hasRChild()) pos->getRChild()->parent = pos->parent;
			parentNode = pos->parent;
			delete pos;
		}
		--(this->_size);

		// Rotate node to make tree balance
		for (BinNode<T>* g = this->_hot; g; g = g->parent)
		{
			if (!AVLBalanced(g))
			{
				g = this->rotateAt(tallerChild(tallerChild(g)));
			}
			this->updateHeight(g);
		}

		return true;
	}
	else
	{
		return false;
	}
}
