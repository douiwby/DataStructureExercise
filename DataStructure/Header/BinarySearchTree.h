#pragma once
#include "BinTree.h"

template<typename T>
class BinarySearchTree : public BinTree<T>
{
protected:

	BinNode<T>* _hot;

public:

	virtual BinNode<T>* search(const T& val);
	virtual BinNode<T>* searchIn(BinNode<T>* r, const T& val);

	virtual BinNode<T>* insert(const T& val);
	virtual bool erase(const T& val);

protected:

	// Remember updateHeightAbove on return node's parent
	BinNode<T>* connect34(BinNode<T>* a, BinNode<T>* b, BinNode<T>* c, BinNode<T>* T0, BinNode<T>* T1, BinNode<T>* T2, BinNode<T>* T3);
	BinNode<T>* rotateAt(BinNode<T>* v);

	void swap(BinNode<T>*& val1, BinNode<T>*& val2);
};

template<typename T>
BinNode<T>* BinarySearchTree<T>::search(const T & val)
{
	return searchIn(this->root(), val);
}

template<typename T>
BinNode<T>* BinarySearchTree<T>::searchIn(BinNode<T>* r, const T & val)
{
	_hot = nullptr;
	BinNode<T>* currentNode = r;
	while (currentNode)
	{
		_hot = currentNode;
		if (val < currentNode->getData())
		{
			currentNode = currentNode->getLChild();
		}
		else if (val > currentNode->getData())
		{
			currentNode = currentNode->getRChild();
		}
		else
		{
			_hot = currentNode->getParent();
			return currentNode;
		}
	}
	return currentNode;
}

template<typename T>
BinNode<T>* BinarySearchTree<T>::insert(const T & val)
{
	if (this->empty()) this->insertAsRoot(val);

	BinNode<T>* pos = search(val);
	BinNode<T>* ret = nullptr;
	if (!pos)
	{
		if (val > _hot->getData())
		{
			ret = _hot->insertAsRChild(new BinNode<T>(val));
		}
		else
		{
			ret = _hot->insertAsLChild(new BinNode<T>(val));
		}
		++(this->_size);
		this->updateHeightAbove(ret);
		return ret;
	}
	else
	{
		return pos;
	}
}

template<typename T>
bool BinarySearchTree<T>::erase(const T& val)
{
	BinNode<T>* pos = search(val);
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
			swap(pos, swapNode);
			
			this->fromParentTo(pos) = pos->getRChild();
			if (pos->hasRChild()) pos->getRChild()->parent = pos->parent;
			parentNode = pos->parent;
			delete pos;
		}
		--(this->_size);
		this->updateHeightAbove(parentNode);
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
BinNode<T>* BinarySearchTree<T>::connect34(BinNode<T>* a, BinNode<T>* b, BinNode<T>* c, BinNode<T>* T0, BinNode<T>* T1, BinNode<T>* T2, BinNode<T>* T3)
{
	/*********************
	*      B
	*    /   \
	*   A     C
	*  / \   / \
	* T0 T1 T2 T3
	*********************/

	a->lChild = T0;
	if (T0) T0->parent = a;
	a->rChild = T1;
	if (T1) T1->parent = a;
	this->updateHeight(a);
	c->lChild = T2;
	if (T2) T2->parent = c;
	c->rChild = T3;
	if (T3)T3->parent = c;
	this->updateHeight(c);
	b->lChild = a;
	a->parent = b;
	b->rChild = c;
	c->parent = b;
	this->updateHeight(b);
	return b;
}

template<typename T>
BinNode<T>* BinarySearchTree<T>::rotateAt(BinNode<T>* v)
{
	BinNode<T>* p = v->parent;
	BinNode<T>* g = p->parent;
	if (p->isLChild())
	{
		if (v->isLChild())
		{
			// zig-zig
			this->fromParentTo(g) = p;
			p->parent = g->parent;
			return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
		}
		else
		{
			// zig-zag
			this->fromParentTo(g) = v;
			v->parent = g->parent;
			return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
		}
	}
	else
	{
		if (v->isRChild())
		{
			// zag-zag
			this->fromParentTo(g) = p;
			p->parent = g->parent;
			return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
		}
		else
		{
			// zag-zig
			this->fromParentTo(g) = v;
			v->parent = g->parent;
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
		}
	}
}

template<typename T>
inline void BinarySearchTree<T>::swap(BinNode<T>*& val1, BinNode<T>*& val2)
{
	BinNode<T>* tempParent = val2->parent;
	BinNode<T>* tempLChild = val2->lChild;
	BinNode<T>* tempRChild = val2->rChild;
	int tempHeight = val2->_height;

	this->fromParentTo(val1) = val2;
	this->fromParentTo(val2) = val1;
	val2->parent = val1->parent;
	val1->parent = tempParent;

	if (val1->hasLChild())
	{
		val1->lChild->parent = val2;
		val2->lChild = val1->lChild;
		val1->lChild = tempLChild;
		if (tempLChild) tempLChild->parent = val1;
	}

	if (val1->hasRChild())
	{
		val1->rChild->parent = val2;
		val2->rChild = val1->rChild;
		val1->rChild = tempRChild;
		if (tempRChild) tempRChild->parent = val1;
	}

	val2->_height = val1->_height;
	val1->_height = tempHeight;
}