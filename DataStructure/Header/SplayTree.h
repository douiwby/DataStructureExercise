#pragma once
#include "BinarySearchTree.h"

template<typename T>
class SplayTree : public BinarySearchTree<T>
{
public:

	virtual BinNode<T>* search(const T& val) override;
	virtual BinNode<T>* insert(const T& val) override;
	virtual bool erase(const T& val) override;

protected:

	BinNode<T>* splay(BinNode<T>* pos);
};

template<typename T>
inline BinNode<T>* SplayTree<T>::search(const T & val)
{
	BinNode<T>* result = this->searchIn(this->_root, val);
	return splay(result ? result : this->_hot);
}

template<typename T>
inline BinNode<T>* SplayTree<T>::insert(const T & val)
{
	if (this->empty()) this->insertAsRoot(val);

	BinNode<T>* pos = search(val);
	if (pos->getData() < val)
	{
		this->_root = new BinNode<T>(val);
		this->_root->lChild = pos;
		pos->parent = this->_root;
		this->_root->rChild = pos->rChild;
		if (pos->rChild) pos->rChild->parent = this->_root;
		pos->rChild = nullptr;
		++(this->_size);
	}
	else if(pos->getData() > val)
	{
		this->_root = new BinNode<T>(val);
		this->_root->rChild = pos;
		pos->parent = this->_root;
		this->_root->lChild = pos->lChild;
		if (pos->lChild) pos->lChild->parent = this->_root;
		pos->lChild = nullptr;
		++(this->_size);
	}
	this->updateHeightAbove(pos);
	return this->_root;
}

template<typename T>
inline bool SplayTree<T>::erase(const T & val)
{
	BinNode<T>* pos = search(val);
	if (pos->getData() != val) return false;
	
	pos->rChild->parent = nullptr;
	BinNode<T>* newRoot = this->searchIn(pos->rChild, val);
	if (!newRoot) newRoot = this->_hot;
	splay(this->_hot);

	newRoot->lChild = pos->lChild;
	pos->lChild->parent = newRoot;
	newRoot->parent = nullptr;

	delete pos;
	--(this->_size);
	this->updateHeightAbove(this->_root);
	return true;
}

template<typename T>
inline BinNode<T>* SplayTree<T>::splay(BinNode<T>* pos)
{
	BinNode<T>* v = pos;
	BinNode<T>* p = v->parent;
	BinNode<T>* g = p ? p->parent : nullptr;

	while (p && g)
	{
		BinNode<T>* t;
		if ((v->isLChild() && p->isRChild()) || (v->isRChild() && p->isLChild()))
		{
			t = this->rotateAt(v);
			this->updateHeight(v->lChild);
			this->updateHeight(v->rChild);
			this->updateHeightAbove(t);
		}
		else if (v->isLChild() && p->isLChild())
		{
			this->zig(g);
			this->zig(p);
			t = v;
		}
		else if (v->isRChild() && p->isRChild())
		{
			this->zag(g);
			this->zag(p);
			t = v;
		}
		else
		{
			assert(false);
		}

		v = t;
		p = v->parent;
		if(p) g = p->parent;
		else g = nullptr;
	}

	if (p)
	{
		if (v->isLChild())
		{
			this->zig(p);
		}
		else
		{
			this->zag(p);
		}
	}
	v->parent = nullptr;
	this->_root = v;
	return v;
}
