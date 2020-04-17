#pragma once
#include <cassert>

#include "Algorithm.h"

template<typename T> class List;

template<typename T>
class ListNode
{
	friend class List<T>;
public:
	ListNode(T val = T(), ListNode<T>* inPrev = nullptr, ListNode<T>* inNext = nullptr) :data(val), prev(inPrev), next(inNext) {}
private:
	ListNode<T>* prev;
	ListNode<T>* next;
	T data;
};

template<typename T>
class List
{
public:
	// --------------------
	// Type declaration
	// --------------------

	typedef unsigned SizeType;
	class ListIterator;

	// --------------------
	// Constructor and destructor
	// --------------------

	List() { init(); }
	List(const List& l)
	{
		init();
		insert(begin(), l.begin(), l.end());
	}
	~List();
	void clear();

	// --------------------
	// Get member
	// --------------------

	SizeType size() const { return _size; }
	ListIterator begin() const { return ListIterator(header->next); }
	ListIterator end() const { return ListIterator(trailer); }

	// --------------------
	// Algorithms
	// --------------------

	ListIterator insert(const ListIterator& it, const T& v);
	ListIterator insert(const ListIterator& it, const ListIterator& b, const ListIterator& e);
	ListIterator erase(const ListIterator& it);
	ListIterator erase(const ListIterator& b, const ListIterator& e);
	void unique();

	ListIterator find(const T& v) { return Algorithm::find(begin(), end(), v); }
	ListIterator searchFirstGreater(const T&v, ListIterator b, ListIterator e) { return Algorithm::find_if(b, e, [&v](const T& element) {return v < element; }); }
	// Move elements to before dest
	static void moveElement(const ListIterator& src, const ListIterator& dest);
	static void moveElement(const ListIterator& b, const ListIterator& e, const ListIterator& dest);
	static void swapElement(ListIterator lhs, ListIterator rhs);

	void insertionSort(ListIterator b, const ListIterator& e);
	void insertionSort() { insertionSort(begin(), end()); }

	ListIterator searchMax(const ListIterator& b, const ListIterator& e);
	void selectionSort(ListIterator b, ListIterator e);
	void selectionSort() { selectionSort(begin(), end()); }

	// Todo: use more readable one to replace this
	void mergeSort(ListIterator& b, ListIterator& e);
	void mergeSort() { auto _beg = begin(),_end=end(); mergeSort(_beg, _end); }

private:
	ListNode<T>* header;
	ListNode<T>* trailer;
	SizeType _size;

	void init();

	// Merge two element ranges and move it to before dest. Return the first element after merge.
	ListIterator innerMerge(ListIterator b1, const ListIterator& e1, ListIterator b2, const ListIterator& e2, const ListIterator& dest);
};

template<typename T>
class List<T>::ListIterator
{
	friend class List<T>;
public:
	typedef int DifferenceType;
	ListIterator(ListNode<T>* p) :pNode(p) {}
	T& operator*()
	{
		assert(pNode);
		return pNode->data;
	}
	ListIterator& operator++()
	{
		assert(pNode && pNode->next);  //Can not increase at trailer
		pNode = pNode->next;
		return *this;
	}
	ListIterator operator++(int)
	{
		assert(pNode && pNode->next);  //Can not increase at trailer
		ListIterator ret(pNode);
		pNode = pNode->next;
		return ret;
	}
	ListIterator& operator--()
	{
		assert(pNode && pNode->prev && pNode->prev->prev);  //Can not decrease at header and begin()
		pNode = pNode->prev;
		return *this;
	}
	ListIterator operator--(int)
	{
		assert(pNode && pNode->prev && pNode->prev->prev);  //Can not decrease at header and begin()
		ListIterator ret(pNode);
		pNode = pNode->prev;
		return ret;
	}
	bool operator==(const ListIterator& rhs) const { return this->pNode == rhs.pNode; }
	bool operator!=(const ListIterator& rhs) const { return !(*this == rhs); }
	ListIterator operator+(DifferenceType i) const
	{
		ListIterator ret = *this;
		if (i >= 0)
		{
			while (i != 0)
			{
				++ret;
				--i;
			}
			return ret;
		}
		else return *this - (-i);
	}
	ListIterator operator-(DifferenceType i) const
	{
		ListIterator ret = *this;
		if (i >= 0)
		{
			while (i != 0)
			{
				--ret;
				--i;
			}
			return ret;
		}
		else return *this + (-i);
	}
	DifferenceType operator-(const ListIterator& v) const
	{
		ListIterator it = *this;
		DifferenceType ret = 0;
		if (it == v) return ret;
		// Assume *this is larger then v
		while (it.pNode->prev->prev)
		{
			--it; ++ret;
			if (it == v) return ret;
		}
		//Reach begin(), *this is less then v
		while (it.pNode->next)
		{
			++it; --ret;
			if (it == v) return ret;
		}
		// Reach end(), *this is not in the same list of v
		assert(false);
		return ret;
	}
private:
	ListNode<T>* pNode;
};

