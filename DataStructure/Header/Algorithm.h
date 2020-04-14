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
}