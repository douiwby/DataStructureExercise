#pragma once
#include <iomanip>
#include "BinNode.h"

template<typename T>
class BinTree
{
public:
	BinTree(BinNode<T>* inRoot = nullptr) :_root(inRoot) { _size = inRoot ? inRoot->size() : 0; }
	~BinTree() { remove(_root); }

protected:

	BinNode<T>* _root;
	int _size;

public:

	int size() const { return _size; }
	bool empty() const { return !_root; }
	BinNode<T>* root() const { return _root; }

	// --------------------
	// Member operator
	// --------------------

	BinNode<T>* insertAsRoot(const T& data);
	BinNode<T>* insertAsLChild(BinNode<T>* parent, const T& data);
	BinNode<T>* insertAsRChild(BinNode<T>* parent, const T& data);

	// Move a subtree and attach to certain node
	// Return the subtree root after moved
	BinNode<T>* attachAsLChild(BinNode<T>* parent, BinNode<T>* node);
	BinNode<T>* attachAsRChild(BinNode<T>* parent, BinNode<T>* node);

	// Attach another tree to one node
	// Return the original root node of tree that added
	BinNode<T>* attachAsLChild(BinNode<T>* parent, BinTree& tree);
	BinNode<T>* attachAsRChild(BinNode<T>* parent, BinTree& tree);

	// Remove the subtree from given node
	BinNode<T>* remove(BinNode<T>* node);

	// Move the subtree from given node to a new tree
	BinTree<T>* secede(BinNode<T>* node);

	template<typename FUNC> void traversalInorder(FUNC func) { if (_root)_root->traversalInorder(func); }
	template<typename FUNC> void traversalPreorder(FUNC func) { if (_root)_root->traversalPreorder(func); }
	template<typename FUNC> void traversalPostorder(FUNC func) { if (_root)_root->traversalPostorder(func); }
	template<typename FUNC> void traversalLevel(FUNC func) { if (_root)_root->traversalLevel(func); }

	bool operator==(const BinTree& tree) { return _root && tree._root&&_root == tree._root; }
	bool operator!=(const BinTree& tree) { return !(*this == tree); }

	BinNode<T>* zig(BinNode<T>* node);
	BinNode<T>* zag(BinNode<T>* node);

	virtual void printTree(int wordWidth = 4);

protected:

	virtual void updateHeight(BinNode<T>* node);
	void updateHeightAbove(BinNode<T>* node);

	BinNode<T>*& fromParentTo(const BinNode<T>* node);

	virtual void internalPrintData(int currentHeight, int wordWidth, BinNode<T>* node);

private:

	void internalRemove(BinNode<T>* node);

};

template<typename T>
inline BinNode<T>* BinTree<T>::zig(BinNode<T>* node)
{
	BinNode<T>* lc = node->lChild;
	if (!lc) return nullptr;
	fromParentTo(node) = lc;
	lc->parent = node->parent;
	node->parent = nullptr;
	node->lChild = nullptr;
	updateHeight(node);  // We cut off half of the subtree of node, the height may change.
	attachAsLChild(node, lc->rChild);
	attachAsRChild(lc, node);
	return lc;
}

template<typename T>
inline BinNode<T>* BinTree<T>::zag(BinNode<T>* node)
{
	BinNode<T>* rc = node->rChild;
	if (!rc) return nullptr;
	fromParentTo(node) = rc;
	rc->parent = node->parent;
	node->parent = nullptr;
	node->rChild = nullptr;
	updateHeight(node);  // We cut off half of the subtree of node, the height may change.
	attachAsRChild(node, rc->lChild);
	attachAsLChild(rc, node);
	return rc;
}

