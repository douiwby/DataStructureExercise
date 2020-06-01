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

	template<typename Iterator, typename T>
	Iterator upper_bound(Iterator b, Iterator e, const T& v)
	{
		if (b == e) return b;
		Iterator mid = b + (e - b) / 2;
		Iterator searchBegin = b;
		Iterator searchEnd = e;
		do 
		{
			if (v < *mid)
			{
				searchEnd = mid;
			}
			else
			{
				searchBegin = ++mid;
			}
			mid = searchBegin + (searchEnd - searchBegin) / 2;
		} while (searchBegin != searchEnd);
		return mid;
	}

	template<typename Iterator, typename T>
	Iterator lower_bound(Iterator b, Iterator e, const T& v)
	{
		if (b == e) return b;
		Iterator mid = b + (e - b) / 2;
		Iterator searchBegin = b;
		Iterator searchEnd = e;
		do
		{
			if (*mid < v)
			{
				searchBegin = ++mid;
			}
			else
			{
				searchEnd = mid;
			}
			mid = searchBegin + (searchEnd - searchBegin) / 2;
		} while (searchBegin != searchEnd);
		return mid;
	}

	template<typename Iterator, typename T>
	Iterator binarySearch(Iterator b, Iterator e, const T& v)
	{
		Iterator result = lower_bound(b, e, v);
		if ( result!= e && *result == v) return result;
		else return e;
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

	template<typename Iterator, typename Compare>
	bool is_sorted(Iterator b, Iterator e, Compare comp)
	{
		if (b == e) return true;
		Iterator next = b;
		while (++next != e)
		{
			if (comp(*next, *b)) return false;  //Note it's not if(!comp(*b,*next))
			++b;
		}
		return true;
	}

	template<typename Iterator>
	bool is_sorted(Iterator b, Iterator e)
	{
		if (b == e) return true;
		Iterator next = b;
		while (++next != e)
		{
			if (*next < *b) return false;
			++b;
		}
		return true;
	}
}

#include <iostream>

template <typename Iterator>
void printByIterator(Iterator b, Iterator e, const char* separateSymbol = ",")
{
	for (; b != e; )
	{
		std::cout << *b;
		++b;
		if (b != e && separateSymbol) std::cout << separateSymbol;
	}
	std::cout << std::endl;
}

template<typename Container>
void printContainer(const Container& c, const char* separateSymbol = ",")
{
	printByIterator(c.begin(), c.end(), separateSymbol);
}

template<typename Adaptor>
void printAdaptor(const Adaptor& a, const char* separateSymbol = ",")
{
	printContainer(a.data, separateSymbol);
}