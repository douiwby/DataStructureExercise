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

	template<typename Iterator>
	Iterator unique(Iterator b, Iterator e)
	{
		if (b == e) return e;
		Iterator currentPosition(b);
		while (++b != e)
		{
			while (*b == *currentPosition) ++b;
			if (!(*++currentPosition == *b))
				*currentPosition = *b;
		}
		return ++currentPosition;
	}

	template<typename Iterator, typename BinaryPredicate>
	Iterator unique(Iterator b, Iterator e, BinaryPredicate pred)
	{
		if (b == e) return e;
		Iterator currentPosition(b);
		while (++b != e)
		{
			while (pred(*b,*e)) ++b;
			if (!(*++currentPosition == *b))
				*currentPosition = *b;
		}
		return ++currentPosition;
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