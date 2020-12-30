#pragma once

#include "Stack.h"
#include "Queue.h"

#define stature(node) ((node)?(node)->height():-1)

template<typename T> class BinTree;
template<typename T> class BinarySearchTree;
template<typename T> class AVLTree;

enum RBColor
{
	RED,
	BLACK
};

enum TraversalImplementVersion
{
	RECURSION,
	ITERATION_1,
	ITERATION_2,
};
#define DefaultVersion ITERATION_1

template<typename T>
class BinNode
{
	friend class BinTree<T>;
	friend class BinarySearchTree<T>;
	friend class AVLTree<T>;

public:

	// --------------------
	// Constructor
	// --------------------

	BinNode(
		const T& inData,
		BinNode<T>* inParent = nullptr,
		BinNode<T>* inLChild = nullptr,
		BinNode<T>* inRChild = nullptr,
		int inHeight = 0,
		RBColor inColor = RED
		) :data(inData), parent(inParent), lChild(inLChild), rChild(inRChild), _height(inHeight), _color(inColor)
	{}
private:

	// --------------------
	// Data member
	// --------------------

	T data;
	BinNode<T>* parent;
	BinNode<T>* lChild;
	BinNode<T>* rChild;

	// --------------------
	// Helper member
	// --------------------

	int _height;
	RBColor _color;

public:

	int height() const { return _height; }
	int size() const;

	BinNode<T>* getParent() const{ return parent; }
	BinNode<T>* getLChild() const{ return lChild; }
	BinNode<T>* getRChild() const{ return rChild; }
	T& getData() { return data; }
	const T& getData() const { return data; }

	// --------------------
	// Check status
	// --------------------

	bool isRoot() const { return !parent; }
	bool isLChild() const { return !isRoot() && (this == parent->lChild); }
	bool isRChild() const { return !isRoot() && (this == parent->rChild); }
	bool hasParent() const { return parent; }
	bool hasLChild() const { return lChild; }
	bool hasRChild() const { return rChild; }
	bool hasChild() const { return hasLChild() || hasRChild(); }
	bool hasBothChild() const { return hasLChild() && hasRChild(); }
	bool isLeaf() const { return !hasChild(); }

	// --------------------
	// Operator override
	// --------------------

	bool operator==(const BinNode& rhs) const { return data == rhs.data; }
	bool operator!=(const BinNode& rhs) const { return data != rhs.data; }
	bool operator<(const BinNode& rhs) const { return data < rhs.data; }
	bool operator>(const BinNode& rhs) const { return data > rhs.data; }
	bool operator<=(const BinNode& rhs) const { return data <= rhs.data; }
	bool operator>=(const BinNode& rhs) const { return data >= rhs.data; }

	// --------------------
	// BinTree operation function
	// --------------------

	BinNode<T>* insertAsLChild(BinNode<T>* data);
	BinNode<T>* insertAsRChild(BinNode<T>* data);

	template<typename FUNC> void traversalInorder(FUNC func, TraversalImplementVersion version = DefaultVersion);
	template<typename FUNC> void traversalPreorder(FUNC func, TraversalImplementVersion version = DefaultVersion);
	template<typename FUNC> void traversalPostorder(FUNC func, TraversalImplementVersion version = DefaultVersion);
	template<typename FUNC> void traversalLevel(FUNC func, TraversalImplementVersion version = DefaultVersion);

	BinNode<T>* succ();
};

template<typename T>
inline int BinNode<T>::size() const
{
	/*
	int sz = 1;
	if (lChild) sz += lChild->size();
	if (rChild) sz += rChild->size();
	return sz;
	*/

	Stack<const BinNode<T>*> s;
	const BinNode<T>* p = this;
	s.push(p);
	int sz = 0;
	while (!s.empty())
	{
		if (p->hasLChild()) s.push(p->lChild);
		if (p->hasRChild())
		{
			p = p->rChild;
			sz += 1;
		}
		else
		{
			p = s.top();
			s.pop();
			sz += 1;
		}
	}
	return sz;
}

template<typename T>
BinNode<T>* BinNode<T>::insertAsLChild(BinNode<T>* data)
{
	assert(!lChild);
	lChild = data;
	lChild->parent = this;
	return lChild;
}

