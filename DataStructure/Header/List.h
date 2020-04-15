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
	typedef unsigned SizeType;
	class ListIterator;

	List() { init(); }
	List(const List& l)
	{
		init();
		insert(begin(), l.begin(), l.end());
	}
	~List();
	void clear();

	SizeType size() const { return _size; }

	ListIterator begin() const { return ListIterator(header->next); }
	ListIterator end() const { return ListIterator(trailer); }
	ListIterator insert(const ListIterator& it, const T& v);
	ListIterator insert(const ListIterator& it, const ListIterator& b, const ListIterator& e);
	ListIterator erase(const ListIterator& it);
	ListIterator erase(const ListIterator& b, const ListIterator& e);

	ListIterator find(const T& v) { return Algorithm::find(begin(), end(), v); }

	ListIterator searchFirstGreater(const T&v, ListIterator b, ListIterator e)
	{
		return Algorithm::find_if(b, e, [&v](const T& element) {return v < element; });
	}
	static ListIterator moveElement(const ListIterator& src, const ListIterator& dest);
	static void swapElement(ListIterator lhs, ListIterator rhs);
	void insertionSort(ListIterator b, const ListIterator& e);
	void insertionSort() { insertionSort(begin(), end()); }
	void unique();

private:
	ListNode<T>* header;
	ListNode<T>* trailer;
	SizeType _size;

	void init();
};

template<typename T>
class List<T>::ListIterator
{
	friend class List<T>;
public:
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
inline typename List<T>::ListIterator List<T>::moveElement(const ListIterator & src, const ListIterator & dest)
{
	assert(src.pNode && src.pNode->next);  //Can not move end()
	src.pNode->prev->next = src.pNode->next;
	src.pNode->next->prev = src.pNode->prev;
	src.pNode->prev = dest.pNode->prev;
	src.pNode->next = dest.pNode;
	dest.pNode->prev->next = src.pNode;
	dest.pNode->prev = src.pNode;
	return src;
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