template<typename T>
void BinTree<T>::printTree(int wordWidth)
{
	BinNode<T>* p = _root;
	Queue<BinNode<T>*> q;
	q.push(p);

	int totalHeight = _root->_height + 1;

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
inline void BinTree<T>::internalPrintData(int currentWidth, int wordWidth, BinNode<T>* node)
{
#define CENTER 1

#if CENTER
	if (currentWidth != wordWidth)
	{
		std::cout << std::right;
		std::cout << std::setw(currentWidth / 2) << node->data << std::setw(currentWidth / 2) << ' ';
		std::cout << std::left;
	}
	else
#endif
	{
		std::cout << std::setw(currentWidth) << node->data;
	}
}

template<typename T>
inline void BinTree<T>::updateHeight(BinNode<T>* node)
{
	if (!node) return;
	int lHeight = stature(node->lChild);
	int rHeight = stature(node->rChild);
	node->_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
}

template<typename T>
inline void BinTree<T>::updateHeightAbove(BinNode<T>* node)
{
	while (node)
	{
		updateHeight(node);
		node = node->parent;
	}
}

template<typename T>
inline BinNode<T>* BinTree<T>::insertAsRoot(const T& data)
{
	assert(!_root);
	_root = new BinNode<T>(data);
	_size = 1;
	return _root;
}

template<typename T>
inline BinNode<T>* BinTree<T>::insertAsLChild(BinNode<T>* parent, const T & data)
{
	assert(parent);
	BinNode<T>* node = new BinNode<T>(data);
	parent->insertAsLChild(node);
	++_size;
	updateHeightAbove(parent);
	return parent->lChild;
}

template<typename T>
inline BinNode<T>* BinTree<T>::insertAsRChild(BinNode<T>* parent, const T & data)
{
	assert(parent);
	BinNode<T>* node = new BinNode<T>(data);
	parent->insertAsRChild(node);
	++_size;
	updateHeightAbove(parent);
	return parent->rChild;
}

template<typename T>
inline BinNode<T>* BinTree<T>::attachAsLChild(BinNode<T>* parent, BinNode<T>* node)
{
	assert(parent && !parent->hasLChild());
	if (!node) return nullptr;
	BinNode<T>* originalParent = node->parent;
	fromParentTo(node) = nullptr;
	node->parent = parent;
	parent->lChild = node;
	updateHeightAbove(parent);
	updateHeightAbove(originalParent);
	return node;
}

template<typename T>
inline BinNode<T>* BinTree<T>::attachAsRChild(BinNode<T>* parent, BinNode<T>* node)
{
	assert(parent && !parent->hasRChild());
	if (!node) return nullptr;
	BinNode<T>* originalParent = node->parent;
	fromParentTo(node) = nullptr;
	node->parent = parent;
	parent->rChild = node;
	updateHeightAbove(parent);
	updateHeightAbove(originalParent);
	return node;
}

template<typename T>
inline BinNode<T>* BinTree<T>::attachAsLChild(BinNode<T>* parent, BinTree & tree)
{
	assert(parent && !parent->lChild);
	if (!tree._root) return nullptr;
	parent->lChild = tree._root;
	tree._root->parent = parent;
	tree._root = nullptr;
	_size += tree._size;
	tree._size = 0;
	updateHeightAbove(parent);
	return parent->lChild;
}

template<typename T>
inline BinNode<T>* BinTree<T>::attachAsRChild(BinNode<T>* parent, BinTree & tree)
{
	assert(parent && !parent->rChild);
	if (!tree._root) return nullptr;
	parent->rChild = tree._root;
	tree._root->parent = parent;
	tree._root = nullptr;
	_size += tree._size;
	tree._size = 0;
	updateHeightAbove(parent);
	return parent->rChild;
}

template<typename T>
inline BinNode<T>* BinTree<T>::remove(BinNode<T>* node)
{
	if (!node) return nullptr;
	BinNode<T>* ret = node->parent;
	if (node->parent)
	{
		fromParentTo(node) = nullptr;
		updateHeightAbove(node->parent);
	}
	internalRemove(node);
	return ret;
}

template<typename T>
inline void BinTree<T>::internalRemove(BinNode<T>* node)
{
	assert(node);
	if (node->lChild) internalRemove(node->lChild);
	if (node->rChild) internalRemove(node->rChild);
	delete node;
	node = nullptr;
	--_size;
}

template<typename T>
inline BinTree<T>* BinTree<T>::secede(BinNode<T>* node)
{
	if (node->parent)
	{
		fromParentTo(node) = nullptr;
		updateHeightAbove(node->parent);
	}
	node->parent = nullptr;
	BinTree<T>* newTree =  new BinTree<T>(node);
	newTree->_size = node->size();
	_size -= newTree->_size;
	return newTree;
}

template<typename T>
inline BinNode<T>*& BinTree<T>::fromParentTo(const BinNode<T>* node)
{
	// This assume given node is in this tree
	if (node->isLChild()) return node->parent->lChild;
	else if (node->isRChild()) return node->parent->rChild;
	else if (node->isRoot()) return _root;
	else assert(false);		
}