template<typename T>
void List<T>::init()
{
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->next = trailer;
	trailer->prev = header;
	_size = 0;
}

template<typename T>
List<T>::~List()
{
	clear();
	delete header;
	delete trailer;
}

template<typename T>
void List<T>::clear()
{
	//for (ListIterator it = end(); it != begin(); it = end())  //erase() will remain iterator point to the deleted element, do not use --it at condition.
	//{
	//	erase(--it);
	//}
	while (begin() != end())
	{
		erase(--end());
	}
}

template<typename T>
inline typename List<T>::ListIterator List<T>::insert(const ListIterator& it, const T& v)
{
	//assert(it.pNode && it.pNode->prev);  //Can not insert before header //Assume the Iterator is in range of begin() and end()
	ListNode<T>* pNewNode = new ListNode<T>(v,it.pNode->prev,it.pNode);
	it.pNode->prev->next = pNewNode;
	it.pNode->prev = pNewNode;
	++_size;
	return pNewNode;
}

template<typename T>
typename List<T>::ListIterator List<T>::insert(const ListIterator& it, const ListIterator& b, const ListIterator& e)
{
	//assert(it.pNode && it.pNode->prev);  //Can not insert before header //Assume the Iterator is in range of begin() and end()
	ListNode<T>* prev = it.pNode->prev;
	for (ListIterator _begin = b; _begin != e; ++_begin)
	{
		assert(_begin.pNode && _begin.pNode->next); //Can not read end()
		insert(it, *_begin);
	}
	return prev->next;
}

template<typename T>
inline typename List<T>::ListIterator List<T>::erase(const ListIterator& it)
{
	assert(it.pNode && it.pNode->next);  //Can not erase end()
	ListNode<T>* ret = it.pNode->next;
	it.pNode->prev->next = it.pNode->next;
	it.pNode->next->prev = it.pNode->prev;
	delete it.pNode;
	--_size;
	return ret;
}

template<typename T>
typename List<T>::ListIterator List<T>::erase(const ListIterator& b, const ListIterator& e)
{
	assert(b.pNode && b.pNode->next);  //Can not erase end()
	if (b == e) return e;
	ListIterator last(e.pNode->prev);
	ListIterator beforeFirst(b.pNode->prev);
	b.pNode->prev->next = e.pNode;
	e.pNode->prev = b.pNode->prev;
	for (; last != beforeFirst;)
	{
		ListIterator toBeDeleted = last--;
		delete toBeDeleted.pNode;
		--_size;
	}
	return e;
}

template<typename T>
inline void List<T>::moveElement(const ListIterator & src, const ListIterator & dest)
{
	assert(src.pNode && src.pNode->next);  //Can not move end()
	auto last = src;
	++last;
	moveElement(src, last, dest);
}

template<typename T>
inline void List<T>::moveElement(const ListIterator& b, const ListIterator& e, const ListIterator& dest)
{
	assert(b.pNode && b.pNode->next);  //Can not move end()
	if (b == e) return;
	ListIterator last(e.pNode->prev);
	b.pNode->prev->next = last.pNode->next;
	last.pNode->next->prev = b.pNode->prev;
	b.pNode->prev = dest.pNode->prev;
	last.pNode->next = dest.pNode;
	dest.pNode->prev->next = b.pNode;
	dest.pNode->prev = last.pNode;
}

template<typename T>
inline void List<T>::swapElement(ListIterator lhs, ListIterator rhs)
{
	assert(lhs.pNode && lhs.pNode->next);  //Can not move end()
	assert(rhs.pNode && rhs.pNode->next);  //Can not move end()
	ListIterator lElement = lhs++;
	ListIterator rElement = rhs++;
	moveElement(lElement, rhs);
	moveElement(rElement, lhs);
}

