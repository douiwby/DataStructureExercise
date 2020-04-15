#pragma once

namespace Algorithm
{
	template<typename Iterator, typename T>
	Iterator find(Iterator b, Iterator e, const T& v)
	{
		for (; b != e; ++b)
		{
			if (*b == v) return b;
		}
		return e;
	}

	template<typename Iterator, typename UnaryPredicate>
	Iterator find_if(Iterator b, Iterator e, UnaryPredicate pred)
	{
		for (; b != e; ++b)
		{
			if (pred(*b)) return b;
		}
		return e;
	}
}

#include <iostream>

template <typename Iterator>
void printByIterator(Iterator b, Iterator e)
{
	for (; b != e; )
	{
		std::cout << *b;
		++b;
		if (b != e) std::cout << ",";
	}
	std::cout << std::endl;
}

template<typename Container>
void printContainer(const Container& c)
{
	printByIterator(c.begin(), c.end());
}