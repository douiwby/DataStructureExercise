#pragma once
#include "BinarySearchTree.h"

template<typename T>
class RedBlackTree : public BinarySearchTree<T>
{
public:

	virtual BinNode<T>* insert(const T& val) override;
	virtual bool erase(const T& val) override;

	static bool isBlack(BinNode<T>* node) { return !node || node->_color == BLACK; }
	static bool isRed(BinNode<T>* node) { return !isBlack(node); }
	static bool isBlackHeightUpdated(BinNode<T>* node);

	virtual void printTree(int wordWidth = 4);

protected:

	virtual void updateHeight(BinNode<T>* node) override;

	virtual void internalPrintData(int currentWidth, int wordWidth, BinNode<T>* node) override;

	void solveDoubleRed(BinNode<T>* pos);
	void solveDoubleBlack(BinNode<T>* pos);
};

template<typename T>
inline BinNode<T>* RedBlackTree<T>::insert(const T & val)
{
	if (this->empty())
	{
		this->insertAsRoot(val);
		this->_root->_color = BLACK;
		updateHeight(this->_root);
		return this->_root;
	}

	BinNode<T>* pos = this->search(val);
	if (pos) return pos;
	else
	{
		// Insert node as red.
		BinNode<T>* ret = nullptr;
		if (val > this->_hot->getData())
		{
			ret = this->_hot->insertAsRChild(new BinNode<T>(val));
		}
		else
		{
			ret = this->_hot->insertAsLChild(new BinNode<T>(val));
		}
		++(this->_size);
		updateHeight(ret);

		solveDoubleRed(ret);

		return ret;
	}
}

