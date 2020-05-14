#pragma once

#include "Vector.h"

template<typename T, typename Container = Vector<T>>
class Stack
{
public:
	// --------------------
	// Type declaration
	// --------------------
	typedef typename Container::SizeType SizeType;

	// --------------------
	// Constructor and destructor
	// --------------------
	Stack() = default;
	Stack(const Container& c) : data(c) {}
	~Stack() = default;

	// --------------------
	// Member operator
	// --------------------
	SizeType size() const { return data.size(); }
	bool empty() const { return data.empty(); }
	T& top() const { return data.back(); }

	void push(const T& v) { return data.push_back(v); }
	void pop() { return data.pop_back(); }

private:
	Container data;

	// --------------------
	// Friend declaration
	// --------------------
	friend void printAdaptor<Stack>(const Stack& a, char* separateSymbol);
};