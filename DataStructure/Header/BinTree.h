#pragma once

#include "BinNode.h"

template<typename T>
class BinTree
{
public:
	BinTree(BinNode<T>* inRoot = nullptr) :_root(inRoot) { _size = inRoot ? 1 : 0; }
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

	BinNode<T>* attachAsLChild(BinNode<T>* parent, BinTree& subTree);
	BinNode<T>* attachAsRChild(BinNode<T>* parent, BinTree& subTree);

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


protected:

	void updateHeight(BinNode<T>* node);
	void updateHeightAbove(BinNode<T>* node);

	BinNode<T>*& fromParentTo(const BinNode<T>* node);

private:

	void internalRemove(BinNode<T>* node);

};

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
inline BinNode<T>* BinTree<T>::attachAsLChild(BinNode<T>* parent, BinTree & subTree)
{
	assert(parent);
	if (!subTree._root) return nullptr;
	parent->lChild = subTree._root;
	subTree._root->parent = parent;
	subTree._root = nullptr;
	_size += subTree._size;
	subTree._size = 0;
	updateHeightAbove(parent);
	return parent->lChild;
}

template<typename T>
inline BinNode<T>* BinTree<T>::attachAsRChild(BinNode<T>* parent, BinTree & subTree)
{
	assert(parent);
	if (!subTree._root) return nullptr;
	parent->rChild = subTree._root;
	subTree._root->parent = parent;
	subTree._root = nullptr;
	_size += subTree._size;
	subTree._size = 0;
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