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

	ListIterator find(const T& v) { return Algorithm::find(begin(), end(), v); }
private:
	ListNode<T>* header;
	ListNode<T>* trailer;
	SizeType _size;

	void init();
public:
	void printList() const;
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
		assert(pNode && pNode->next);
		pNode = pNode->next;
		return *this;
	}
	ListIterator operator++(int)
	{
		assert(pNode && pNode->next);
		ListIterator ret(pNode);
		pNode = pNode->next;
		return ret;
	}
	ListIterator& operator--()
	{
		assert(pNode && pNode->prev && pNode->prev->prev);  //Can not decrease at begin()
		pNode = pNode->prev;
		return *this;
	}
	ListIterator operator--(int)
	{
		assert(pNode && pNode->prev && pNode->prev->prev);  //Can not decrease at begin()
		ListIterator ret(pNode);
		pNode = pNode->prev;
		return ret;
	}
	bool operator==(const ListIterator& rhs) { return this->pNode == rhs.pNode; }
	bool operator!=(const ListIterator& rhs) { return !(*this == rhs); }
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
typename List<T>::ListIterator List<T>::insert(const ListIterator& it, const T& v)
{
	assert(it.pNode && it.pNode->prev);  //Can not insert before header
	ListNode<T>* pNewNode = new ListNode<T>(v,it.pNode->prev,it.pNode);
	it.pNode->prev->next = pNewNode;
	it.pNode->prev = pNewNode;
	++_size;
	return pNewNode;
}

template<typename T>
typename List<T>::ListIterator List<T>::insert(const ListIterator& it, const ListIterator& b, const ListIterator& e)
{
	assert(it.pNode && it.pNode->prev);  //Can not insert before header
	ListNode<T>* prev = it.pNode->prev;
	for (ListIterator _begin = b; _begin != e; ++_begin)
	{
		insert(it, *_begin);
	}
	return prev->next;
}

template<typename T>
typename List<T>::ListIterator List<T>::erase(const ListIterator& it)
{
	assert(it.pNode && it.pNode->next);  //Can not erase at trailer
	ListNode<T>* ret = it.pNode->next;
	it.pNode->prev->next = it.pNode->next;
	it.pNode->next->prev = it.pNode->prev;
	delete it.pNode;
	--_size;
	return ret;
}

#include <iostream>

template<typename T>
void List<T>::printList() const
{
	for (auto it = begin(); it != end(); ++it)
	{
		std::cout << *it;
		if (it != --end()) std::cout << ", ";
	}
	std::cout << std::endl;
}