template<typename T>
BinNode<T>* BinNode<T>::insertAsRChild(BinNode<T>* data)
{
	assert(!rChild);
	rChild = data;
	rChild->parent = this;
	return rChild;
}

template<typename T>
inline BinNode<T>* BinNode<T>::succ()
{
	BinNode<T>* ret = this;
	if (hasRChild())
	{
		ret = rChild;
		while (ret->hasLChild()) ret = ret->lChild;
	}
	else
	{
		while (ret->isRChild())ret = ret->parent;
		ret = ret->parent;
	}
	return ret;
}

template<typename T>
template<typename FUNC>
inline void BinNode<T>::traversalInorder(FUNC func, TraversalImplementVersion version)
{
	if (version == RECURSION)
	{
		if (lChild) lChild->traversalInorder(func);
		func(data);
		if (rChild) rChild->traversalInorder(func);
	}
	else if (version == ITERATION_1)
	{
		BinNode<T>* p = this;
		Stack<BinNode<T>*> s;
		while (true)
		{
			if (p)
			{
				s.push(p);
				p = p->lChild;
			}
			else if(!s.empty())
			{
				p = s.top();
				s.pop();
				func(p->data);
				p = p->rChild;
			}
			else break;
		}
	}
	else if (version == ITERATION_2)
	{
		// use bool to instead stack, use succ(). Take more time but less memory.
		bool backtrace = false;
		BinNode<T>* p = this;
		while (p)
		{
			if (!backtrace && p->hasLChild()) p = p->lChild;
			else
			{
				func(p->data);
				if (p->hasRChild())
				{
					p = p->rChild;
					backtrace = false;
				}
				else
				{
					p = p->succ();
					backtrace = true;
				}
			}
		}
	}
}

template<typename T>
template<typename FUNC>
inline void BinNode<T>::traversalPreorder(FUNC func, TraversalImplementVersion version)
{
	if (version == RECURSION)
	{
		func(data);
		if (lChild) lChild->traversalPreorder(func);
		if (rChild) rChild->traversalPreorder(func);
	}
	else if (version == ITERATION_1)
	{
		BinNode<T>* p = this;
		Stack<BinNode<T>*> s;
		s.push(p);
		while (!s.empty())
		{
			p = s.top();
			s.pop();
			func(p->data);

			// Push rChild first!
			if (p->hasRChild()) s.push(p->rChild);
			if (p->hasLChild()) s.push(p->lChild);
		}
	}
	else if (version == ITERATION_2)
	{
		BinNode<T>* p = this;
		Stack<BinNode<T>*> s;
		while (p)
		{
			func(p->data);
			if (p->hasRChild()) s.push(p->rChild);
			if (p->hasLChild())
			{
				p = p->lChild;
			}
			else
			{
				if (s.empty())break;
				p = s.top();
				s.pop();
			}
		}
	}
}

template<typename T>
template<typename FUNC>
inline void BinNode<T>::traversalPostorder(FUNC func, TraversalImplementVersion version)
{
	if (version == RECURSION)
	{
		if (lChild) lChild->traversalPostorder(func);
		if (rChild) rChild->traversalPostorder(func);
		func(data);
	}
	else if (version == ITERATION_1 || version == ITERATION_2)
	{
		BinNode<T>* p = this;
		Stack<BinNode<T>*> s;
		s.push(p);
		while (!s.empty())
		{
			if (s.top() != p->parent)
			{
				while (p = s.top())
				{
					if (p->hasLChild())
					{
						s.push(p->rChild);
						s.push(p->lChild);
					}
					else
						s.push(p->rChild);
				}
				s.pop(); // remove the empty node
			}
			p = s.top();
			s.pop();
			func(p->data);
		}
	}
}

template<typename T>
template<typename FUNC>
inline void BinNode<T>::traversalLevel(FUNC func, TraversalImplementVersion version)
{
	BinNode<T>* p = this;
	Queue<BinNode<T>*> q;
	q.push(p);
	while (!q.empty())
	{
		p = q.front();
		q.pop();

		func(p->data);

		if (p->hasLChild()) q.push(p->lChild);
		if (p->hasRChild()) q.push(p->rChild);
	}
}
