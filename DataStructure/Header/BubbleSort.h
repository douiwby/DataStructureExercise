#pragma once
#include <cassert>
#include <utility>

template<typename T, unsigned N>
void bubbleSort_Array(T(&a)[N])
{
	assert(N != 0);
	bool isDone = false;
	while (!isDone)
	{
		isDone = true;
		unsigned n = N - 1;
		for (unsigned i = 0; i < n; ++i)
		{
			if (a[i+1] < a[i])
			{
				std::swap(a[i], a[i + 1]);
				isDone = false;
			}
		}
		--n; // The last element is the largest one, no need to compare again.
	}
}

#include <iostream>

template <typename T, unsigned N>
void printArray(const T(&a)[N])
{
	assert(N != 0);
	for (int i = 0; i < N; ++i)
	{
		std::cout << a[i];
		if (i != N - 1) std::cout << ", ";
	}
	std::cout << std::endl;
}