template<typename T>
void List<T>::insertionSort(ListIterator b, const ListIterator& e)
{
	assert(b.pNode && b.pNode->next);  //Can not move end()
	if (b == e) return;
	ListIterator it(b.pNode->next);
	for (; it != e;)
	{
		ListIterator nextIterator(it.pNode->next);
		ListIterator searchResult = searchFirstGreater(*it, b, it);
		if (searchResult != it) moveElement(it, searchResult);
		if (searchResult == b) b = --searchResult; // always keep b as the first iterator
		it = nextIterator;
	}
}

template<typename T>
void List<T>::unique()
{
	if (_size < 2) return;
	for (auto it = begin(); it != end();)
	{
		auto first = it++;
		if (*first == *it)
		{
			erase(it);
			it = first;
		}
	}
}

template<typename T>
typename List<T>::ListIterator List<T>::searchMax(const ListIterator& b, const ListIterator& e)
{
	auto maxElement = b, it = b;
	++it;
	for (; it != e; ++it)
	{
		if (!(*it < *maxElement)) maxElement = it;
	}
	return maxElement;
}

template<typename T>
void List<T>::selectionSort(ListIterator b, ListIterator e)
{
	assert(b.pNode && b.pNode->next);  //Can not move end()
	while (e != b)
	{
		auto maxElement = searchMax(b,e);
		if (maxElement == b) ++b;
		moveElement(maxElement, e);
		if (b.pNode->next == maxElement.pNode) break;
		e = maxElement;
	}
}

template<typename T>
void List<T>::mergeSort(ListIterator& b, ListIterator& e)
{
	assert(b.pNode && b.pNode->next);  //Can not move end()
	typename ListIterator::DifferenceType diff = e - b;
	assert(diff >= 0);
	ListIterator mid = b + (diff / 2);

	// Insert a new node to avoid moveElement's dest is in range b~e
	ListIterator tempBeginNode = insert(b, T());

	if (diff == 2)
	{
		b = innerMerge(b, b + 1, e - 1, e, tempBeginNode);
		e = tempBeginNode + 1;
	}
	else if (diff == 1 || diff == 0)
	{
		// Do nothing
	}
	else
	{
		mergeSort(b, mid);
		mergeSort(mid, e);
		b = innerMerge(b, mid, mid, e, tempBeginNode);
		e = tempBeginNode + 1;
	}

	erase(tempBeginNode);
}

template<typename T>
typename List<T>::ListIterator List<T>::innerMerge(ListIterator b1, const ListIterator& e1, ListIterator b2, const ListIterator& e2, const ListIterator& dest)
{
	assert(b1.pNode && b1.pNode->next);  //Can not move end()
	assert(b2.pNode && b2.pNode->next);  //Can not move end()
	assert((e1 - b1 >= 0) && (e2 - b2) >= 0);

	// Insert a new node to avoid e1/e2 being moved if b1==e2 or b2==e1
	ListIterator tempEndNode1 = insert(e1, T());
	ListIterator tempEndNode2 = insert(e2, T());

	// All element in one list is less then another list, no need to move one by one.
	if (*(tempEndNode1 - 1) < *b2)
	{

		moveElement(b1, tempEndNode1, dest);
		moveElement(b2, tempEndNode2, dest);

		erase(tempEndNode1);
		erase(tempEndNode2);
		return b1;
	}
	if (*(tempEndNode2 - 1) < *b1)
	{
		moveElement(b2, tempEndNode2, dest);
		moveElement(b1, tempEndNode1, dest);

		erase(tempEndNode1);
		erase(tempEndNode2);
		return b2;
	}

	// Compare the first element of two list and move the smaller element to dest
	ListIterator firstElement = (*b1 < *b2)?b1:b2;
	while (b1 != tempEndNode1 && b2 != tempEndNode2)
	{
		if (*b1 < *b2)
		{
			moveElement(b1++, dest);
		}
		else
		{
			moveElement(b2++, dest);
		}
	}

	// One list has been all merged, append another list
	if (b1 == tempEndNode1)
	{
		moveElement(b2, tempEndNode2, dest);
	}
	if (b2 == tempEndNode2)
	{
		moveElement(b1, tempEndNode1, dest);
	}

	erase(tempEndNode1);
	erase(tempEndNode2);
	return firstElement;
}