template<typename T>
inline bool RedBlackTree<T>::erase(const T & val)
{
	BinNode<T>* pos = this->search(val);
	if (pos)
	{
		bool isDeleteNodeBlack = isBlack(pos);
		BinNode<T>* ret = this->eraseAt(pos);
		--(this->_size);

		if (isDeleteNodeBlack)
		{
			if (ret && ret->_color == RED)
			{
				ret->_color = BLACK;
				ret->_height++;
			}
			else
			{
				solveDoubleBlack(ret);
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline bool RedBlackTree<T>::isBlackHeightUpdated(BinNode<T>* node)
{
	if (!node) return true;
	int lHeight = stature(node->lChild);
	int rHeight = stature(node->rChild);
	return lHeight == rHeight && node->_height == (isRed(node->lChild) ? lHeight : lHeight + 1);
}

template<typename T>
inline void RedBlackTree<T>::updateHeight(BinNode<T>* node)
{
	// The _height represent black height, but the real value of black height is _height+1
	int lHeight = stature(node->lChild);
	int rHeight = stature(node->rChild);
	node->_height = lHeight > rHeight ? lHeight : rHeight;
	if (isBlack(node)) node->_height++;
}

template<typename T>
inline void RedBlackTree<T>::solveDoubleRed(BinNode<T>* pos)
{
	BinNode<T>* v = pos;
	BinNode<T>* p = v->parent;
	BinNode<T>* g = p ? p->parent : nullptr;
	BinNode<T>* u = nullptr;

	while (p&&g)
	{
		if (p->_color == BLACK) return; // No double red issue

		u = p->isLChild() ? g->rChild : g->lChild;

		if (isBlack(u))
		{
			// Rotate and exchange the color

#define ROTATE_FIRST 1

#if ROTATE_FIRST
			BinNode<T>* r = this->rotateAt(v);

			r->_color = BLACK;
			r->lChild->_color = RED;
			r->rChild->_color = RED;
#else
			if (v->isLChild() == p->isLChild())
			{
				p->_color = BLACK;
			}
			else
			{
				v->_color = BLACK;
			}
			g->_color = RED;

			BinNode<T>* r = this->rotateAt(v);
#endif
			updateHeight(r->lChild);
			updateHeight(r->rChild);
			updateHeight(r);

			return;
		}
		else
		{
			// Switch the color
			p->_color = BLACK;
			p->_height++;
			u->_color = BLACK;
			u->_height++;
			g->_color = RED;

			// g is red now, consider the double red on higher level
			v = g;
			p = v->parent;
			g = p ? p->parent : nullptr;
		}
	}

	if (!p)
	{
		// v is root
		if (v->_color == RED)
		{
			v->_color = BLACK;
			v->_height++;
		}
		return;
	}
	else
	{
		// p is root, it must be black, we got no issue now.
		return;
	}
}

template<typename T>
inline void RedBlackTree<T>::solveDoubleBlack(BinNode<T>* pos)
{
	BinNode<T>* v = pos;  // note v could be nullptr, can't use v->isLChild()
	BinNode<T>* p = v ? v->parent : this->_hot;
	if (!p) return; // v is root
	BinNode<T>* s = (v == p->lChild) ? p->rChild : p->lChild;  // sibling
	BinNode<T>* n = nullptr;

	assert(s);

	while (p)
	{
		if (isBlack(s))
		{
			if (isRed(s->lChild)) n = s->lChild;
			if (isRed(s->rChild)) n = s->rChild;
			if (n)
			{
				s->_color = p->_color;
				p->_color = BLACK;
				n->_color = BLACK;

				this->rotateAt(n);

				updateHeight(n);
				updateHeight(p);
				updateHeight(s);

				return;
			}

			if (isRed(p))
			{
				p->_color = BLACK;
				s->_color = RED;
				updateHeight(s);
				return;
			}
			else
			{
				s->_color = RED;
				updateHeight(s);
				updateHeight(p);

				// All node's height decrease 1, treat as that we has erased a node in p
				v = p;
				p = v->parent;
				if (!p) return;  // v is root now
				s = (v == p->lChild) ? p->rChild : p->lChild;
			}
		}
		else
		{
			p->_color = RED;
			s->_color = BLACK;

			if (s->isLChild())
				this->zig(p);
			else 
				this->zag(p);

			updateHeight(s);
			updateHeight(p);

			s = (v == p->lChild) ? p->rChild : p->lChild;
		}
	}
}

template<typename T>
inline void RedBlackTree<T>::printTree(int wordWidth)
{
	BinNode<T>* p = this->_root;
	Vector<BinNode<T>*> v;
	v.push_back(p);

	size_t currrentIndex = 0;
	int numNodeOfCurrentLevel = 1;
	int totalHeight = 0;

	while (true)
	{
		bool bReachOutsideOfTree = true;

		for (int i = 0; i < numNodeOfCurrentLevel; ++i)
		{
			p = v[currrentIndex];

			if (p)
			{
				v.push_back(p->lChild);
				v.push_back(p->rChild);

				bReachOutsideOfTree = false;
			}
			else
			{
				v.push_back(nullptr);
				v.push_back(nullptr);
			}

			++currrentIndex;
		}

		if (bReachOutsideOfTree) break;

		++totalHeight;
		numNodeOfCurrentLevel *= 2;
	}

	p = this->_root;
	Queue<BinNode<T>*> q;
	q.push(p);

	int currentHeight = 1;
	int currentRowCount = 1;
	int currentWidth = wordWidth << (totalHeight - 1);

	std::cout << "--------" << std::endl;
	std::cout << std::left;

	do
	{
		for (int i = 0; i < currentRowCount; ++i)
		{
			p = q.front();
			q.pop();

			if (p)
			{
				internalPrintData(currentWidth, wordWidth, p);
				q.push(p->lChild);
				q.push(p->rChild);
			}
			else
			{
				std::cout << std::setw(currentWidth) << '\0';
				q.push(nullptr);
				q.push(nullptr);
			}
		}

		++currentHeight;
		currentRowCount <<= 1;
		currentWidth >>= 1;

		std::cout << std::endl;
	} while (currentHeight <= totalHeight);

	std::cout << "--------" << std::endl;
}

template<typename T>
inline void RedBlackTree<T>::internalPrintData(int currentWidth, int wordWidth, BinNode<T>* node)
{
	char color = node->_color == BLACK ? 'B' : 'R';
	if (currentWidth != wordWidth)
	{
		std::cout << std::right;
		std::cout << std::setw(currentWidth / 2) << node->data << color << std::setw(currentWidth / 2) << ' ';
		std::cout << std::left;
	}
	else
	{
		std::cout << node->data << std::setw(currentWidth) << color;
	}
}