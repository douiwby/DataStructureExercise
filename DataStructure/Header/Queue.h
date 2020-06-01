#pragma once

#include "List.h"

template<typename T, typename Container = List<T>>
class Queue
{
public:
	// --------------------
	// Type declaration
	// --------------------
	typedef typename Container::SizeType SizeType;

	// --------------------
	// Constructor and destructor
	// --------------------
	Queue() = default;
	Queue(const Container& c) : data(c) {}
	~Queue() = default;

	// --------------------
	// Member operator
	// --------------------
	SizeType size() const { return data.size(); }
	bool empty() const { return data.empty(); }
	T& front() const { return data.front(); }
	T& back() const { return data.back(); }

	void push(const T& v) { return data.push_back(v); }
	void pop() { return data.pop_front(); }

private:
	Container data;

	// --------------------
	// Friend declaration
	// --------------------
	friend void printAdaptor<Queue>(const Queue& a, const char* separateSymbol);
};